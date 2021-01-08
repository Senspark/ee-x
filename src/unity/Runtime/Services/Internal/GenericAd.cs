using System;
using System.Threading.Tasks;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class GenericAd : ObserverManager<AdObserver>, IAd {
        private readonly IFullScreenAd _ad;
        private bool _capped;
        private readonly int _interval;
        private readonly ObserverHandle _handle;

        public GenericAd(IFullScreenAd ad, int interval) {
            _ad = ad;
            _interval = interval;
            _capped = false;
            Utils.NoAwait(async () => await _ad.Load());
            UpdateCapping();
            _handle = new ObserverHandle();
            _handle.Bind(_ad)
                .AddObserver(new AdObserver {
                    OnLoaded = () => DispatchEvent(observer => observer.OnLoaded?.Invoke()),
                    OnClicked = () => DispatchEvent(observer => observer.OnClicked?.Invoke())
                });
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
            if (_interval > 0 && _capped) {
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
                Utils.NoAwait(async () => await _ad.Load());
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
            Utils.NoAwait(async () => await _ad.Load());
            switch (result) {
                case FullScreenAdResult.Completed:
                    UpdateCapping();
                    return AdResult.Completed;
                case FullScreenAdResult.Canceled:
                    return AdResult.Canceled;
                case FullScreenAdResult.Failed:
                    return AdResult.Failed;
            }
            Assert.IsFalse(true);
            return AdResult.Failed;
        }

        private void UpdateCapping() {
            if (_interval > 0) {
                _capped = true;
                Utils.NoAwait(async () => {
                    await Task.Delay(_interval * 1000);
                    _capped = false;
                });
            }
        }

        private static async Task<bool> TestConnection(float timeOut) {
            return await Platform.TestConnection("www.google.com", timeOut);
        }
    }
}