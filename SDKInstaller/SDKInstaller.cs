using System;
using System.Collections.Generic;

namespace SDKInstaller
{
    public sealed class SDKInstaller
    {
        public sealed class SDKPackage
        {
            public string Name = "";
            public string Target = "";
            public string Host = "";
            public DateTime Updated = DateTime.Now;
        }

        public List<SDKPackage> Packages = new List<SDKPackage>();
    }
}
