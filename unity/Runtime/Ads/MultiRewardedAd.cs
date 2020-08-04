using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace EE {
    public class MultiRewardedAd : ObserverManager<IRewardedAdObserver>, IRewardedAd {
        private readonly List<IRewardedAd> _items;
        private readonly ObserverHandle _handle;

        public MultiRewardedAd() {
            _items = new List<IRewardedAd>();
            _handle = new ObserverHandle();
        }

        public MultiRewardedAd AddItem(IRewardedAd item) {
            _items.Add(item);
            _handle.Bind(item).AddObserver(new IRewardedAdObserver {
                OnLoaded = () => DispatchEvent(observer => observer.OnLoaded?.Invoke()),
                OnClicked = () => DispatchEvent(observer => observer.OnClicked?.Invoke())
            });
            return this;
        }

        public void Destroy() {
            foreach (var item in _items) {
                item.Destroy();
            }
            _items.Clear();
            _handle.Clear();
        }

        public bool IsLoaded => _items.Any(item => item.IsLoaded);

        public async Task<bool> Load() {
            var result = false;
            foreach (var item in _items) {
                if (item.IsLoaded) {
                    continue;
                }
                if (await item.Load()) {
                    result = true;
                }
            }
            return result;
        }

        public async Task<IRewardedAdResult> Show() {
            var result = IRewardedAdResult.Failed;
            foreach (var item in _items) {
                if (result == IRewardedAdResult.Failed) {
                    // Only process if there isn't any successfully displayed ad.
                    result = await item.Show();
                }
                if (!item.IsLoaded) {
                    // Load in background.
                    _ = item.Load();
                }
            }
            return result;
        }
    }
}