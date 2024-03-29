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

            // Core plugin is always enabled.
            // Prevent user from changing.
            EditorGUI.BeginDisabledGroup(true);
            settings.IsCoreEnabled = EditorGUILayout.Toggle(new GUIContent("Core"), settings.IsCoreEnabled);
            EditorGUI.EndDisabledGroup();

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
            settings.IsAdMobMediationEnabled =
                EditorGUILayout.Toggle(new GUIContent("Use Mediation"), settings.IsAdMobMediationEnabled);
            settings.IsAdMobTestSuiteEnabled =
                EditorGUILayout.Toggle(new GUIContent("Add Test Suite"), settings.IsAdMobTestSuiteEnabled);
            settings.AdMobAndroidAppId = EditorGUILayout.TextField("Android App ID", settings.AdMobAndroidAppId);
            settings.AdMobIosAppId = EditorGUILayout.TextField("iOS App ID", settings.AdMobIosAppId);
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
            settings.IsIronSourceMediationEnabled =
                EditorGUILayout.Toggle(new GUIContent("Use Mediation"), settings.IsIronSourceMediationEnabled);
            --EditorGUI.indentLevel;
            EditorGUI.EndDisabledGroup();

            // UnityAds plugin.
            settings.IsUnityAdsEnabled = EditorGUILayout.Toggle(new GUIContent("UnityAds"), settings.IsUnityAdsEnabled);

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