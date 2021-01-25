using System;
using System.Threading.Tasks;
using System.Xml.Xsl;

namespace EE {
    public class NullFullScreenAd : ObserverManager<AdObserver>, IFullScreenAd {
        public void Destroy() {
        }

        public bool IsLoaded { get; } = false;

        public Task<bool> Load() {
            return Task.FromResult(false);
        }

        public Task<FullScreenAdResult> Show() {
            return Task.FromResult(FullScreenAdResult.Failed);
        }
    }
}