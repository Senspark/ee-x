using System.IO;

using UnityEditor.Android;

namespace EE.Editor {
    public class AndroidGradleProcessor : IPostGenerateGradleAndroidProject {
        public int callbackOrder { get; }

        public void OnPostGenerateGradleAndroidProject(string path) {
            var gradlePath = Path.Combine(path, "..", "launcher", "build.gradle");
            var gradleConfig = new GradleConfig(gradlePath);
            var settings = LibrarySettings.Instance;
            if (settings.IsMultiDexEnabled) {
                SetMultiDexEnabled(gradleConfig, true);
            }
            if (settings.IsFixPackageOptionsEnabled) {
                SetFixPackageOptionsEnabled(gradleConfig, true);
            }
            gradleConfig.Save();
        }

        private static void SetMultiDexEnabled(GradleConfig config, bool enabled) {
            var android = config.Root.TryGetNode("android");
            var defaultConfig = android.TryGetNode("defaultConfig");
            defaultConfig.RemoveContentNode("multiDexEnabled false");
            defaultConfig.AppendContentNode("multiDexEnabled true");
        }

        private static void SetFixPackageOptionsEnabled(GradleConfig config, bool enabled) {
            var android = config.Root.TryGetNode("android");
            var packagingOptions = android.TryGetNode("packagingOptions");
            if (packagingOptions == null) {
                packagingOptions = new GradleNode("packagingOptions");
                android.AppendChildNode(packagingOptions);
            }
            packagingOptions.AppendContentNode("exclude 'META-INF/kotlinx-serialization-json.kotlin_module'");
        }
    }
}