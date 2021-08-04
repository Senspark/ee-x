# Getting Started
## Requirements
- Unity 2020.1+: <https://unity3d.com/get-unity/download>
- Android SDK: 17+
- iOS SDK: 12.0+

## Configuration

### Installation
- Add the following block to **Packages/manifest.json**

    			"scopedRegistries": [
			    {
			      "name": "Senspark",
			      "url": "http://senspark.com:9999",
			      "scopes": [
			        "com.google"
			      ]
			    },
			    {
			      "name": "npmjs",
			      "url": "https://registry.npmjs.org/",
			      "scopes": [
			        "com.senspark"
			      ]
			    }
			  ]

            

- Go to **Window/Package Manager** and install **Senspark EE-x** package in **My Registries** packages
![](getting-started-1.png)

- Go to **Assets/Senspark EE-x/Settings** to customize plugin settings:
![](getting-started-2.png)

## Initialization
```csharp
public class TestScene : MonoBehavior {
    private void Awake() {
        // Call this method only once.
        EE.Utils.NoAwait(Initialize);
    }
    
    public async Task Initialize() {
        EE.PluginManager.InitializePlugins();
        await Platform.RequestTrackingAuthorization();
    }
}
```
