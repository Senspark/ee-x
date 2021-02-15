using System;
using System.Threading.Tasks;
using System.Xml.Xsl;

namespace EE {
    public class NullFullScreenAd : NullAd, IFullScreenAd {
        public Task<FullScreenAdResult> Show() {
            return Task.FromResult(FullScreenAdResult.Failed);
        }
    }
}