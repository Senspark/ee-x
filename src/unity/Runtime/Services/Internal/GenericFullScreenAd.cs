using System;
using System.Threading.Tasks;

namespace EE.Internal {
    internal class GenericFullScreenAd : ObserverManager<AdObserver>, IFullScreenAd {
        private readonly IFullScreenAd _ad;
        private readonly ICapper _displayCapper;
        private readonly ICapper _loadCapper;
        private readonly IRetrier _loadRetrier;
        private readonly ObserverHandle _handle;

        public GenericFullScreenAd(
            IFullScreenAd ad,
            ICapper displayCapper,
            ICapper loadCapper,
            IRetrier loadRetrier) {
            _ad = ad;
            _displayCapper = displayCapper;
            _loadCapper = loadCapper;
            _loadRetrier = loadRetrier;
            _handle = new ObserverHandle();
            _handle.Bind(_ad)
                .AddObserver(new AdObserver {
                    OnLoaded = () => DispatchEvent(observer =>
                        observer.OnLoaded?.Invoke()),

                    OnLoadResult = result => DispatchEvent(observer =>
                        observer.OnLoadResult?.Invoke(result)),

                    OnClicked = () => DispatchEvent(observer =>
                        observer.OnClicked?.Invoke())
                });
        }

        public void Destroy() {
            _ad.Destroy();
            _handle.Clear();
            _loadRetrier.Stop();
        }

        public bool IsLoaded => _ad.IsLoaded;

        public async Task<bool> Load() {
            var result = await LoadInternal();
            if (result) {
                _loadRetrier.Stop();
            } else {
                Utils.NoAwait(async () => { //
                    await _loadRetrier.Process(async () => await LoadInternal());
                });
            }
            return result;
        }

        private async Task<bool> LoadInternal() {
            if (_loadCapper.IsCapped) {
                return false;
            }
            _loadCapper.Cap();
            return await _ad.Load();
        }

        public async Task<AdResult> Show() {
            if (_displayCapper.IsCapped) {
                return AdResult.Capped;
            }
            if (_ad.IsLoaded) {
                var hasInternet = await TestConnection(0.2f);
                if (hasInternet) {
                    // OK.
                } else {
                    return AdResult.NoInternet;
                }
            } else {
                Utils.NoAwait(async () => await Load());
                var hasInternet = false;
                await Task.WhenAll(Task.Delay(100),
                    ((Func<Task>) (async () => { //
                        hasInternet = await TestConnection(0.2f);
                    }))());
                if (hasInternet) {
                    // OK.
                } else {
                    return AdResult.NoInternet;
                }
            }
            if (_ad.IsLoaded) {
                // OK.
            } else {
                return AdResult.NotLoaded;
            }
            var result = await _ad.Show();
            Utils.NoAwait(async () => await Load());
            if (result == AdResult.Completed) {
                _displayCapper.Cap();
            }
            return result;
        }

        private static async Task<bool> TestConnection(float timeOut) {
            return await Platform.TestConnection("www.google.com", timeOut);
        }
    }
}