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
            iosPods.RemoveAll();
            if (IsCoreEnabled) {
                var iosPod = new XElement("iosPod");
                iosPod.Add(new XAttribute("name", "ee-x/core"));
                iosPods.Add(iosPod);
            }
            if (IsAdMobEnabled) {
                var iosPod = new XElement("iosPod");
                iosPod.Add(new XAttribute("name", "ee-x/admob"));
                iosPods.Add(iosPod);
            }
            document.Save(LibraryDependenciesFile);
        }
    }
}

#endif // UNITY_EDITOR