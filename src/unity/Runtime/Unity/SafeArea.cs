using UnityEngine;

namespace EE {
    public class SafeArea : MonoBehaviour {
        /// <summary>
        /// Conform to screen safe area on X-axis (default true, disable to ignore).
        /// </summary>
        [SerializeField]
        private bool _conformX = true;

        /// <summary>
        /// Conform to screen safe area on Y-axis (default true, disable to ignore).
        /// </summary>
        [SerializeField]
        private bool _conformY = true;

        [SerializeField]
        private bool _logging = false;

        private RectTransform _panel;
        private ScreenOrientation? _orientation;
        private int _screenWidth;
        private int _screenHeight;
        private SafeInset _inset;
        private Rect _safeArea;

        private void Awake() {
            _panel = GetComponent<RectTransform>();
            if (_panel == null) {
                Debug.LogError($"Cannot apply safe area - no RectTransform found on {name}");
                Destroy(gameObject);
            }
            Refresh();
        }

        private void Update() {
            Refresh();
        }

        private void Refresh() {
            var orientation = Screen.orientation;
            var screenWidth = Screen.width;
            var screenHeight = Screen.height;
            if (orientation == _orientation &&
                screenWidth == _screenWidth &&
                screenHeight == _screenHeight) {
                // Nothing changes.
                return;
            }
            _orientation = orientation;
            _screenWidth = screenWidth;
            _screenHeight = screenHeight;
            var inset = Platform.GetSafeInset();
            var rect = new Rect(
                inset.left,
                inset.bottom,
                screenWidth - inset.left - inset.right,
                screenHeight - inset.bottom - inset.top
            );
            _inset = inset;
            _safeArea = rect;
            ApplySafeArea(rect);
        }

        private void ApplySafeArea(Rect rect) {
            // Ignore x-axis?
            if (!_conformX) {
                rect.x = 0;
                rect.width = _screenWidth;
            }

            // Ignore y-axis?
            if (!_conformY) {
                rect.y = 0;
                rect.height = _screenHeight;
            }

            // Check for invalid screen startup state on some Samsung devices (see below).
            if (_screenWidth > 0 && _screenHeight > 0) {
                // Convert safe area rectangle from absolute pixels to normalised anchor coordinates
                var anchorMin = rect.position;
                var anchorMax = rect.position + rect.size;
                anchorMin.x /= _screenWidth;
                anchorMin.y /= _screenHeight;
                anchorMax.x /= _screenWidth;
                anchorMax.y /= _screenHeight;

                // Fix for some Samsung devices (e.g. Note 10+, A71, S20) where Refresh gets called twice and the first time returns NaN anchor coordinates
                // See https://forum.unity.com/threads/569236/page-2#post-6199352
                if (anchorMin.x >= 0 && anchorMin.y >= 0 && anchorMax.x >= 0 && anchorMax.y >= 0) {
                    _panel.anchorMin = anchorMin;
                    _panel.anchorMax = anchorMax;
                }
            }

            if (_logging) {
                Debug.Log(
                    $"New safe area applied to {name}: x={rect.x}, y={rect.y}, w={rect.width}, h={rect.height} on full extents w={_screenWidth}, h={_screenHeight}");
            }
        }
    }
}