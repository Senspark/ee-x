# Appsflyer
## Configuration
- Go to **Assets/Senspark EE-x/Settings** and enable Appsflyer plugin
- Add 2 attributes to Assets/Plugins/Android/AndroidManifest.xml
```
<application...
          android:fullBackupContent="@xml/backup_rule"
          tools:replace="android:fullBackupContent"
          ...>
```
- Add the file named **backup_rule.xml** to *Assets/Plugins/Android/FirebaseApp.androidlib/res/xml/*:
```
<?xml version="1.0" encoding="utf-8"?>
<full-backup-content>
    <exclude domain="file" path="vungle" />
    <exclude domain="file" path="vungle_cache" />
    <exclude domain="external" path="vungle_cache" />
    <exclude domain="database" path="vungle_db" />
    <exclude domain="sharedpref" path="com.vungle.sdk.xml" />
</full-backup-content>
```

## Basic usage
Initialization
```csharp
var plugin = PluginManager.CreatePlugin<IAppsFlyer>();
plugin.Initialize("dev key", "ios app id");
plugin.StartTracking();
```

## Get dev key and ios app id
- Dev key: contact Mr.Hai
- iOS app id:

![](appsflyer.png)