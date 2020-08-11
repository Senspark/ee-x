#if UNITY_EDITOR

using System.IO;

using EE.Editor;

using UnityEditor.Android;

public class GradleProcessor : IPostGenerateGradleAndroidProject {
    public int callbackOrder { get; }

    public void OnPostGenerateGradleAndroidProject(string path) {
        var gradlePath = Path.Combine(path, "..", "launcher", "build.gradle");
        var file = new GradleConfig(gradlePath);
        var android = file.Root.TryGetNode("android");
        var defaultConfig = android.TryGetNode("defaultConfig");
        defaultConfig.AppendContentNode("multiDexEnabled true");
        file.Save();
    }
}

#endif // UNITY_EDITOR