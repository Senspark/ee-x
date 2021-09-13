using UnityEngine;

namespace EE.Internal {
    /// <summary>
    /// Tạo prefab bằng code.
    /// </summary>
    internal static class AdSensparkPrefab {
        private static bool _initialized = false;

        public static void Initialize() {
            if (_initialized)
                return;
            _initialized = true;
            CreateCanvas();
        }

        /// <summary>
        /// Khởi tạo canvas chứa ad senspark.
        /// </summary>
        private static void CreateCanvas() {
            var canvasObject = new GameObject {name = "AdSenspark"};
            var adSensparkCanvas = canvasObject.AddComponent<AdSensparkCanvas>();
            Object.DontDestroyOnLoad(canvasObject);
            ServiceLocatorSimple.AddService(adSensparkCanvas);
        }
    }
}