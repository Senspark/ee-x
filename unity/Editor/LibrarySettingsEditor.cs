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
            settings.IsCoreEnabled = EditorGUILayout.Toggle(new GUIContent("Core"), settings.IsCoreEnabled);
            settings.IsAdMobEnabled = EditorGUILayout.Toggle(new GUIContent("AdMob"), settings.IsAdMobEnabled);
            EditorGUILayout.Separator();
            EditorGUILayout.LabelField("AdMob App ID");
            settings.AdMobAndroidAppId = EditorGUILayout.TextField("Android", settings.AdMobAndroidAppId);
            settings.AdMobIosAppId = EditorGUILayout.TextField("iOS", settings.AdMobIosAppId);
            EditorGUILayout.Separator();
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