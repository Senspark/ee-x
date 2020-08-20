#if UNITY_EDITOR

using System.Collections.Generic;
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
        private bool _isMultiDexEnabled = false;

        [SerializeField]
        private bool _isCoreEnabled = true;

        [SerializeField]
        private bool _isAdMobEnabled = false;

        [SerializeField]
        private bool _isAdMobMediationEnabled = false;

        [SerializeField]
        private string _adMobAndroidAppId;

        [SerializeField]
        private string _adMobIosAppId;

        [SerializeField]
        private bool _isAdjustEnabled = false;

        [SerializeField]
        private bool _isIronSourceEnabled = false;

        [SerializeField]
        private bool _isIronSourceMediationEnabled = false;

        public bool IsMultiDexEnabled {
            get => _isMultiDexEnabled;
            set => _isMultiDexEnabled = value;
        }

        public bool IsCoreEnabled {
            get => _isCoreEnabled;
            set => _isCoreEnabled = value;
        }

        public bool IsAdMobEnabled {
            get => _isAdMobEnabled;
            set => _isAdMobEnabled = value;
        }

        public bool IsAdMobMediationEnabled {
            get => _isAdMobMediationEnabled;
            set => _isAdMobMediationEnabled = value;
        }

        public string AdMobAndroidAppId {
            get => _adMobAndroidAppId;
            set => _adMobAndroidAppId = value;
        }

        public string AdMobIosAppId {
            get => _adMobIosAppId;
            set => _adMobIosAppId = value;
        }

        public bool IsAdjustEnabled {
            get => _isAdjustEnabled;
            set => _isAdjustEnabled = value;
        }

        public bool IsIronSourceEnabled {
            get => _isIronSourceEnabled;
            set => _isIronSourceEnabled = value;
        }

        public bool IsIronSourceMediationEnabled {
            get => _isIronSourceMediationEnabled;
            set => _isIronSourceMediationEnabled = value;
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
            var androidLibraries = new List<string>();
            var iosLibraries = new List<string>();
            if (IsMultiDexEnabled) {
                androidLibraries.Add("androidx.multidex:multidex:2.0.1");
            }
            if (IsCoreEnabled) {
                androidLibraries.Add("com.senspark.ee:core:1.0.0");
                iosLibraries.Add("ee-x/cs-core");
            }
            if (IsAdMobEnabled) {
                if (IsAdMobMediationEnabled) {
                    androidLibraries.Add("com.senspark.ee:admob-mediation:1.0.0");
                    iosLibraries.Add("ee-x/cs-admob-mediation");
                } else {
                    androidLibraries.Add("com.senspark.ee:admob:1.0.0");
                    iosLibraries.Add("ee-x/cs-admob");
                }
            }
            if (IsAdjustEnabled) {
                androidLibraries.Add("com.senspark.ee:adjust:1.0.0");
                iosLibraries.Add("ee-x/cs-adjust");
            }
            if (IsIronSourceEnabled) {
                if (IsIronSourceMediationEnabled) {
                    androidLibraries.Add("com.senspark.ee:iron-source-mediation:1.0.0");
                    iosLibraries.Add("ee-x/cs-iron-source-mediation");
                } else {
                    androidLibraries.Add("com.senspark.ee:iron-source:1.0.0");
                    iosLibraries.Add("ee-x/cs-iron-source");
                }
            }
            foreach (var library in androidLibraries) {
                androidPackages.Add(new XElement("androidPackage", new XAttribute("spec", library)));
            }
            foreach (var library in iosLibraries) {
                iosPods.Add(new XElement("iosPod", new XAttribute("name", library)));
            }
            document.Save(LibraryDependenciesFile);
        }
    }
}

#endif // UNITY_EDITOR