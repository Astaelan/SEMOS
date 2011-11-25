using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.IO;

namespace SDKInstaller
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] prms)
        {
            //if (prms.Length > 0 && prms[0] == "-silent")
            //{
            {
                string buf = "";
                List<string> ValidTargets = new List<string>();
                List<string> ValidHosts = new List<string>();
                // First string is argument, second is name.
                Dictionary<string, string> ValidLibraries = new Dictionary<string, string>();
                List<string> WindowsHosts = new List<string>();

                #region Get the lists of valid hosts, targets, and libraries
                WebClient clnt = new WebClient();
                // Targets.txt
                using (StreamReader rdr = new StreamReader(new MemoryStream(clnt.DownloadData("http://server3.binarycompany.com/downloads/Targets.txt"))))
                {
                    while (!rdr.EndOfStream)
                    {
                        buf = rdr.ReadLine();
                        if (buf != "")
                            ValidTargets.Add(buf);
                    }
                }
                // Hosts.txt
                using (StreamReader rdr = new StreamReader(new MemoryStream(clnt.DownloadData("http://server3.binarycompany.com/downloads/Hosts.txt"))))
                {
                    while (!rdr.EndOfStream)
                    {
                        buf = rdr.ReadLine();
                        if (buf != "")
                            ValidHosts.Add(buf);
                    }
                }
                // Libs.txt
                using (StreamReader rdr = new StreamReader(new MemoryStream(clnt.DownloadData("http://server3.binarycompany.com/downloads/Libs.txt"))))
                {
                    while (!rdr.EndOfStream)
                    {
                        buf = rdr.ReadLine();
                        if (buf != "")
                        {
                            string[] tmp = buf.Split(new char[] { '|' });
                            if (tmp.Length != 2)
                                throw new Exception("To many arguments in the lib name!");
                            ValidLibraries.Add(tmp[1].ToLower(), tmp[0]);
                        }
                    }
                }
                // WindowsHosts.txt
                using (StreamReader rdr = new StreamReader(new MemoryStream(clnt.DownloadData("http://server3.binarycompany.com/downloads/WindowsHosts.txt"))))
                {
                    while (!rdr.EndOfStream)
                    {
                        buf = rdr.ReadLine();
                        if (buf != "")
                            WindowsHosts.Add(buf);
                    }
                }
                #endregion

                #region Parse Parameters
                List<string> parms = new List<string>(prms);
                Configuration config = new Configuration();
                foreach (string s in parms)
                {
                    if (s.ToLower().StartsWith("-target="))
                    {
                        buf = s.Substring(8);
                        if (!ValidTargets.Contains(buf))
                        {
                            Console.WriteLine("Invalid Target Provided!");
                            return;
                        }
                        config.ToolsTargetType = buf;
                    }
                    else if (s.ToLower().StartsWith("-host="))
                    {
                        buf = s.Substring(6);
                        if (!ValidHosts.Contains(buf))
                        {
                            Console.WriteLine("Invalid Host Provided!");
                            return;
                        }
                        config.ToolsHostType = buf;
                    }
                    // Otherwise the arg must be one of the dynamically loaded ones.
                    else
                    {
                        if (!ValidLibraries.ContainsKey(s.ToLower()))
                        {
                            Console.WriteLine("Unknown Switch: '" + s.ToLower() + "'");
                            return;
                        }
                        else
                        {
                            config.AdditionalLibraries.Add(ValidLibraries[s.ToLower()]);
                        }
                    }
                }
                #endregion

                if (WindowsHosts.Contains(config.ToolsHostType))
                {
                    config.IsWindows = true;
                }
                else
                {
                    config.IsWindows = false;
                }
                SDKInstaller inst = new SDKInstaller(config);
                inst.Install();
            }
            ////}
            ////else
            ////{
            ////    Application.EnableVisualStyles();
            ////    Application.SetCompatibleTextRenderingDefault(false);
            ////    Application.Run(new Form1());
            ////}
        }
    }
}
