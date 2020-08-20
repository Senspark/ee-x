#if UNITY_EDITOR

using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml.Linq;

using UnityEditor.Android;

using UnityEngine;
using UnityEngine.Assertions;

namespace EE.Editor {
    public class AndroidProcessor : IPostGenerateGradleAndroidProject {
        private static readonly XNamespace ManifestNamespace = "http://schemas.android.com/apk/res/android";
        private const string MetaAdMobApplicationId = "com.google.android.gms.ads.APPLICATION_ID";

        public int callbackOrder { get; }

        public void OnPostGenerateGradleAndroidProject(string path) {
            var gradlePath = Path.Combine(path, "..", "launcher", "build.gradle");
            var gradleConfig = new GradleConfig(gradlePath);
            var manifestPath = Path.Combine(path, "src", "main", "AndroidManifest.xml");
            var manifest = XDocument.Load(manifestPath);
            var settings = LibrarySettings.Instance;
            if (settings.IsMultiDexEnabled) {
                SetMultiDexEnabled(gradleConfig, true);
            }
            if (settings.IsAdMobEnabled) {
                AddAdMobAppId(manifest);
            }
            gradleConfig.Save();
        }

        private static void AddAdMobAppId(XContainer manifest) {
            var settings = LibrarySettings.Instance;
            var manifestElement = manifest.Element("manifest");
            Assert.IsNotNull(manifestElement);
            var applicationElement = manifestElement.Element("application");
            Assert.IsNotNull(applicationElement);
            var metaElements = applicationElement
                .Descendants()
                .Where(item => item.Name.LocalName == "meta-data");
            var metaElement = GetMetaElement(metaElements, MetaAdMobApplicationId);
            if (settings.IsAdMobEnabled) {
                var appId = settings.AdMobAndroidAppId;
                if (appId.Length == 0) {
                    Debug.LogError("AdMob iOS App Id is missing");
                } else {
                    if (metaElement == null) {
                        applicationElement.Add(CreateMetaElement(MetaAdMobApplicationId, appId));
                    } else {
                        metaElement.SetAttributeValue(ManifestNamespace + "value", appId);
                    }
                }
            } else {
                metaElement?.Remove();
            }
        }

        private static XElement CreateMetaElement(string name, object value) {
            return new XElement("meta-data",
                new XAttribute(ManifestNamespace + "name", name),
                new XAttribute(ManifestNamespace + "value", value));
        }

        private static XElement GetMetaElement(IEnumerable<XElement> metas, string metaName) {
            foreach (var meta in metas) {
                var attributes = meta.Attributes();
                foreach (var attribute in attributes) {
                    if (attribute.Name.Namespace == ManifestNamespace &&
                        attribute.Name.LocalName == "name" &&
                        attribute.Value == metaName) {
                        return meta;
                    }
                }
            }
            return null;
        }

        private static void SetMultiDexEnabled(GradleConfig config, bool enabled) {
            var android = config.Root.TryGetNode("android");
            var defaultConfig = android.TryGetNode("defaultConfig");
            defaultConfig.RemoveContentNode("multiDexEnabled false");
            defaultConfig.AppendContentNode("multiDexEnabled true");
        }
    }
}

#endif // UNITY_EDITOR