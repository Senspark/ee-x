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
            if (settings.IsAdMobEnabled ||
                settings.IsIronSourceMediationEnabled) {
                var items = plist.root["SKAdNetworkItems"]?.AsArray() ?? plist.root.CreateArray("SKAdNetworkItems");
                items.AddDict().SetString("SKAdNetworkIdentifier", "cstr6suwn9.skadnetwork");
            }
            if (settings.IsAdMobMediationEnabled ||
                settings.IsFacebookAdsEnabled ||
                settings.IsIronSourceMediationEnabled) {
                var items = plist.root["SKAdNetworkItems"]?.AsArray() ?? plist.root.CreateArray("SKAdNetworkItems");
                items.AddDict().SetString("SKAdNetworkIdentifier", "v9wttpbfk9.skadnetwork");
                items.AddDict().SetString("SKAdNetworkIdentifier", "n38lu8286q.skadnetwork");
            }
            if (settings.IsAdMobMediationEnabled ||
                settings.IsIronSourceMediationEnabled ||
                settings.IsUnityAdsEnabled) {
                var items = plist.root["SKAdNetworkItems"]?.AsArray() ?? plist.root.CreateArray("SKAdNetworkItems");
                items.AddDict().SetString("SKAdNetworkIdentifier", "4DZT52R2T5.skadnetwork");
                items.AddDict().SetString("SKAdNetworkIdentifier", "bvpn9ufa9b.skadnetwork");
            }
            plist.WriteToFile(plistPath);
        }
    }
}

#endif // UNITY_IOS