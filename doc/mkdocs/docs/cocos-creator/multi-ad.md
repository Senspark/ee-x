# Multi Ad
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:ad-mob-mediation:2.9.0'
    implementation 'com.senspark.ee:facebook-ads:2.9.0'
    implementation 'com.senspark.ee:iron-source-mediation:2.9.0'
    implementation 'com.senspark.ee:unity-ads:2.9.0'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/ad-mob-mediation', '2.9.0'
pod 'ee-x/facebook-ads', '2.9.0'
pod 'ee-x/iron-source-mediation', '2.9.0'
pod 'ee-x/unity-ads', '2.9.0'
```

## Basic usage
Initializes with JSON config
```ts
import * as ee_x from '@senspark/ee-x';

cc.resources.load<cc.JsonAsset>('config_path', (err, res) => {
    const manager = new ee_x.DefaultAdsManager(JSON.stringify(res.json));
    ee_x.Utils.noAwait(async () => {
        await manager.initialize();
    });
});
```

Normal banner ad
```ts
// Show ad.
manager.bannerAd.isVisible = true;

// Set ad position and anchor.
const winSize = cc.winSize();
manager.bannerAd.position = [winSize.width / 2, winSize.height / 2];
manager.bannerAd.anchor = [0.5f, 0.5f];
```

Rectangle banner ad
```ts
// Show ad.
manager.rectangleAd.isVisible = true;

// Set ad position and anchor.
const winSize = cc.winSize();
manager.rectangleAd.position = [winSize.width / 2, winSize.height / 2];
manager.rectangleAd.anchor = [0.5f, 0.5f];
```

App open ad
```ts
const result = await manager.appOpenAd.show();
if (result == ee_x.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```

Interstitial ad
```ts
auto result = await manager.interstitialAd.show();
if (result == ee_x.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```

Rewarded interstitial ad
```ts
const result = await manager.rewardedInterstitialAd.show();
if (result == ee_x.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```

Rewarded ad
```ts
const result = await manager.rewardedAd.show();
if (result == ee_x.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```