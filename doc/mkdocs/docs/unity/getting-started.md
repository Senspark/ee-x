# Getting Started
## Configuration
Prerequisite:

- External Dependency Manager: <https://github.com/googlesamples/unity-jar-resolver>
- How to install package from a Git URL: <https://docs.unity3d.com/Manual/upm-ui-giturl.html>

Installation:

- Install package from this Git URL:
```
https://github.com/Senspark/ee-x.git?path=/unity#develop
```
- Go to **Assets/Senspark EE-x/Settings** to customize plugin settings

## Initialization
```csharp
public class TestScene : MonoBehavior {
    private void Awake() {
        EE.PluginManager.InitializePlugins();
    }
}
```
