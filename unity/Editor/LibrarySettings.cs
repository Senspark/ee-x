#if UNITY_EDITOR

using System.IO;
using System.Xml.Linq;

using UnityEditor;

using UnityEngine;

namespace EE.Editor {
    internal class LibrarySettings : ScriptableObject {
        private const string LibrarySettingsDir = "Assets/EE";
        private const string LibrarySettingsResDir = "Assets/EE/Resources";
        private const string LibrarySettingsFile = "Assets/EE/Resources/EESettings.asset";

        private const string LibrarySettingsEditorDir = "Assets/EE/Editor";
        private const string LibraryDependenciesFile = "Assets/EE/Editor/Dependencies.xml";

        private static LibrarySettings _sharedInstance;

        [SerializeField]
        private bool _injectMultiDex = false;

        [SerializeField]
        private bool _isCoreEnabled = false;

        [SerializeField]
        private bool _isAdMobEnabled = false;

        [SerializeField]
        private string _adMobAndroidAppId;

        [SerializeField]
        private string _adMobIosAppId;

        public bool InjectMultiDex {
            get => _injectMultiDex;
            set => _injectMultiDex = value;
        }

        public bool IsCoreEnabled {
            get => _isCoreEnabled;
            set => _isCoreEnabled = value;
        }

        public bool IsAdMobEnabled {
            get => _isAdMobEnabled;
            set => _isAdMobEnabled = value;
        }

        public string AdMobAndroidAppId {
            get => _adMobAndroidAppId;
            set => _adMobAndroidAppId = value;
        }

        public string AdMobIosAppId {
            get => _adMobIosAppId;
            set => _adMobIosAppId = value;
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
            UpdateDependencies();
            AssetDatabase.SaveAssets();
        }

        private void UpdateDependencies() {
            if (!AssetDatabase.IsValidFolder(LibrarySettingsEditorDir)) {
                AssetDatabase.CreateFolder(LibrarySettingsDir, "Editor");
            }
            XDocument document;
            try {
                document = XDocument.Load(LibraryDependenciesFile);
            } catch (IOException exception) {
                document = new XDocument();
            }
            var dependencies = document.Element("dependencies");
            if (dependencies == null) {
                dependencies = new XElement("dependencies");
                document.Add(dependencies);
            }
            var iosPods = dependencies.Element("iosPods");
            if (iosPods == null) {
                iosPods = new XElement("iosPods");
                dependencies.Add(iosPods);
            }
            var androidPackages = dependencies.Element("androidPackages");
            if (androidPackages == null) {
                androidPackages = new XElement("androidPackages");
                dependencies.Add(androidPackages);
            }
            iosPods.RemoveAll();
            androidPackages.RemoveAll();
            if (InjectMultiDex) {
                androidPackages.Add(
                    new XElement("androidPackage", new XAttribute("spec", "androidx.multidex:multidex:2.0.1")));
            }
            if (IsCoreEnabled) {
                iosPods.Add(new XElement("iosPod", new XAttribute("name", "ee-x/cs-core")));
                androidPackages.Add(
                    new XElement("androidPackage", new XAttribute("spec", "com.senspark.ee:core:1.0.0")));
            }
            if (IsAdMobEnabled) {
                iosPods.Add(new XElement("iosPod", new XAttribute("name", "ee-x/cs-admob")));
                androidPackages.Add(
                    new XElement("androidPackage", new XAttribute("spec", "com.senspark.ee:admob:1.0.0")));
            }
            document.Save(LibraryDependenciesFile);
        }
    }
}

#endif // UNITY_EDITOR