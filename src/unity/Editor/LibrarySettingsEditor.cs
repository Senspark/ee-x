using UnityEditor;

using UnityEngine;

namespace EE.Editor {
    [CustomEditor(typeof(LibrarySettings))]
    public class LibrarySettingsEditor : UnityEditor.Editor {
        [MenuItem("Assets/Senspark EE-x/Settings")]
        public static void OpenInspector() {
            Selection.activeObject = LibrarySettings.Instance;
        }

        public override void OnInspectorGUI() {
            var settings = LibrarySettings.Instance;

            if (GUILayout.Button("Update Dependencies")) {
                UpdateDependencies();
            }

            EditorGUILayout.LabelField("Config", EditorStyles.boldLabel);
            settings.IsDeveloperModeEnabled =
                EditorGUILayout.Toggle(new GUIContent("Developer Mode"), settings.IsDeveloperModeEnabled);
            EditorGUI.BeginDisabledGroup(!settings.IsDeveloperModeEnabled);
            ++EditorGUI.indentLevel;
            settings.LibraryPath = EditorGUILayout.TextField("Library path", settings.LibraryPath);
            --EditorGUI.indentLevel;
            EditorGUI.EndDisabledGroup();

            EditorGUILayout.Separator();
            EditorGUILayout.LabelField("Utilities", EditorStyles.boldLabel);
            settings.IsMultiDexEnabled =
                EditorGUILayout.Toggle(new GUIContent("Use MultiDex"), settings.IsMultiDexEnabled);
            settings.IsFixPackageOptionsEnabled =
                EditorGUILayout.Toggle(new GUIContent("Fix PackageOptions"), settings.IsFixPackageOptionsEnabled);
            settings.OverwriteNSUserTrackingUsageDescription =
                EditorGUILayout.Toggle(new GUIContent("Overwrite NSUserTrackingUsageDescription"),
                    settings.OverwriteNSUserTrackingUsageDescription);

            EditorGUILayout.Separator();
            EditorGUILayout.LabelField("Modules", EditorStyles.boldLabel);

            // Core plugin.
            settings.IsCoreEnabled = EditorGUILayout.Toggle(new GUIContent("Core"), settings.IsCoreEnabled);
            EditorGUI.BeginDisabledGroup(!settings.IsCoreEnabled); // Core.

            // Adjust plugin.
            settings.IsAdjustEnabled = EditorGUILayout.Toggle(new GUIContent("Adjust"), settings.IsAdjustEnabled);

            // AppsFlyer plugin.
            settings.IsAppsFlyerEnabled =
                EditorGUILayout.Toggle(new GUIContent("AppsFlyer"), settings.IsAppsFlyerEnabled);

            // AdColony plugin.
            settings.IsAdColonyEnabled = EditorGUILayout.Toggle(new GUIContent("AdColony"), settings.IsAdColonyEnabled);

            // AdMob plugin.
            settings.IsAdMobEnabled = EditorGUILayout.Toggle(new GUIContent("AdMob"), settings.IsAdMobEnabled);
            EditorGUI.BeginDisabledGroup(!settings.IsAdMobEnabled);
            ++EditorGUI.indentLevel;
            // Config.
            settings.AdMobAndroidAppId = EditorGUILayout.TextField("Android App ID", settings.AdMobAndroidAppId);
            settings.AdMobIosAppId = EditorGUILayout.TextField("iOS App ID", settings.AdMobIosAppId);
            // Mediation.
            var adMobNetworks = new[] {
                AdNetwork.AdColony,
                AdNetwork.AppLovin,
                AdNetwork.Facebook,
                AdNetwork.InMobi,
                AdNetwork.IronSource,
                AdNetwork.Tapjoy,
                AdNetwork.Unity,
                AdNetwork.Vungle,
            };
            foreach (var network in adMobNetworks) {
                settings.SetAdMobMediationEnabled(network,
                    EditorGUILayout.Toggle(new GUIContent($"Use {network.ToString()}"),
                        settings.IsAdMobMediationEnabled(network)));
            }
            --EditorGUI.indentLevel;
            EditorGUI.EndDisabledGroup();

            // FacebookAds plugin.
            settings.IsFacebookAdsEnabled =
                EditorGUILayout.Toggle(new GUIContent("FacebookAds"), settings.IsFacebookAdsEnabled);

            // IronSource plugin.
            settings.IsIronSourceEnabled =
                EditorGUILayout.Toggle(new GUIContent("IronSource"), settings.IsIronSourceEnabled);
            EditorGUI.BeginDisabledGroup(!settings.IsIronSourceEnabled);
            ++EditorGUI.indentLevel;
            // Mediation.
            var ironSourceNetworks = new[] {
                AdNetwork.AdColony,
                AdNetwork.AdMob,
                AdNetwork.AppLovin,
                AdNetwork.Facebook,
                AdNetwork.InMobi,
                AdNetwork.Tapjoy,
                AdNetwork.Unity,
                AdNetwork.Vungle,
            };
            foreach (var network in ironSourceNetworks) {
                settings.SetIronSourceMediationEnabled(network,
                    EditorGUILayout.Toggle(new GUIContent($"Use {network.ToString()}"),
                        settings.IsIronSourceMediationEnabled(network)));
            }
            --EditorGUI.indentLevel;
            EditorGUI.EndDisabledGroup();

            // UnityAds plugin.
            settings.IsUnityAdsEnabled = EditorGUILayout.Toggle(new GUIContent("UnityAds"), settings.IsUnityAdsEnabled);

            EditorGUI.EndDisabledGroup(); // Core.

            if (GUI.changed) {
                OnSettingsChanged();
            }
        }

        private void OnSettingsChanged() {
            EditorUtility.SetDirty(target);
            UpdateDependencies();
        }

        private void UpdateDependencies() {
            LibrarySettings.Instance.WriteSettingsToFile();
        }
    }
}