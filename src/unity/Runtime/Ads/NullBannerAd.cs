namespace EE {
    public class NullBannerAd : NullAd, IBannerAd {
        private (float, float) _anchor;
        private (float, float) _position;
        private (float, float) _size;

        public bool IsVisible { get; set; }

        public (float, float) Anchor {
            get => _anchor;
            set {
                _position.Item1 -= (value.Item1 - _anchor.Item1) * _size.Item1;
                _position.Item2 -= (value.Item2 - _anchor.Item2) * _size.Item2;
                _anchor = value;
            }
        }

        public (float, float) Position {
            get => (
                _position.Item1 + _anchor.Item1 * _size.Item1,
                _position.Item2 + _anchor.Item2 * _size.Item2);
            set => _position = (
                value.Item1 - _anchor.Item1 * _size.Item1,
                value.Item2 - _anchor.Item2 * _size.Item2);
        }

        public (float, float) Size {
            get => _size;
            set {
                _position.Item1 -= (value.Item1 - _size.Item1) * _anchor.Item1;
                _position.Item2 -= (value.Item2 - _size.Item2) * _anchor.Item2;
                _size = value;
            }
        }
    }
}