# Getting Started
## Configuration
### Prerequisite
- External Dependency Manager: <https://github.com/googlesamples/unity-jar-resolver>

### Installation
- Add the **Senspark** to your Unity project
    - If you use Unity 2019.4.12+ then go to **Edit/Project Settings/Package Manager** and add **Senspark** scoped registry
    ![](getting-started-1.png)

    - Else you can also manually add the following block to **Packages/manifest.json**

            "scopedRegistries": [
              {
                "name": "Senspark",
                "url": "https://registry.npmjs.org",
                "scopes": [
                  "com.senspark"
                ]
              }
            ]
            

- Go to **Window/Package Manager** and install **Senspark EE-x** package in **My Registries** packages
![](getting-started-2.png)

- Go to **Assets/Senspark EE-x/Settings** to customize plugin settings

## Initialization
```csharp
public class TestScene : MonoBehavior {
    private void Awake() {
        // Call this method only once.
        EE.PluginManager.InitializePlugins();
    }
}
```
