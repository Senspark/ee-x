# Multi Ad
## Configuration
Go to **Assets/Senspark EE-x/Settings** and enable:

- AdMob plugin with mediation
- Facebook Ads plugin
- Iron Source plugin with mediation
- Unity Ads plugin

## Basic usage
Initializes with JSON config
```csharp
var config = Resources.Load<TextAsset>("config_path").text;
var manager = new EE.DefaultAdsManager(config);
EE.Utils.NoAwait(async () => {
    await manager.Initialize();
});
```

Normal banner ad
```csharp
// Show ad.
manager.IsBannerAdVisible = true;

// Set ad position and anchor.
var (viewWidth, viewHeight) = EE.Platform.GetViewSize();
adsManager.BannerAdAnchor = (0.5f, 0.5f);
adsManager.BannerAdPosition = (viewWidth * 0.5f, viewHeight * 0.5f);
```

Rectangle banner ad
```csharp
// Show ad.
manager.IsRectangleAdVisible = true;

// Set ad position and anchor.
var (viewWidth, viewHeight) = EE.Platform.GetViewSize();
adsManager.RectangleAdAnchor = (0.5f, 0.5f);
adsManager.RectangleAdPosition = (viewWidth * 0.5f, viewHeight * 0.5f);
```

App open ad
```csharp
var result = await manager.ShowAppOpenAd();
if (result == EE.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```

Interstitial ad
```csharp
var result = await manager.ShowInterstitialAd();
if (result == EE.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```

Rewarded interstitial ad
```csharp
var result = await manager.ShowRewardedInterstitialAd();
if (result == EE.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```

Rewarded ad
```csharp
var result = await manager.ShowRewardedAd();
if (result == EE.AdResult.Completed) {
    // Completed.
} else {
    // Failed.
}
```