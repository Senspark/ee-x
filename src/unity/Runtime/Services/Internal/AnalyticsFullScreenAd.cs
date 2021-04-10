using System.Threading.Tasks;

namespace EE.Internal {
    internal class AnalyticsFullScreenAd : ObserverManager<AdObserver>, IFullScreenAd {
        private readonly IFullScreenAd _ad;
        private readonly IAnalyticsManager _manager;
        private readonly AdFormat _format;
        private readonly ObserverHandle _handle;

        public AnalyticsFullScreenAd(
            IFullScreenAd ad,
            IAnalyticsManager manager,
            AdFormat format) {
            _ad = ad;
            _manager = manager;
            _format = format;
            _handle = new ObserverHandle();
        }

        public void Destroy() {
            _ad.Destroy();
            _handle.Clear();
        }

        public bool IsLoaded => _ad.IsLoaded;

        public async Task<bool> Load() {
            return await _ad.Load();
        }

        public async Task<AdResult> Show() {
            var result = await _ad.Show();
            _manager.LogEvent(new AdEvent {
                Format = _format,
                Result = result
            });
            return result;
        }
    }
}