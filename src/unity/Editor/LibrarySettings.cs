using System.Collections.Generic;
using System.IO;
using System.Xml.Linq;

using UnityEditor;

using UnityEngine;

namespace EE.Editor {
    internal enum AdNetwork {
        AdColony = 0,
        AdMob = 1,
        AppLovin = 2,
        Facebook = 3,
        InMobi = 4,
        IronSource = 5,
        Pangle = 9,
        Tapjoy = 6,
        Unity = 7,
        Vungle = 8,
    }

    internal class LibrarySettings : ScriptableObject {
        private const string LibraryVersion = "2.10.3";

        private const string LibrarySettingsDir = "Assets/EE";
        private const string LibrarySettingsResDir = "Assets/EE/Resources";
        private const string LibrarySettingsFile = "Assets/EE/Resources/EESettings.asset";

        private const string LibrarySettingsEditorDir = "Assets/EE/Editor";
        private const string LibraryDependenciesFile = "Assets/EE/Editor/Dependencies.xml";

        private static LibrarySettings _sharedInstance;

        [SerializeField]
        private bool _isDeveloperModeEnabled = false;

        [SerializeField]
        private string _libraryPath = "ee-x";

        [SerializeField]
        private bool _isMultiDexEnabled = false;

        [SerializeField]
        private bool _isFixPackageOptionsEnabled = false;

        [SerializeField]
        private bool _overwriteNSUserTrackingUsageDescription = false;

        [SerializeField]
        private bool _isCoreEnabled = false;

        [SerializeField]
        private bool _isAdjustEnabled = false;

        [SerializeField]
        private bool _isAppsFlyerEnabled = false;

        [SerializeField]
        private bool _isAdColonyEnabled = false;

        [SerializeField]
        private bool _isAdMobEnabled = false;

        [SerializeField]
        private int _adMobMediationFlags = 0;

        [SerializeField]
        private string _adMobAndroidAppId;

        [SerializeField]
        private string _adMobIosAppId;

        [SerializeField]
        private bool _isFacebookAdsEnabled = false;

        [SerializeField]
        private bool _isIronSourceEnabled = false;

        [SerializeField]
        private int _ironSourceMediationFlag = 0;

        [SerializeField]
        private bool _isUnityAdsEnabled = false;

        public bool IsDeveloperModeEnabled {
            get => _isDeveloperModeEnabled;
            set => _isDeveloperModeEnabled = value;
        }

        public string LibraryPath {
            get => _libraryPath;
            set => _libraryPath = value;
        }

        public bool IsMultiDexEnabled {
            get => _isMultiDexEnabled;
            set => _isMultiDexEnabled = value;
        }

        public bool IsFixPackageOptionsEnabled {
            get => _isFixPackageOptionsEnabled;
            set => _isFixPackageOptionsEnabled = value;
        }

        public bool OverwriteNSUserTrackingUsageDescription {
            get => _overwriteNSUserTrackingUsageDescription;
            set => _overwriteNSUserTrackingUsageDescription = value;
        }

        public bool IsCoreEnabled {
            get => _isCoreEnabled;
            set => _isCoreEnabled = value;
        }

        public bool IsAdjustEnabled {
            get => _isAdjustEnabled;
            set => _isAdjustEnabled = value;
        }

        public bool IsAppsFlyerEnabled {
            get => _isAppsFlyerEnabled;
            set => _isAppsFlyerEnabled = value;
        }

        public bool IsAdColonyEnabled {
            get => _isAdColonyEnabled;
            set => _isAdColonyEnabled = value;
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

        public bool IsFacebookAdsEnabled {
            get => _isFacebookAdsEnabled;
            set => _isFacebookAdsEnabled = value;
        }

        public bool IsIronSourceEnabled {
            get => _isIronSourceEnabled;
            set => _isIronSourceEnabled = value;
        }

        public bool IsUnityAdsEnabled {
            get => _isUnityAdsEnabled;
            set => _isUnityAdsEnabled = value;
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

        public bool IsAdMobMediationEnabled(AdNetwork network) {
            return ((_adMobMediationFlags >> (int) network) & 1) == 1;
        }

        public void SetAdMobMediationEnabled(AdNetwork network, bool enabled) {
            if (enabled) {
                _adMobMediationFlags |= 1 << (int) network;
            } else {
                _adMobMediationFlags &= ~(1 << (int) network);
            }
        }

        public bool IsIronSourceMediationEnabled(AdNetwork network) {
            return ((_ironSourceMediationFlag >> (int) network) & 1) == 1;
        }

        public void SetIronSourceMediationEnabled(AdNetwork network, bool enabled) {
            if (enabled) {
                _ironSourceMediationFlag |= 1 << (int) network;
            } else {
                _ironSourceMediationFlag &= ~(1 << (int) network);
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
            var androidLibraries = new HashSet<string>();
            var fastAndroidLibraries = new HashSet<string>();
            var androidRepositories = new HashSet<string>();
            var iosLibraries = new HashSet<string>();
            if (IsMultiDexEnabled) {
                androidLibraries.Add("androidx.multidex:multidex:[2.0.1]");
            }
            if (IsCoreEnabled) {
                androidLibraries.Add($"com.senspark.ee:core:[{LibraryVersion}]");
                iosLibraries.Add("ee-x/core");
                if (IsAdMobMediationEnabled(AdNetwork.IronSource) || IsIronSourceEnabled) {
                    androidRepositories.Add("https://android-sdk.is.com");
                }
                if (IsAdMobMediationEnabled(AdNetwork.Tapjoy) || IsIronSourceMediationEnabled(AdNetwork.Tapjoy)) {
                    androidRepositories.Add("https://sdk.tapjoy.com");
                }
                if (IsAdMobMediationEnabled(AdNetwork.Pangle) || IsIronSourceMediationEnabled(AdNetwork.Pangle)) {
                    androidRepositories.Add("https://artifact.bytedance.com/repository/pangle/");
                }
                if (IsAdjustEnabled) {
                    androidLibraries.Add($"com.senspark.ee:adjust:[{LibraryVersion}]");
                    iosLibraries.Add("ee-x/adjust");
                    iosPods.Add(new XElement("iosPod", new XAttribute("name", "Adjust"),
                        new XAttribute("modular_headers", true)));
                }
                if (IsAppsFlyerEnabled) {
                    androidLibraries.Add($"com.senspark.ee:apps-flyer:[{LibraryVersion}]");
                    iosLibraries.Add("ee-x/apps-flyer");
                }
                if (IsAdColonyEnabled) {
                    androidLibraries.Add($"com.senspark.ee:ad-colony:[{LibraryVersion}]");
                    iosLibraries.Add("ee-x/ad-colony");
                }
                if (IsAdMobEnabled) {
                    androidLibraries.Add($"com.senspark.ee:ad-mob:[{LibraryVersion}]");
                    iosLibraries.Add("ee-x/ad-mob");
                    var networks = new Dictionary<AdNetwork, string> {
                        { AdNetwork.AdColony, "ad-colony" },
                        { AdNetwork.AppLovin, "app-lovin" },
                        { AdNetwork.Facebook, "facebook" },
                        { AdNetwork.InMobi, "inmobi" },
                        { AdNetwork.IronSource, "iron-source" },
                        { AdNetwork.Pangle, "pangle" },
                        { AdNetwork.Tapjoy, "tapjoy" },
                        { AdNetwork.Unity, "unity" },
                        { AdNetwork.Vungle, "vungle" },
                    };
                    foreach (var (network, path) in networks) {
                        if (IsAdMobMediationEnabled(network)) {
                            androidLibraries.Add($"com.senspark.ee:ad-mob-mediation-{path}:[{LibraryVersion}]");
                            iosLibraries.Add($"ee-x/ad-mob-mediation-{path}");
                        }
                    }
                }
                if (IsFacebookAdsEnabled) {
                    androidLibraries.Add($"com.senspark.ee:facebook-ads:[{LibraryVersion}]");
                    iosLibraries.Add("ee-x/facebook-ads");
                }
                if (IsIronSourceEnabled) {
                    androidLibraries.Add($"com.senspark.ee:iron-source:[{LibraryVersion}]");
                    iosLibraries.Add("ee-x/iron-source");
                    var networks = new Dictionary<AdNetwork, string> {
                        { AdNetwork.AdColony, "ad-colony" },
                        { AdNetwork.AdMob, "ad-mob" },
                        { AdNetwork.AppLovin, "app-lovin" },
                        { AdNetwork.Facebook, "facebook" },
                        { AdNetwork.InMobi, "inmobi" },
                        { AdNetwork.Pangle, "pangle" },
                        { AdNetwork.Tapjoy, "tapjoy" },
                        { AdNetwork.Unity, "unity" },
                        { AdNetwork.Vungle, "vungle" },
                    };
                    foreach (var (network, path) in networks) {
                        if (IsIronSourceMediationEnabled(network)) {
                            androidLibraries.Add($"com.senspark.ee:iron-source-mediation-{path}:[{LibraryVersion}]");
                            iosLibraries.Add($"ee-x/iron-source-mediation-{path}");
                        }
                    }
                }
                if (IsUnityAdsEnabled) {
                    androidLibraries.Add($"com.senspark.ee:unity-ads:[{LibraryVersion}]");
                    iosLibraries.Add("ee-x/unity-ads");
                }
            }
            var repositories = new XElement("repositories");
            foreach (var repository in androidRepositories) {
                repositories.Add(new XElement("repository", repository));
            }
            androidPackages.Add(repositories);
            foreach (var library in androidLibraries) {
                androidPackages.Add(new XElement("androidPackage", new XAttribute("spec", library)));
            }
            foreach (var library in fastAndroidLibraries) {
                androidPackages.Add(new XElement("androidPackage", new XAttribute("spec", library)));
            }
            foreach (var library in iosLibraries) {
                var pod = new XElement("iosPod", new XAttribute("name", library));
                if (IsDeveloperModeEnabled) {
                    pod.Add(new XAttribute("path", LibraryPath));
                } else {
                    pod.Add(new XAttribute("version", LibraryVersion));
                }
                iosPods.Add(pod);
            }
            document.Save(LibraryDependenciesFile);
        }
    }
}