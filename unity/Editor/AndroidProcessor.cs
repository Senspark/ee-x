#if UNITY_EDITOR

using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Xml.Linq;

using EE.Editor;

using UnityEditor.Android;

using UnityEngine;
using UnityEngine.Assertions;

public class AndroidProcessor : IPostGenerateGradleAndroidProject {
    private static readonly XNamespace ManifestNamespace = "http://schemas.android.com/apk/res/android";
    private const string MetaAdMobApplicationId = "com.google.android.gms.ads.APPLICATION_ID";

    public int callbackOrder { get; }

    public void OnPostGenerateGradleAndroidProject(string path) {
        AddAdMobAppId(path);
        EnableMultiDex(path);
    }

    private static void AddAdMobAppId(string path) {
        var manifestPath = Path.Combine(path, "src", "main", "AndroidManifest.xml");
        var settings = LibrarySettings.Instance;
        var document = XDocument.Load(manifestPath);
        var manifest = document.Element("manifest");
        Assert.IsNotNull(manifest);
        var application = manifest.Element("application");
        Assert.IsNotNull(application);
        var metas = application
            .Descendants()
            .Where(item => item.Name.LocalName == "meta-data");
        var meta = GetMetaElement(metas, MetaAdMobApplicationId);
        if (settings.IsAdMobEnabled) {
            var appId = settings.AdMobAndroidAppId;
            if (appId.Length == 0) {
                Debug.LogError("AdMob iOS App Id is missing");
            } else {
                if (meta == null) {
                    application.Add(CreateMetaElement(MetaAdMobApplicationId, appId));
                } else {
                    meta.SetAttributeValue(ManifestNamespace + "value", appId);
                }
            }
        } else {
            meta?.Remove();
        }
        document.Save(manifestPath);
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

    private static void EnableMultiDex(string path) {
        var gradlePath = Path.Combine(path, "..", "launcher", "build.gradle");
        var file = new GradleConfig(gradlePath);
        var android = file.Root.TryGetNode("android");
        var defaultConfig = android.TryGetNode("defaultConfig");
        defaultConfig.AppendContentNode("multiDexEnabled true");
        file.Save();
    }
}

#endif // UNITY_EDITOR