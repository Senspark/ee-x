using System.Threading.Tasks;

namespace EE {
    public class NullAd : ObserverManager<AdObserver>, IAd {
        public void Destroy() {
        }

        public bool IsLoaded { get; } = false;

        public Task<bool> Load() {
            return Task.FromResult(false);
        }
    }
}