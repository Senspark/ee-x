using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml.Linq;

using UnityEditor.Build;
using UnityEditor.Build.Reporting;

using UnityEngine;
using UnityEngine.Assertions;

namespace EE.Editor {
    public class AndroidManifestProcessor : IPreprocessBuildWithReport {
        private static readonly XNamespace ManifestNamespace = "http://schemas.android.com/apk/res/android";
        private const string MetaAdMobApplicationId = "com.google.android.gms.ads.APPLICATION_ID";

        public int callbackOrder { get; }

        public void OnPreprocessBuild(BuildReport report) {
            var projectDir = Path.Combine(Application.dataPath, "Plugins", "Android", "EE");
            Directory.CreateDirectory(projectDir);
            EnsureProjectProperties(projectDir);
            var manifestPath = Path.Combine(projectDir, "AndroidManifest.xml");
            XDocument manifest;
            try {
                manifest = XDocument.Load(manifestPath);
            } catch (IOException ex) {
                manifest = new XDocument();
                var manifestElement = new XElement("manifest",
                    new XAttribute(XNamespace.Xmlns + "android", ManifestNamespace),
                    new XAttribute("package", "com.senspark.ee"));
                manifest.Add(manifestElement);
            }
            var settings = LibrarySettings.Instance;
            if (settings.IsAdMobEnabled) {
                AddAdMobAppId(settings, manifest);
            }
            manifest.Save(manifestPath);
        }

        private static void EnsureProjectProperties(string projectDir) {
            var propertiesPath = Path.Combine(projectDir, "project.properties");
            if (File.Exists(propertiesPath)) {
                // OK.
            } else {
                File.WriteAllLines(propertiesPath, new[] {
                    "target=android-19",
                    "android.library=true",
                });
            }
        }

        private static void AddAdMobAppId(LibrarySettings settings, XContainer manifest) {
            var manifestElement = manifest.Element("manifest");
            Assert.IsNotNull(manifestElement);
            var applicationElement = manifestElement.Element("application");
            if (applicationElement == null) {
                applicationElement = new XElement("application");
                manifestElement.Add(applicationElement);
            }
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
    }
}