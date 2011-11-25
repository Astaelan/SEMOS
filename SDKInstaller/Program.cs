using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Threading;
using System.Xml;
using System.Xml.Serialization;

namespace SDKInstaller
{
    internal static class Program
    {
        private static string sURL = "https://github.com/downloads/Astaelan/SEMOS/";
        private static string sPackageList = "SDKInstaller.xml";
        private static string sTarget = "i686-elf";
        private static string sHost = "i386-mingw32";
        private static string[] sOptionals = new string[0];

        private static string sExtractorFile = "";
        private static string sDownloadFile = "";
        private static ManualResetEvent sDownloadEvent = new ManualResetEvent(false);
        private static object sDownloadLock = new object();
        private static bool sDownloadResult = false;

        [STAThread]
        private static void Main(string[] pCommandLine)
        {
            Arguments args = new Arguments(pCommandLine);
            if (args.Contains("url")) sURL = args["url"];
            if (args.Contains("target")) sTarget = args["target"];
            if (args.Contains("host")) sHost = args["host"];
            if (args.Contains("optional")) sOptionals = args["optional"].Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);

            Console.WriteLine("Downloading {0}...", sPackageList);
            Console.Title = string.Format("SDKInstaller: Downloading {0}", sPackageList);
            WebClient client = new WebClient();
            try { client.DownloadFile(sURL + sPackageList, sPackageList); }
            catch (Exception exc)
            {
                Console.WriteLine("Failed: {0}", exc.Message);
                return;
            }
            SDKInstaller installer = null;
            using (XmlReader reader = XmlReader.Create(sPackageList)) installer = (SDKInstaller)(new XmlSerializer(typeof(SDKInstaller))).Deserialize(reader);
            File.Delete(sPackageList);
            Console.WriteLine("Found {0} package{1}...", installer.Packages.Count, installer.Packages.Count == 1 ? "" : "s");

            SDKInstaller.SDKPackage package = installer.Packages.Find(p => p.Target == sTarget && p.Host == sHost);
            if (package == null)
            {
                Console.WriteLine("There is no SDK package available for {0}-{1}", sTarget, sHost);
                return;
            }

            client.DownloadFileCompleted += DownloadFileCompleted;
            client.DownloadProgressChanged += DownloadProgressChanged;

            sDownloadFile = string.Format("7za-{0}.exe", sHost);
            Console.WriteLine("Downloading {0}...", sDownloadFile);
            sDownloadEvent.Reset();
            client.DownloadFileAsync(new Uri(sURL + sDownloadFile), sDownloadFile);
            sDownloadEvent.WaitOne();
            sExtractorFile = sDownloadFile;

            if (!sDownloadResult) return;
            sDownloadResult = false;

            sDownloadFile = string.Format("SDK-{0}-{1}.7z", sTarget, sHost);
            Console.WriteLine("Downloading {0}...", sDownloadFile);
            sDownloadEvent.Reset();
            client.DownloadFileAsync(new Uri(sURL + sDownloadFile), sDownloadFile);
            sDownloadEvent.WaitOne();

            if (!sDownloadResult) return;
            sDownloadResult = false;

            Console.WriteLine("Extracting {0}...", sDownloadFile);
            Console.Title = string.Format("SDKInstaller: Extracting {0}", sDownloadFile);

            Process extract = new Process();
            extract.StartInfo.FileName = Path.GetFullPath(sExtractorFile);
            extract.StartInfo.Arguments = "x -y " + sDownloadFile;
            extract.StartInfo.UseShellExecute = false;
            extract.Start();
            extract.WaitForExit();

            Console.WriteLine();
            Console.WriteLine("Done, press any key to exit.");
            Console.ReadKey(true);
        }

        private static void DownloadFileCompleted(object pSender, AsyncCompletedEventArgs pArgs)
        {
            lock (sDownloadLock)
            {
                sDownloadResult = !pArgs.Cancelled;
                Console.WriteLine();
                sDownloadEvent.Set();
            }
        }

        private static void DownloadProgressChanged(object pSender, DownloadProgressChangedEventArgs pArgs)
        {
            lock (sDownloadLock)
            {
                Console.CursorLeft = 0;
                Console.Write("Progress {0,3}% [{1,-50}]", pArgs.ProgressPercentage, new string('.', pArgs.ProgressPercentage / 2));
                Console.Title = string.Format("SDKInstaller: Downloading {0}, {1}%", sDownloadFile, pArgs.ProgressPercentage);
            }
        }

    }
}
