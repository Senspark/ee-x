using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace EE {
    public class MultiBannerAd : ObserverManager<AdObserver>, IBannerAd, IMultiAd<IBannerAd> {
        private bool _visible;
        private (float, float) _anchor;
        private (float, float) _position;
        private readonly List<IBannerAd> _items;
        private readonly ObserverHandle _handle;
        private IBannerAd _activeItem;
        private readonly HashSet<IBannerAd> _loadedItems;

        public MultiBannerAd() {
            _items = new List<IBannerAd>();
            _handle = new ObserverHandle();
            _loadedItems = new HashSet<IBannerAd>();
        }

        public void AddItem(IBannerAd item) {
            _items.Add(item);
            item.IsVisible = _visible;
            _handle.Bind(item).AddObserver(new AdObserver {
                OnLoaded = () => {
                    _loadedItems.Add(item);
                    Invalidate();
                    DispatchEvent(observer => observer.OnLoaded?.Invoke());
                },
                OnClicked = () => DispatchEvent(observer => observer.OnClicked?.Invoke())
            });
        }

        public void Destroy() {
            foreach (var item in _items) {
                item.Destroy();
            }
            _items.Clear();
            _handle.Clear();
            _activeItem = null;
            _loadedItems.Clear();
        }

        public bool IsLoaded => _items.Any(item => item.IsLoaded);

        public async Task<bool> Load() {
            var result = false;
            foreach (var item in _items) {
                if (item == _activeItem && _visible) {
                    // Ignore displaying item.
                    continue;
                }
                if (_loadedItems.Contains(item)) {
                    // Already loaded and not displayed.
                    continue;
                }
                if (await item.Load()) {
                    result = true;
                }
            }
            return result;
        }

        public bool IsVisible {
            get => _visible;
            set {
                _visible = value;
                Invalidate();
                foreach (var item in _items) {
                    if (!_loadedItems.Contains(item)) {
                        _ = item.Load();
                    }
                }
            }
        }

        public (float, float) Anchor {
            get => _anchor;
            set {
                _anchor = value;
                foreach (var item in _items) {
                    item.Anchor = value;
                }
            }
        }

        public (float, float) Position {
            get => _position;
            set {
                _position = value;
                foreach (var item in _items) {
                    item.Position = value;
                }
            }
        }

        public (float, float) Size {
            get {
                // Combined size of all ad views.
                var width = 0.0f;
                var height = 0.0f;
                foreach (var item in _items) {
                    var (itemWidth, itemHeight) = item.Size;
                    width = Math.Max(width, itemWidth);
                    height = Math.Max(height, itemHeight);
                }
                return (width, height);
            }
            set {
                foreach (var item in _items) {
                    item.Size = value;
                }
            }
        }

        private void Invalidate() {
            var lastActiveItem = _activeItem;
            foreach (var item in _items) {
                if (_loadedItems.Contains(item)) {
                    _activeItem = item;
                    break;
                }
            }
            if (_activeItem != null) {
                _activeItem.IsVisible = _visible;
            }
            if (lastActiveItem != null && lastActiveItem != _activeItem) {
                lastActiveItem.IsVisible = false;
            }
        }
    }
}