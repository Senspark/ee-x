# Multi Ad
### Android
Modify `build.gradle`
```java
dependencies {
    implementation 'com.senspark.ee:ad-mob-mediation:2.4.2'
    implementation 'com.senspark.ee:facebook-ads:2.4.2'
    implementation 'com.senspark.ee:iron-source-mediation:2.4.2'
    implementation 'com.senspark.ee:unity-ads:2.4.2'
}
```

### iOS
Modify `Podfile`
```ruby
pod 'ee-x/ad-mob-mediation', '2.4.2'
pod 'ee-x/facebook-ads', '2.4.2'
pod 'ee-x/iron-source-mediation', '2.4.2'
pod 'ee-x/unity-ads', '2.4.2'
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
manager.isBannerAdVisible = true;

// Set ad position and anchor.
const winSize = cc.winSize();
manager.bannerAdPosition = [winSize.width / 2, winSize.height / 2];
manager.bannerAdAnchor = [0.5f, 0.5f];
```

Rectangle banner ad
```ts
// Show ad.
manager.isRectangleAdVisible = true;

// Set ad position and anchor.
const winSize = cc.winSize();
manager.rectangleAdPosition = [winSize.width / 2, winSize.height / 2];
manager.rectangleAdAnchor = [0.5f, 0.5f];
```

App open ad
```ts
const result = await manager.showAppOpenAd();
if (result == ee_x.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```

Interstitial ad
```ts
auto result = await manager.showInterstitialAd();
if (result == ee_x.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```

Rewarded interstitial ad
```ts
const result = await manager.showRewardedInterstitialAd();
if (result == ee_x.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```

Rewarded ad
```ts
const result = await manager.showRewardedAd();
if (result == ee_x.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```