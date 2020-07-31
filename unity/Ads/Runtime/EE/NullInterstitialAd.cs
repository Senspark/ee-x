using System.Threading.Tasks;

namespace EE {
    public class NullInterstitialAd : ObserverManager<IInterstitialAdObserver>, IInterstitialAd {
        public void Destroy() {
        }

        public bool IsLoaded { get; } = false;

        public Task<bool> Load() {
            return Task.FromResult(false);
        }

        public Task<bool> Show() {
            return Task.FromResult(false);
        }
    }
}