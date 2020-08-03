using System;
using System.Drawing;

using UnityEngine;

namespace EE.Internal {
    internal class AdViewHelper {
        private readonly IMessageBridge _bridge;
        private readonly MessageHelper _helper;
        private PointF _anchor;
        private PointF _position;
        private SizeF _size;
        private bool _visible;

        public AdViewHelper(IMessageBridge bridge, MessageHelper helper, Size size) {
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

        private void SetPositionTopLeft(PointF position) {
            var request = new SetPositionTopLeftRequest {
                x = (int) position.X,
                y = (int) position.Y
            };
            _bridge.Call(_helper.SetPosition, JsonUtility.ToJson(request));
            _position = position;
        }

        public PointF Anchor {
            get => _anchor;
            set {
                SetPositionTopLeft(new PointF(
                    _position.X - (value.X - _anchor.X) * _size.Width,
                    _position.Y - (value.Y - _anchor.Y) * _size.Height));
                _anchor = value;
            }
        }

        public PointF Position {
            get => new PointF(
                _position.X + _anchor.X * _size.Width,
                _position.Y + _anchor.Y * _size.Height);
            set => SetPositionTopLeft(new PointF(
                value.X - _anchor.X * _size.Width,
                value.Y - _anchor.Y * _size.Height));
        }

        [Serializable]
        private struct SetSizeRequest {
            public int width;
            public int height;
        }

        public SizeF Size {
            get => _size;
            set {
                SetPositionTopLeft(new PointF(
                    _position.X - (value.Width - _size.Width) * _anchor.X,
                    _position.Y - (value.Height - _size.Height) * _anchor.Y));
                var request = new SetSizeRequest {
                    width = (int) value.Width,
                    height = (int) value.Height
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