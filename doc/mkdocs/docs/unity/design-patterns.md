# Design Patterns
## Service Locator
- Use *Service Locator* instead of *Singleton*.
- A service must:
    + Implement `EE.IService`.
    + Use `EE.ServiceAttribute` to register a unique name.
- Usages:
    + Use `EE.ServiceLocator.Provide` to set a service.
    + Use `EE.ServiceLocator.Resolve` to get a service.
- Samples:
```csharp
// Creates services.
var dataManager = new EE.DefaultDataManager();
var sceneLoader = new EE.DefaultSceneLoader();

// Initializes services.
await dataManager.Initialize();
await sceneLoader.Initialize();

// Registers services.
EE.ServiceLocator.Provide<EE.IDataManager>(dataManager);
EE.ServiceLocator.Provide<EE.ISceneLoader>(sceneLoader);

...

// Uses services.
EE.ServiceLocator.Resolve<EE.ISceneLoader>().LoadScene<GameScene>("GameScene");

var dataManager = EE.ServiceLocator.Resolve<EE.IDataManager>();
var times = dataManager.Get("times", 1);
times++;
dataManager.Set("times", times);
```
## Observer
## Decorator