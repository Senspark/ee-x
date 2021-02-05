# Utils
## Platform Utils
```ts
import * as ee_x from '@senspark/ee';

// Checks whether the specified application is installed.
const applicationId = 'com.senspark.ee.test';
const isInstalled = ee_x.Platform.isApplicationInstalled(applicationId);

// Attempts to open the specified application.
const applicationId = 'com.senspark.ee.test';
ee_x.Platform.openApplication(applicationId);

// Gets the current application information.
const applicationId = ee_x.Platform.getApplicationId();
const applicationName = ee_x.Platform.getApplicationName();
const versinName = ee_x.Platform.getVersionName();
const versionCode = ee_x.Platform.getVersionCode();

// Gets the SHA1 signature (Android only).
const signature = ee_x.Platform.getSha1Signature();

// Gets activity (Android) or view (iOS) size in pixels.
const [width, height] = ee_x.Platform.getViewSize();

// Gets the full screen size in pixels (including bars).
const [width, height] = ee_x.Platform.getScreenSize();

// Gets the unique ID for the current device.
ee_x.Utils.noAwait(async () => {
    const deviceId = await ee_x.Platform.getDeviceId();
});

// Composes and send an email.
ee_x.Platform.sendMail("feedback@senspark.com", "ee-x test app", "Hello, ");

// Checks the current internet connection.
ee_x.Utils.noAwait(async () => {
    const isAvailable = await ee_x.Platform.testConnection("www.google.com", 1.0);
});
```

## Other Utils
```ts
import * as ee_x from '@senspark/ee-x';

// Runs an async action inside a synced action.
ee_x.Utils.noAwait(async () => {
    await ee_x.Utils.delay(1.0);
});
```