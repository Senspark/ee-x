using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace EE {
    public class MultiAdView : ObserverManager<AdObserver>, IAdView {
        private (float, float) _anchor;
        private (float, float) _position;
        private bool _visible;
        private readonly List<IAdView> _items;
        private readonly ObserverHandle _handle;
        private IAdView _activeItem;
        private readonly HashSet<IAdView> _loadedItems;

        public MultiAdView() {
            _items = new List<IAdView>();
            _handle = new ObserverHandle();
            _loadedItems = new HashSet<IAdView>();
        }

        public MultiAdView AddItem(IAdView item) {
            _items.Add(item);
            item.IsVisible = _visible;
            _handle.Bind(item).AddObserver(new AdObserver {
                OnLoaded = () => {
                    _loadedItems.Add(item);
                    DispatchEvent(observer => observer.OnLoaded?.Invoke());
                },
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

        public bool IsVisible {
            get => _visible;
            set {
                _visible = value;
                foreach (var item in _items) {
                    item.IsVisible = false;
                }
                if (_visible) {
                    if (_loadedItems.Count == 0) {
                        foreach (var item in _items) {
                            if (item != _activeItem && item.IsLoaded) {
                                _activeItem = item;
                                break;
                            }
                        }
                    } else {
                        // Prefer to displaying loaded ad.
                        foreach (var item in _items) {
                            if (_loadedItems.Contains(item)) {
                                _loadedItems.Remove(item);
                                _activeItem = item;
                                break;
                            }
                        }
                    }
                    if (_activeItem != null) {
                        _activeItem.IsVisible = true;
                    }
                } else {
                    if (_activeItem != null) {
                        // Reload the currently active ad.
                        _ = _activeItem.Load();
                    }
                }
            }
        }
    }
}