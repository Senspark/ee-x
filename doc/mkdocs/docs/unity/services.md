# Services
## Audio
Initialization
```csharp
// Declare audio enum.
public enum Audio {
    Tap,
    LevelStart,
    LevelEnd,
    MenuScene,
    LevelScene,
};

// Create services.
var dataManager = new EE.DefaultDataManager();
var audioManager = new EE.DefaultAudioManager(dataManager, {
    [Audio.Tap] = new EE.AudioInfo("Audio/sfx_tap"),
    [Audio.LevelStart] = new EE.AudioInfo("Audio/sfx_level_start"),
    [Audio.LevelEnd] = new EE.AudioInfo("Audio/sfx_level_end"),
    [Audio.MenuScene] = new EE.AudioInfo("Audio/bgm_menu_scene"),
    [Audio.LevelScene] = new EE.AudioInfo("Audio/bgm_level_scene")
});

await audioManager.Initialize();

// Register services.
EE.ServiceLocator.Provide(audioManager);
```

Basic usage
```csharp
var manager = EE.ServiceLocator.Resolve<EE.IAudioManager>();

// Enable/disable music/sound.
manager.IsMusicEnabled = true;
manager.IsSoundEnabled = true;

// Adjust volume.
manager.MusicVolume = 1f;
manager.SoundVolume = 1f;

// Play music.
manager.PlayMusic(Audio.MenuScene);

// Play sound.
manager.PlaySound(Audio.Tap);
```

## Scene Loader
Initialization
```csharp
// Create services
var sceneLoader = new EE.DefaultSceneLoader();

await sceneLoader.Initialize();

// Register services.
EE.ServiceLocator.Provide(sceneLoader);
```

Basic usage
```csharp
// Load a scene asynchronously.
var loader = EE.ServiceLocator.Resolve<EE.ISceneLoader>();
var scene = await loader.LoadScene<MenuScene>(nameof(MenuScene));

// Do something after the scene is loaded.
scene.OpenOfferDialog();
```

## Firebase Analytics
Initialization
```csharp
// Create services.
#if UNITY_EDITOR
var analyticsManager = new EE.NullAnalyticsManager();
#else
var analyticsManager = new EE.FirebaseAnalyticsManager();
#endif

await analyticsManager.Initialize();

// Register services.
EE.ServiceLocator.Provide(analyticsManager);
```

Basic usage
```csharp
// Track screen.
// Used in scene.
private void Awake() {
    var manager = EE.ServiceLocator.Resolve<EE.IAnalyticsManager>();
    manager.PushScreen("menu_scene");
}

private void OnDestroy() {
    var manager = EE.ServiceLocator.Resolve<EE.IAnalyticsManager>();
    manager.PopAllScreens();
}

// Used in dialog.
private void Awake() {
    var manager = EE.ServiceLocator.Resolve<EE.IAnalyticsManager>();
    manager.PushScreen("menu_scene");
}

private void OnDestroy() {
    var manager = EE.ServiceLocator.Resolve<EE.IAnalyticsManager>();
    manager.PopScreen();
}

// Track conversion events.
var manager = EE.ServiceLocator.Resolve<EE.IAnalyticsManager>();
manager.LogEvent("conversion_level_start");

// Track custom events.
public class LevelEvent : EE.IAnalyticsEvent {
    public string EventName => "track_level";
    public int level;
}

var manager = EE.ServiceLocator.Resolve<EE.IAnalyticsManager>();
manager.LogEvent(new LevelEvent {
    level = 10
});
```

## Firebase Remote Config
Initialization
```csharp
// Create services.
var dataManager = new EE.DefaultDataManager();
var defaults = new Dictionary<string, object> {
    ["bool_key"] = true,
    ["long_key"] = 10L,
    ["double_key"] = 2.0,
    ["string_key"] = "value"
};

#if UNITY_EDITOR
var remoteConfigManager = new EE.NullRemoteConfigManager(defaults);
#else
var remoteConfigManager = new EE.FirebaseRemoteConfigManager(dataManager, defaults);
#endif

await remoteConfigManager.Initialize();

// Register services.
EE.ServiceLocator.Provide(remoteConfigManager);
```

Basic usage.
```csharp
var manager = EE.ServiceLocator.Resolve<EE.IRemoteConfigManager>();

// Get the data timestamp.
var timestamp = manager.Timestamp;

// Get values.
var boolValue = manager.GetBool("bool_key");
var longValue = manager.GetLong("long_key");
var doubleValue = manager.GetDouble("double_key");
var stringValue = manager.GetString("string_key");
```
