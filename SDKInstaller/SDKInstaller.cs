using System;
using System.IO;
using System.Linq;
using System.Threading;
using System.Collections.Generic;
using System.Net;
using System.Diagnostics;

namespace SDKInstaller
{
    public sealed class SDKInstaller
    {
        const string HostName = "server3.binarycompany.com";
        private readonly Configuration _config;
        private int _progress;
        private readonly object _updateLock = new object();

        public SDKInstaller(Configuration cfg)
        {
            _config = cfg;
        }

        /// <summary>
        /// Does the actual install.
        /// </summary>
        public void Install()
        {
            var logsFolder = Path.GetFullPath("../Logs");

            Console.WriteLine("> SEMOS SDK Installer.");
            
            EnsureDirectory(logsFolder);

            var hostType = _config.ToolsHostType;
            var targetType = _config.ToolsTargetType;

            var files = new List<string>
                            {
                                // Process Toolchain
                                string.Format("Toolchain/{0}/Common.7z", hostType),
                                string.Format("Toolchain/{0}/{1}.7z", hostType, targetType),
                                // Process Libraries
                                string.Format("Libs/{0}/Core.7z", targetType)
                            };

            files.AddRange(_config.AdditionalLibraries.Select(lib => string.Format("Libs/{0}/{1}.7z", targetType, lib)));

            Console.WriteLine("> Downloading Files.");
           
            DownloadFiles(files, hostType);
            
            Console.WriteLine("> Extracting Files.");
           
            ExtractFiles(files);


            #region Clean up
            Directory.SetCurrentDirectory("./Tools/");
            foreach (String s in files)
            {
                File.Delete("Downloaded/" + s);
            }
            RecursiveDirectoryDelete("./Downloaded/");
            #endregion

        }

        private static void EnsureDirectory(string directoryName)
        {
            if (Directory.Exists(directoryName)) return;
            Directory.CreateDirectory(directoryName);
        }

        private void DownloadFiles(IEnumerable<string> files, string hostType)
        {
            var clnt = new WebClient();
            clnt.DownloadProgressChanged += ClientDownloadProgressChanged;

            
            Console.WriteLine("\t Downloading 7zip");
            
            var zipToolFilename = string.Format("7za{0}", (_config.IsWindows ? ".exe" : ""));
            var zipToolUri = new Uri(string.Format("http://{0}/downloads/Toolchain/{1}/{2}", HostName, hostType, zipToolFilename));
            clnt.DownloadFileAsync(zipToolUri, zipToolFilename);

            while (clnt.IsBusy)
            { Thread.Sleep(10); }

            Console.WriteLine(); // The console is still on the previous line.
            _progress = 0;
            

            foreach (var filename in files)
            {

                var fullfileName = Path.GetFullPath(string.Format("Downloaded/{0}", filename));
                var directoryName = Path.GetDirectoryName(fullfileName);
                
                Console.WriteLine("\t Downloading {0}.", filename);

                EnsureDirectory(directoryName);

                clnt.DownloadFileAsync(new Uri(string.Format("http://{0}/downloads/{1}", HostName, filename)), "Downloaded/" + filename);

                while (clnt.IsBusy)
                {
                    Thread.Sleep(100);
                }
                Console.WriteLine(); // The console is still on the previous line.
                _progress = 0;
            }
        }

        private void ExtractFiles(IEnumerable<string> files)
        {
            Directory.SetCurrentDirectory("../"); 
            
            var z7FileName = Path.GetFullPath("./Tools/7za" + ((_config.IsWindows) ? ".exe" : ""));

            foreach (var filename in files)
            {
                Console.WriteLine("\t Extracting {0}.", Path.GetFileNameWithoutExtension(filename));
                var arguments = string.Format("-y x \"./Tools/Downloaded/{0}\"", filename);
                var p = new Process
                            {
                                StartInfo = new ProcessStartInfo(z7FileName, arguments)
                                                {
                                                    CreateNoWindow = true,
                                                    UseShellExecute = false,
                                                 }
                            };
               
                if (p.Start())
                {
                    while (!p.HasExited)
                    {
                        Thread.Sleep(200);
                    }
                    Console.ForegroundColor = ConsoleColor.Green;
                    Console.WriteLine("\t Extracted {0}.", Path.GetFileNameWithoutExtension(filename));
                    Console.ResetColor();
                }
                else
                {
                    Console.WriteLine("Failed to start extraction of {0}.", filename);
                }
            }
        }


        void ClientDownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
        {
            if (_progress == e.ProgressPercentage || e.ProgressPercentage == 0) return;

            lock (_updateLock)
            {
                Console.CursorLeft = 0;
                _progress = e.ProgressPercentage;
                if (_progress < 100)
                {
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.Write("\t {0}% Done [{1}] {2}/{3}kb", e.ProgressPercentage,
                                  (new string('=', _progress / 5) + (_progress < 100 ? ">" : "")).PadRight(20), e.BytesReceived / 1024, e.TotalBytesToReceive / 1024);
                    Console.ResetColor();
                }
                else
                {
                    Console.ForegroundColor = ConsoleColor.Green;
                    Console.Write("\t {0}% Done {1}kb {2}", e.ProgressPercentage, e.TotalBytesToReceive / 1024, new string(' ',30));
                    Console.ResetColor();
                }
            }
        }

        private static void RecursiveDirectoryDelete(string path)
        {
            Directory.Delete(path,true);
        }

    }
}
