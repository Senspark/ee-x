using System.Threading.Tasks;

namespace EE.Internal {
    internal class LazyAdView : ObserverManager<AdObserver>, IAdView {
        private IAdView _ad;
        private bool _visible;
        private (float, float) _anchor;
        private (float, float) _position;
        private (float, float)? _size;
        private readonly ObserverHandle _handle;

        public LazyAdView() {
            _visible = false;
            _anchor = (0, 0);
            _position = (0, 0);
            _size = null;
            _handle = new ObserverHandle();
        }

        public IAdView Ad {
            get => _ad;
            set {
                _handle.Clear();
                _handle.Bind(value)
                    .AddObserver(new AdObserver {
                        OnLoaded = () => {
                            if (_visible) {
                                _ad.IsVisible = false;
                                _ad.IsVisible = true;
                            }
                            DispatchEvent(observer => observer.OnLoaded?.Invoke());
                        },
                        OnClicked = () => DispatchEvent(observer => observer.OnClicked?.Invoke())
                    });
                _ad = value;
                _ad.IsVisible = _visible;
                _ad.Anchor = _anchor;
                _ad.Position = _position;
                if (_size.HasValue) {
                    _ad.Size = _size.Value;
                }
                Utils.NoAwait(async () => await _ad.Load());
            }
        }

        public void Destroy() {
            _ad?.Destroy();
            _handle.Clear();
        }

        public bool IsLoaded => _ad?.IsLoaded ?? false;

        public async Task<bool> Load() {
            if (_ad == null) {
                return false;
            }
            return await _ad.Load();
        }

        public (float, float) Anchor {
            get => _anchor;
            set {
                _anchor = value;
                if (_ad != null) {
                    _ad.Anchor = value;
                }
            }
        }

        public (float, float) Position {
            get => _position;
            set {
                _position = value;
                if (_ad != null) {
                    _ad.Position = value;
                }
            }
        }

        public (float, float) Size {
            get {
                if (_size.HasValue) {
                    return _size.Value;
                }
                return _ad?.Size ?? (0, 0);
            }
            set {
                _size = value;
                if (_ad != null) {
                    _ad.Size = value;
                }
            }
        }

        public bool IsVisible {
            get => _visible;
            set {
                _visible = value;
                if (_ad != null) {
                    _ad.IsVisible = value;
                }
            }
        }
    }
}