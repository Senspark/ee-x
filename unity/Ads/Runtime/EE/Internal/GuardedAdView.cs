using System.Drawing;
using System.Threading.Tasks;

namespace EE.Internal {
    public class GuardedAdView
        : ObserverManager<IAdViewObserver>, IAdView {
        private readonly IAdView _ad;
        private bool _loading;
        private readonly ObserverHandle _handle;

        public GuardedAdView(IAdView ad) {
            _ad = ad;
            _handle = new ObserverHandle();
            _handle.Bind(ad).AddObserver(new IAdViewObserver {
                OnLoaded = () => {
                    IsLoaded = true;
                    DispatchEvent(observer => observer.OnLoaded?.Invoke());
                },
                OnClicked = () => DispatchEvent(observer => observer.OnClicked?.Invoke())
            });
        }

        public void Destroy() {
            _ad.Destroy();
            _handle.Clear();
        }

        public bool IsLoaded { get; private set; }

        public async Task<bool> Load() {
            if (IsLoaded) {
                return true;
            }
            if (_loading) {
                // Waiting.
                return await _ad.Load();
            }
            _loading = true;
            var result = await _ad.Load();
            _loading = false;
            return result;
        }

        public PointF Anchor {
            get => _ad.Anchor;
            set => _ad.Anchor = value;
        }

        public PointF Position {
            get => _ad.Position;
            set => _ad.Position = value;
        }

        public SizeF Size {
            get => _ad.Size;
            set => _ad.Size = value;
        }

        public bool IsVisible {
            get => _ad.IsVisible;
            set => _ad.IsVisible = value;
        }
    }
}