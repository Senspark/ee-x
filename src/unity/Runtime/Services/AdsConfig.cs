using System;
using System.Collections.Generic;
using System.Linq;

using SimpleJSON;

namespace EE {
    internal class AdsConfig {
        internal enum Network {
            AdMob,
            FacebookAds,
            IronSource,
            UnityAds
        }

        internal enum AdFormat {
            Banner,
            Interstitial,
            Rewarded
        }

        internal class NetworkConfig {
            public Network Network { get; protected set; }

            internal static NetworkConfig ParseImpl(JSONNode node) {
                var network = node["network"].Value;
                switch (network) {
                    case "ad_mob": return new AdMobConfig(node);
                    case "facebook_ads": return new FacebookAdsConfig(node);
                    case "iron_source": return new IronSourceConfig(node);
                    case "unity_ads": return new UnityAdsConfig(node);
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
        }

        internal class AdMobConfig : NetworkConfig {
            internal AdMobConfig(JSONNode node) {
                Network = Network.AdMob;
            }
        }

        internal class FacebookAdsConfig : NetworkConfig {
            internal FacebookAdsConfig(JSONNode node) {
                Network = Network.FacebookAds;
            }
        }

        internal class IronSourceConfig : NetworkConfig {
            public string AppId { get; }

            internal IronSourceConfig(JSONNode node) {
                Network = Network.IronSource;
                AppId = node["app_id"];
            }
        }

        internal class UnityAdsConfig : NetworkConfig {
            public string AppId { get; }

            internal UnityAdsConfig(JSONNode node) {
                Network = Network.UnityAds;
                AppId = node["app_id"];
            }
        }

        internal class AdConfig {
            public AdFormat AdFormat { get; protected set; }
            public AdInstanceConfig Instance { get; protected set; }

            internal static AdConfig ParseImpl(JSONNode node) {
                var format = node["format"].Value;
                switch (format) {
                    case "banner": return new BannerConfig(node);
                    case "interstitial": return new InterstitialConfig(node);
                    case "rewarded": return new RewardedConfig(node);
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
        }

        internal class BannerConfig : AdConfig {
            internal BannerConfig(JSONNode node) {
                AdFormat = AdFormat.Banner;
                Instance = AdInstanceConfig.ParseImpl(node["instance"]);
            }
        }

        internal class InterstitialConfig : AdConfig {
            public int Interval { get; }

            internal InterstitialConfig(JSONNode node) {
                AdFormat = AdFormat.Interstitial;
                Interval = node["interval"] ?? 0;
                Instance = AdInstanceConfig.ParseImpl(node["instance"]);
            }
        }

        internal class RewardedConfig : AdConfig {
            internal RewardedConfig(JSONNode node) {
                AdFormat = AdFormat.Rewarded;
                Instance = AdInstanceConfig.ParseImpl(node["instance"]);
            }
        }

        internal class AdInstanceConfig {
            internal static AdInstanceConfig ParseImpl(JSONNode node) {
                if (node.IsArray) {
                    return new WaterfallInstanceConfig(node);
                }
                return new SingleInstanceConfig(node);
            }
        }

        internal class SingleInstanceConfig : AdInstanceConfig {
            public Network Network { get; }
            public string Id { get; }

            internal SingleInstanceConfig(JSONNode node) {
                var networks = new Dictionary<string, Network> {
                    ["ad_mob"] = Network.AdMob,
                    ["facebook_ads"] = Network.FacebookAds,
                    ["iron_source"] = Network.IronSource,
                    ["unity_ads"] = Network.UnityAds
                };
                var network = node["network"].Value;
                Network = networks[network];
                Id = node["id"];
            }
        }

        internal class WaterfallInstanceConfig : AdInstanceConfig {
            public AdInstanceConfig[] Instances { get; }

            internal WaterfallInstanceConfig(JSONNode node) {
                Instances = node.Children.Select(ParseImpl).ToArray();
            }
        }

        public NetworkConfig[] Networks { get; private set; }
        public AdConfig[] Ads { get; private set; }

        internal static AdsConfig Parse(string text) {
            var node = JSON.Parse(text);
            return Parse(node);
        }

        private static AdsConfig Parse(JSONNode node) {
            var result = new AdsConfig {
                Networks = node["networks"].Children.Select(NetworkConfig.ParseImpl).ToArray(),
                Ads = node["ads"].Children.Select(AdConfig.ParseImpl).ToArray()
            };
            return result;
        }
    }
}