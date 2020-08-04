#if UNITY_EDITOR

using UnityEditor;

using UnityEngine;

namespace EE.Editor {
    internal class LibrarySettings : ScriptableObject {
        private const string LibrarySettingsDir = "Assets/EE";
        private const string LibrarySettingsResDir = "Assets/EE/Resources";
        private const string LibrarySettingsFile = "Assets/EE/Resources/EESettings.asset";

        private static LibrarySettings _sharedInstance;

        [SerializeField]
        private bool _isCoreEnabled = false;

        [SerializeField]
        private bool _isAdMobEnabled = false;

        public bool IsCoreEnabled {
            get => _isCoreEnabled;
            set => _isCoreEnabled = value;
        }

        public bool IsAdMobEnabled {
            get => _isAdMobEnabled;
            set => _isAdMobEnabled = value;
        }

        public static LibrarySettings Instance {
            get {
                if (_sharedInstance == null) {
                    if (!AssetDatabase.IsValidFolder(LibrarySettingsDir)) {
                        AssetDatabase.CreateFolder("Assets", "EE");
                    }
                    if (!AssetDatabase.IsValidFolder(LibrarySettingsResDir)) {
                        AssetDatabase.CreateFolder(LibrarySettingsDir, "Resources");
                    }
                    _sharedInstance = AssetDatabase.LoadAssetAtPath<LibrarySettings>(LibrarySettingsFile);
                    if (_sharedInstance == null) {
                        _sharedInstance = CreateInstance<LibrarySettings>();
                        AssetDatabase.CreateAsset(_sharedInstance, LibrarySettingsFile);
                    }
                }
                return _sharedInstance;
            }
        }

        public void WriteSettingsToFile() {
            AssetDatabase.SaveAssets();
        }
    }
}

#endif // UNITY_EDITOR