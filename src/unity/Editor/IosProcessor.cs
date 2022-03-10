#if UNITY_IOS
using System.Collections.Generic;
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
            if (settings.OverwriteNSUserTrackingUsageDescription) {
                plist.root.SetString("NSUserTrackingUsageDescription",
                    "This identifier will be used to deliver personalized ads to you.");
            }

            // SkAdNetwork
            // https://www.skanids.com/
            // https://developers.ironsrc.com/ironsource-mobile/ios/ios-14-network-support
            var networks = new List<string> {
                "4fzdc2evr5.skadnetwork", "4pfyvq9l8r.skadnetwork", "ydx93a7ass.skadnetwork", "488r3q3dtq.skadnetwork",
                "v72qych5uu.skadnetwork", "6xzpu9s2p8.skadnetwork", "ludvb6z3bs.skadnetwork", "mlmmfzh3r3.skadnetwork",
                "lr83yxwka7.skadnetwork", "c6k4g5qg8m.skadnetwork", "wg4vff78zm.skadnetwork", "3sh42y64q3.skadnetwork",
                "f38h382jlk.skadnetwork", "hs6bdukanm.skadnetwork", "9rd848q2bz.skadnetwork", "prcb7njmu6.skadnetwork",
                "52fl2v3hgk.skadnetwork", "m8dbw4sv7c.skadnetwork", "m5mvw97r93.skadnetwork", "v9wttpbfk9.skadnetwork",
                "n38lu8286q.skadnetwork", "fz2k2k5tej.skadnetwork", "g2y4y55b64.skadnetwork", "cstr6suwn9.skadnetwork",
                "w9q455wk68.skadnetwork", "wzmmz9fp6w.skadnetwork", "su67r6k2v3.skadnetwork", "yclnxrl5pm.skadnetwork",
                "4468km3ulz.skadnetwork", "v79kvwwj4g.skadnetwork", "t38b2kh725.skadnetwork", "7ug5zh24hu.skadnetwork",
                "5lm9lj6jb7.skadnetwork", "zmvfpc5aq8.skadnetwork", "kbd757ywx3.skadnetwork", "275upjj5gd.skadnetwork",
                "9t245vhmpl.skadnetwork", "238da6jt44.skadnetwork", "22mmun2rn5.skadnetwork", "44jx6755aq.skadnetwork",
                "tl55sbb4fm.skadnetwork", "24zw6aqk47.skadnetwork", "2u9pt9hc89.skadnetwork", "8s468mfl3y.skadnetwork",
                "glqzh8vgby.skadnetwork", "av6w8kgt66.skadnetwork", "klf5c3l5u5.skadnetwork", "ppxm28t8ap.skadnetwork",
                "424m5254lk.skadnetwork", "f73kdq92p3.skadnetwork", "44n7hlldy6.skadnetwork", "ecpz2srf59.skadnetwork",
                "pwa73g5rt2.skadnetwork", "4dzt52r2t5.skadnetwork", "bvpn9ufa9b.skadnetwork", "gta9lk7p23.skadnetwork", 
                "3rd42ekr43.skadnetwork",
            };
            AddSkAdNetwork(plist, networks.ToArray());
            plist.WriteToFile(plistPath);
        }

        private static void AddSkAdNetwork(PlistDocument plist, params string[] networks) {
            var items = plist.root["SKAdNetworkItems"]?.AsArray() ?? plist.root.CreateArray("SKAdNetworkItems");
            foreach (var network in networks) {
                items.AddDict().SetString("SKAdNetworkIdentifier", network);
            }
        }
    }
}

#endif // UNITY_IOS