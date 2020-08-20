#if UNITY_EDITOR

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
            EditorGUILayout.LabelField("Utilities", EditorStyles.boldLabel);
            settings.InjectMultiDex =
                EditorGUILayout.Toggle(new GUIContent("Inject MultiDex"), settings.InjectMultiDex);
            EditorGUILayout.Separator();
            EditorGUILayout.LabelField("Modules", EditorStyles.boldLabel);

            // Core plugin is always enabled.
            // Prevent user from changing.
            EditorGUI.BeginDisabledGroup(true);
            settings.IsCoreEnabled = EditorGUILayout.Toggle(new GUIContent("Core"), settings.IsCoreEnabled);
            EditorGUI.EndDisabledGroup();

            // Adjust plugin.
            settings.IsAdjustEnabled = EditorGUILayout.Toggle(new GUIContent("Adjust"), settings.IsAdjustEnabled);

            // AdMob plugin.
            settings.IsAdMobEnabled = EditorGUILayout.Toggle(new GUIContent("AdMob"), settings.IsAdMobEnabled);
            EditorGUILayout.Separator();
            EditorGUI.BeginDisabledGroup(!settings.IsAdMobEnabled);
            EditorGUILayout.LabelField("AdMob App ID");
            settings.AdMobAndroidAppId = EditorGUILayout.TextField("Android", settings.AdMobAndroidAppId);
            settings.AdMobIosAppId = EditorGUILayout.TextField("iOS", settings.AdMobIosAppId);
            EditorGUI.EndDisabledGroup();
            EditorGUILayout.Separator();

            // IronSource plugin.
            settings.IsIronSourceEnabled =
                EditorGUILayout.Toggle(new GUIContent("IronSource"), settings.IsAdjustEnabled);

            if (GUI.changed) {
                OnSettingsChanged();
            }
        }

        private void OnSettingsChanged() {
            EditorUtility.SetDirty(target);
            LibrarySettings.Instance.WriteSettingsToFile();
        }
    }
}

#endif // UNITY_EDITOR