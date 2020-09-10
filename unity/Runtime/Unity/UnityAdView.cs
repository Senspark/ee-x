using System.Threading.Tasks;

namespace EE {
    public class UnityAdView
        : ObserverManager<IAdViewObserver>, IAdView {
        private readonly IAdView _ad;
        private readonly ObserverHandle _handle;

        public UnityAdView(IAdView ad) {
            _ad = ad;
            _handle = new ObserverHandle();
            _handle.Bind(ad).AddObserver(new IAdViewObserver {
                OnLoaded = () => DispatchEvent(observer => observer.OnLoaded?.Invoke()),
                OnClicked = () => DispatchEvent(observer => observer.OnClicked?.Invoke())
            });
        }

        public void Destroy() {
            _ad.Destroy();
            _handle.Clear();
        }

        public bool IsLoaded => _ad.IsLoaded;

        public Task<bool> Load() {
            return _ad.Load();
        }

        public (float, float) Anchor {
            get {
                var (x, y) = _ad.Anchor;
                return (x, 1 - y);
            }
            set {
                var (x, y) = value;
                _ad.Anchor = (x, 1 - y);
            }
        }

        public (float, float) Position {
            get {
                var (x, y) = _ad.Position;
                return (x, 1 - y);
            }
            set {
                var (x, y) = value;
                _ad.Position = (x, 1 - y);
            }
        }

        public (float, float) Size {
            get => _ad.Size;
            set => _ad.Size = value;
        }

        public bool IsVisible {
            get => _ad.IsVisible;
            set => _ad.IsVisible = value;
        }
    }
}