using System.Drawing;
using System.Threading.Tasks;

namespace EE {
    public class NullAdView : ObserverManager<IAdViewObserver>, IAdView {
        private PointF _anchor;
        private PointF _position;
        private SizeF _size;

        public void Destroy() {
        }

        public bool IsLoaded { get; private set; }

        public async Task<bool> Load() {
            await Task.Delay(1000);
            IsLoaded = true;
            return true;
        }

        public PointF Anchor {
            get => _anchor;
            set {
                _position.X -= (value.X - _anchor.X) * _size.Width;
                _position.Y -= (value.Y - _anchor.Y) * _size.Height;
                _anchor = value;
            }
        }

        public PointF Position {
            get => new PointF(
                _position.X + _anchor.X * _size.Width,
                _position.Y + _anchor.Y * _size.Height);
            set => _position = new PointF(
                value.X - _anchor.X * _size.Width,
                value.Y - _anchor.Y * _size.Height);
        }

        public SizeF Size {
            get => _size;
            set {
                _position.X -= (value.Width - _size.Width) * _anchor.X;
                _position.Y -= (value.Height - _size.Height) * _anchor.Y;
                _size = value;
            }
        }

        public bool IsVisible { get; set; }
    }
}