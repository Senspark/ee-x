using System;
using System.Threading.Tasks;

namespace EE.Internal {
    internal class GenericFullScreenAd : GenericAd, IFullScreenAd {
        private readonly IFullScreenAd _ad;
        private readonly ICapper _displayCapper;

        public GenericFullScreenAd(
            IFullScreenAd ad,
            ICapper displayCapper,
            ICapper loadCapper,
            IRetrier loadRetrier) : base(ad, loadCapper, loadRetrier) {
            _ad = ad;
            _displayCapper = displayCapper;
        }

        public async Task<AdResult> Show() {
            if (_displayCapper.IsCapped) {
                return AdResult.Capped;
            }
            if (_ad.IsLoaded) {
                var hasInternet = await TestConnection(0.5f);
                if (hasInternet) {
                    // OK.
                } else {
                    return AdResult.NoInternet;
                }
            } else {
                Utils.NoAwait(async () => await Load());
                var hasInternet = false;
                await Task.WhenAll(Task.Delay(300),
                    ((Func<Task>) (async () => { //
                        hasInternet = await TestConnection(0.5f);
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