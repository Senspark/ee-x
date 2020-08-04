#if UNITY_EDITOR

using UnityEditor.Build;
using UnityEditor.Build.Reporting;

public class PodfileProcessor : IPostprocessBuildWithReport {
    public int callbackOrder { get; }

    public void OnPostprocessBuild(BuildReport report) {
    }
}

#endif // UNITY_EDITOR