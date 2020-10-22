# Utils
## Platform Utils
```csharp
// Checks whether the specified application is installed.
var applicationId = "com.senspark.ee.test";
var isInstalled = EE.Platform.IsApplicationInstalled(applicationId);

// Attempts to open the specified application.
var applicationId = "com.senspark.ee.test";
EE.Platform.OpenApplication(applicationId);

// Gets the SHA1 signature (Android only).
var signature = EE.Platform.GetSha1Signature();

// Gets activity (Android) or view (iOS) size in pixels.
var (width, height) = EE.Platform.GetViewSize();

// Gets the full screen size in pixels (including bars)
var (width, height) = EE.Platform.GetScreenSize();

// Gets the unique ID for the current device.
EE.Utils.NoAwait(async () => {
    var deviceId = await EE.Platform.GetDeviceId();
});

// Composes and sends an email.
EE.Platform.SendMail("feedback@senspark.com", "ee-x test app", "Hello, ");

// Checks the current internet connection.
EE.Utils.NoAwait(async () => {
    var isAvailable = await EE.Platform.TestConnection("www.google.com", 1.0f);
});
```

## Thread Utils
```csharp
// Runs an action on Unity thread.
EE.Thread.RunOnLibraryThread(() => {
    // Prints the current thread ID.
    Debug.Log($"CurrentThreadId = {Thread.CurrentThread.ManagedThreadId}");
});

// Runs an action on UI thread (Android) or Main thread (iOS).
EE.Thread.RunOnMainThread(() => {
    // Prints the current thread ID.
    Debug.Log($"CurrentThreadId = {Thread.CurrentThread.ManagedThreadId}");
});
```

## Other Utils
```csharp
// Runs an async action inside a synced action.
EE.NoAwait(async () => {
    await Task.Delay(1000);
});
```