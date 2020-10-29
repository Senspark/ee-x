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
    var sceneLoader = new EE.DefaultSceneLoader();
    var dataMangaer = new EE.DefaultDataManager();

    // Initializes services.
    await sceneLoader.Initialize();
    await dataManager.Initialize();

    // Registers services.
    EE.ServiceLocator.Provide<EE.ISceneLoader>(sceneLoader);
    EE.ServiceLocator.Provide<EE.IDataManager>(dataManager);

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