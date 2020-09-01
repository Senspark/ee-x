# Adjust
## Configuration
Go to **Assets/Senspark EE-x/Settings** and enable Adjust plugin

## Basic usage
Initialization
```csharp
var plugin = EE.PluginManager.CreatePlugin<EE.IAdjust>();
plugin.Initialize(new EE.AdjustConfig()
    .SetToken("your_app_token")
    .SetEnvironment(EE.AdjustEnvironment.Production));
```

Set push token
```csharp
plugin.SetPushToken("your_push_token");
```