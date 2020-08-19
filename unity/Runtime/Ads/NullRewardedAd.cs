using System;
using System.Threading.Tasks;

namespace EE {
    public class NullRewardedAd : ObserverManager<IRewardedAdObserver>, IRewardedAd {
        public void Destroy() {
            throw new NotImplementedException();
        }

        public bool IsLoaded { get; } = false;

        public Task<bool> Load() {
            return Task.FromResult(false);
        }

        public Task<IRewardedAdResult> Show() {
            return Task.FromResult(IRewardedAdResult.Failed);
        }
    }
}