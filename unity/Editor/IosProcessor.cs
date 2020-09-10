#if UNITY_IOS
using System.IO;

using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using UnityEditor.iOS.Xcode;

using UnityEngine;

namespace EE.Editor {
    public class IosProcessor : IPostprocessBuildWithReport {
        public int callbackOrder { get; }

        public void OnPostprocessBuild(BuildReport report) {
            var plistPath = Path.Combine(report.summary.outputPath, "Info.plist");
            var plist = new PlistDocument();
            plist.ReadFromFile(plistPath);
            var settings = LibrarySettings.Instance;
            if (settings.IsAdMobEnabled) {
                var appId = settings.AdMobIosAppId;
                if (appId.Length == 0) {
                    Debug.LogError("AdMob iOS App Id is missing");
                } else {
                    plist.root.SetString("GADApplicationIdentifier", appId);
                }
            }
            plist.WriteToFile(plistPath);
        }
    }
}

#endif // UNITY_IOS