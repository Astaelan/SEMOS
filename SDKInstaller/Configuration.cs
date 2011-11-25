using System;
using System.Collections.Generic;
using System.Text;

namespace SDKInstaller
{
    public class Configuration
    {
        public bool IsWindows = true;
        public string ToolsHostType = "i386-mingw32";
        public string ToolsTargetType = "i686-elf";
        public List<string> AdditionalLibraries = new List<string>();
    }

}
