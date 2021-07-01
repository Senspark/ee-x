using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace EE {
    public class MultiFullScreenAd : ObserverManager<AdObserver>, IFullScreenAd, IMultiAd<IFullScreenAd> {
        private readonly List<IFullScreenAd> _items;
        private readonly ObserverHandle _handle;

        public MultiFullScreenAd() {
            _items = new List<IFullScreenAd>();
            _handle = new ObserverHandle();
        }

        public void AddItem(IFullScreenAd item) {
            _items.Add(item);
            _handle.Bind(item).AddObserver(new AdObserver {
                OnLoaded = () => DispatchEvent(observer =>
                    observer.OnLoaded?.Invoke()),

                OnLoadResult = result => DispatchEvent(observer =>
                    observer.OnLoadResult?.Invoke(result)),

                OnClicked = () => DispatchEvent(observer =>
                    observer.OnClicked?.Invoke())
            });
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
            var tasks = new HashSet<Task<bool>>(_items.Select(async item => {
                if (item.IsLoaded) {
                    return true;
                }
                return await item.Load();
            }));
            while (tasks.Any()) {
                var next = await Task.WhenAny(tasks);
                if (next.Result) {
                    return true;
                }
                tasks.Remove(next);
            }
            return false;
        }

        public async Task<AdResult> Show() {
            var result = AdResult.Failed;
            foreach (var item in _items) {
                if (result == AdResult.Failed) {
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