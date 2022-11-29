#if UNITY_IOS
using System.Collections.Generic;
using System.IO;
using System.Linq;

using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using UnityEditor.iOS.Xcode;

using UnityEngine;

namespace EE.Editor {
    public class IosProcessor : IPostprocessBuildWithReport {
        public int callbackOrder { get; }

        public void OnPostprocessBuild(BuildReport report) {
            UpdateXcodeProject(report);
            UpdateInfoPlist(report);
        }

        private void UpdateXcodeProject(BuildReport report) {
            // https://support.unity.com/hc/en-us/articles/207942813-How-can-I-disable-Bitcode-support-
            var projectPath = report.summary.outputPath + "/Unity-iPhone.xcodeproj/project.pbxproj";

            var pbxProject = new PBXProject();
            pbxProject.ReadFromFile(projectPath);

            // Disabling Bitcode on all targets.
            // Main.
            var target = pbxProject.GetUnityMainTargetGuid();
            pbxProject.SetBuildProperty(target, "ENABLE_BITCODE", "NO");

            // Unity Tests.
            target = pbxProject.TargetGuidByName(PBXProject.GetUnityTestTargetName());
            pbxProject.SetBuildProperty(target, "ENABLE_BITCODE", "NO");

            // Unity Framework.
            target = pbxProject.GetUnityFrameworkTargetGuid();
            pbxProject.SetBuildProperty(target, "ENABLE_BITCODE", "NO");

            pbxProject.WriteToFile(projectPath);
        }

        private void UpdateInfoPlist(BuildReport report) {
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
            var networks = new List<string> {
                "4fzdc2evr5", // Aarki. 
                "4pfyvq9l8r", // AdColony. 
                "ydx93a7ass", // Adikteev (Trademob).
                "488r3q3dtq", // Adtiming.
                "v72qych5uu", // Appier.
                "6xzpu9s2p8", // Applift. 
                "ludvb6z3bs", // AppLovin. 
                "mlmmfzh3r3", // Appreciate (Triapodi).
                "lr83yxwka7", // Apptimus.
                "c6k4g5qg8m", // Beeswax.
                "wg4vff78zm", // Bidmachine.
                "3sh42y64q3", // Centro.
                "f38h382jlk", // Chartboost. 
                "hs6bdukanm", // Criteo. 
                "9rd848q2bz", // Criteo SA (Manage).
                "prcb7njmu6", // CrossInstall (Twitter).
                "52fl2v3hgk", // Curate. 
                "m8dbw4sv7c", // Dataseat. 
                "m5mvw97r93", // Discipline Digital.
                "v9wttpbfk9", // FAN 1.
                "n38lu8286q", // FAN 2.
                "fz2k2k5tej", // FeedMob. 
                "g2y4y55b64", // GlobalWide Media. 
                "cstr6suwn9", // Google AdMob.
                "w9q455wk68", // Hybrid Adtech. 
                "wzmmz9fp6w", // InMobi. 
                "su67r6k2v3", // ironSource. 
                "yclnxrl5pm", // Jampp.
                "4468km3ulz", // Kayzen (Realtime Technologies).
                "v79kvwwj4g", // Kidoz. 
                "t38b2kh725", // Lifestreet. 
                "7ug5zh24hu", // Liftoff.
                "5lm9lj6jb7", // Loopme. 
                "zmvfpc5aq8", // Maiden Marketing Pvt. 
                "kbd757ywx3", // Mintegral (Mobvista).
                "275upjj5gd", // Mobupps.
                "9t245vhmpl", // Moloco. 
                "238da6jt44", // Pangle/Bytedance-China. 
                "22mmun2rn5", // Pangle/Bytedance-Non china. 
                "44jx6755aq", // Personaly.
                "tl55sbb4fm", // PubNative. 
                "24zw6aqk47", // Qverse. 
                "2u9pt9hc89", // Remerge. 
                "8s468mfl3y", // RTB House.
                "glqzh8vgby", // Sabio Mobile. 
                "av6w8kgt66", // ScaleMonk. 
                "klf5c3l5u5", // Sift. 
                "ppxm28t8ap", // Smadex.
                "424m5254lk", // Snap.
                "f73kdq92p3", // Spotad. 
                "44n7hlldy6", // Skyke Media. 
                "ecpz2srf59", // TapJoy.
                "pwa73g5rt2", // Tremor (Unruly). 
                "4dzt52r2t5", // Unity 1.
                "bvpn9ufa9b", // Unity 2.
                "gta9lk7p23", // Vungle.
                "3rd42ekr43", // YouAppi.

                // https://developers.google.com/admob/ios/3p-skadnetworks
                "2fnua5tdw4", // Adform.
                "5a6flpkh64", // Affle India (RevX).
                "p78axxw29g", // Amazon.
                "cp8zw746q7", // Arpeely.
                "s39g8k73mm", // Bidease.
                "3qy4746246", // Bigabid.
                "v4nxqhlyqp", // i-mobile.
                "y5ghdn5j9k", // Mediaforce.
                "n6fk4nfna4", // MediaMath.
                "47vhws6wlr", // MicroAd.
                "a2p9lx4jpn", // Opera.
                "uw77j35x4d", // The Trade Desk.
                "578prtvx9j", // Unicorn (AdWays).
                "e5fvkxwrpn", // Yahoo (Verizon).
                "8c4e2ghe7u", // Yahoo Japan Ads.
                "zq492l623r", // Yandex.
                "3qcr597p9d", // Zucks.

                // https://developers.ironsrc.com/ironsource-mobile/ios/ios-14-network-support
                // https://developers.is.com/skadnetworkids.xml
                "5tjdwbrq8w", // Webeye.
                "f7s53z58qe", // Tencent.

                // https://support.adcolony.com/helpdesk/network-ids-for-skadnetwork-ios-only/
                "6g9af3uyq4", // AdColony (Mobilike).
                "cg4yq2srnc", // AdTheorent.
                "pwdxu55a5a", // Avazu.
                "g28c52eehv", // BidSlash.
                "523jb4fst2", // Bidsopt.
                "294l99pt4k", // Bidswitch.
                "9nlqeag3gk", // Edge 226.
                "cj5566h2ga", // EngageBDR.
                "u679fj5vs4", // Fiksu.
                "a7xqa6mtl2", // Glispa.
                "rx5hdcabgc", // LiquidM.
                "7rz58n8ntl", // MediaSmart.
                "ejvt5qm6ak", // MMG (Geofy).
                "n9x2a789qt", // MyTarget.
                "74b6s63p6l", // SmartAds.
                "kbmxgpxpgc", // StackAdapt.
                "5l3tpt7t6e", // StartApp.
                "6964rsfnh4", // ThingOrTwo.
                "84993kbrcf", // Tradedoubler.
                "mtkv5xtk9e", // Valassis (Maxpoint).
                "r45fhb6rf7", // WidenAd.

                // https://vungle.com/skadnetworkids.xml
                "c3frkrj4fj", // Adelphic.
                "ggvn48r87g", // Blis Media.
                "mls7yz5dvl", // Bucksense.
                "24t9a8vw3c", // Cheetah Medialink.
                "x44k69ngh6", // Clearpier.
                "mp6xlyr22a", // Clearpier Performance.
                "9b89h5y424", // FrontPorch.
                "7fmhfwg9en", // IQzone.
                "feyaarzu9v", // Mezzomedia.
                "7953jerfzd", // Mopub.
                "k674qkevps", // Pubmatic.
                "xy9t38ct57", // SMN.
                "qu637u8glc", // Sportradar.
                "54nzkqm89y", // Tradingworks.
                "9yg77x724h", // Twitter 1.
                "n66cz3y3bx", // Twitter 2.
                "32z4fx6l9h", // Yeahmobi.

                // https://dash.applovin.com/documentation/mediation/ios/getting-started/skadnetwork-info
                "3l6bd9hu43",
                "79pbpufp6p",
                "a8cz6cu7e5",
                "dkc879ngq3",
                "nzq8sh4pbs",
                "qqp299437r",
                "rvh3l7un93",
                "vcra2ehyfk",
                "x5l83yy675",
                "x8jxxk4ff5",
                "x8uqf25wch",
                "6p4ks3rnbw",
                "737z793b9f",
                "97r2b46745",
                "dzg6xy7pwj",
                "hdw39hrw9y",
                "s69wq72ugq",
                "y45688jllp",
                "b9bk5wbcq9",
                "bxvub5ada5",
                "krvm3zuq6h",
                "252b5q8x7y",
                "9g2aggbj52",
                "nu4557a4je",
                "4mn522wn87",
                "eh6m2bh4zr",
                "vutu7akeur",
                "6v7lgmsu45",
                "89z7zv988g",
                "8m87ys6875",
                "hb56zgv37p",
                "m297p6643m",
                "gvmwg8q7h5",
                "pu4na253f3",
                "yrqqpx2mcb",
                "z4gj7hsk7h",
                "x2jnk7ly8j",
                "ln5gz23vtd",
            }.Select(item => $"{item}.skadnetwork");
            SetSkAdNetwork(plist, networks.ToArray());
            plist.WriteToFile(plistPath);
        }

        private static void SetSkAdNetwork(PlistDocument plist, params string[] networks) {
            var items = new PlistElementArray();
            foreach (var network in networks) {
                items.AddDict().SetString("SKAdNetworkIdentifier", network);
            }
            plist.root["SKAdNetworkItems"] = items;
        }
    }
}

#endif // UNITY_IOS