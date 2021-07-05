using System.Threading.Tasks;

using UnityEngine;

namespace EE {
    public class NullFullScreenAd : NullAd, IFullScreenAd {
        public async Task<AdResult> Show() {
            await Task.Delay(1000);
            var r = Random.Range(0, 8);
            return r switch {
                0 => AdResult.Failed,
                1 => AdResult.Canceled,
                2 => AdResult.Completed,
                3 => AdResult.NotInitialized,
                4 => AdResult.NotConfigured,
                5 => AdResult.Canceled,
                6 => AdResult.NoInternet,
                7 => AdResult.NotLoaded,
                _ => AdResult.Completed
            };
        }
    }
}