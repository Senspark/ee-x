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
            gradleConfig.Save();
        }

        private static void SetMultiDexEnabled(GradleConfig config, bool enabled) {
            var android = config.Root.TryGetNode("android");
            var defaultConfig = android.TryGetNode("defaultConfig");
            defaultConfig.RemoveContentNode("multiDexEnabled false");
            defaultConfig.AppendContentNode("multiDexEnabled true");
        }
    }
}