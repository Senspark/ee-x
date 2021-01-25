using System;

using UnityEngine;

namespace EE.Internal {
    internal class BannerAdHelper {
        private readonly IMessageBridge _bridge;
        private readonly MessageHelper _helper;
        private (float, float) _anchor;
        private (float, float) _position;
        private (float, float) _size;
        private bool _visible;

        public BannerAdHelper(IMessageBridge bridge, MessageHelper helper, (int, int) size) {
            _bridge = bridge;
            _helper = helper;
            _size = size;
        }

        public bool IsLoaded {
            get {
                var response = _bridge.Call(_helper.IsLoaded);
                return Utils.ToBool(response);
            }
        }

        public void Load() {
            _bridge.Call(_helper.Load);
        }

        [Serializable]
        private struct SetPositionTopLeftRequest {
            public int x;
            public int y;
        }

        private void SetPositionTopLeft((float, float) position) {
            var request = new SetPositionTopLeftRequest {
                x = (int) position.Item1,
                y = (int) position.Item2,
            };
            _bridge.Call(_helper.SetPosition, JsonUtility.ToJson(request));
            _position = position;
        }

        public (float, float) Anchor {
            get => _anchor;
            set {
                SetPositionTopLeft((
                    _position.Item1 - (value.Item1 - _anchor.Item1) * _size.Item1,
                    _position.Item2 - (value.Item2 - _anchor.Item2) * _size.Item2));
                _anchor = value;
            }
        }

        public (float, float) Position {
            get => (
                _position.Item1 + _anchor.Item1 * _size.Item1,
                _position.Item2 + _anchor.Item2 * _size.Item2);
            set => SetPositionTopLeft((
                value.Item1 - _anchor.Item1 * _size.Item1,
                value.Item2 - _anchor.Item2 * _size.Item2));
        }

        [Serializable]
        private struct SetSizeRequest {
            public int width;
            public int height;
        }

        public (float, float) Size {
            get => _size;
            set {
                SetPositionTopLeft((
                    _position.Item1 - (value.Item1 - _size.Item1) * _anchor.Item1,
                    _position.Item2 - (value.Item2 - _size.Item2) * _anchor.Item2));
                var request = new SetSizeRequest {
                    width = (int) value.Item1,
                    height = (int) value.Item2
                };
                _bridge.Call(_helper.SetSize, JsonUtility.ToJson(request));
                _size = value;
            }
        }

        public bool IsVisible {
            get => _visible;
            set {
                _bridge.Call(_helper.SetVisible, Utils.ToString(value));
                _visible = value;
            }
        }
    }
}