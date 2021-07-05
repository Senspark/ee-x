using System;
using System.Threading.Tasks;

using Random = UnityEngine.Random;

namespace EE {
    public class NullAd : ObserverManager<AdObserver>, IAd {
        public void Destroy() {
        }

        public bool IsLoaded => false;

        public async Task<bool> Load() {
            await Task.Delay(1000);
            var r = Random.Range(0, 2);
            return r switch {
                0 => true,
                1 => false,
                _ => throw new ArgumentOutOfRangeException()
            };
        }
    }
}