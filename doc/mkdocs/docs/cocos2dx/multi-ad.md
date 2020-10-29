# Multi Ad
- Purpose: use ad waterfall to optimize monetization.
- Supported ad:
    + Use `ee::MultiAdView` for banner and native ads.
    + Use `ee::MultiInterstitialAd` for interstitial ads.
    + Use `ee::MultiRewardedAd` for rewarded ads.
- Samples:
```cpp
// Samples for using 4 ad networks:
// - AdMob.
// - Facebook Audience Network.
// - IronSource
// - Unity Ads

auto admob = ee::PluginManager::createPlugin<ee::IAdMob>();
auto facebook = ee::PluginManager::createPlugin<ee::IFacebookAds>();
auto ironsource = ee::PluginManager::createPlugin<ee::IIronSource>();
auto unity = ee::PluginManager::createPlugin<ee::IUnityAds>();

admob->initialize();
ironsource->initialize("app_id");
unity->initialize("app_id", false);

// Create a banner ad.
auto ad = std::make_shared<ee::MultiAdView>();
ad->addItem(admob->createBannerAd("ad_id", ee::AdMobBannerAdSize::Normal));
ad->addItem(facebook->createBannerAd("ad_id", ee::FacebookBannerAdSize::BannerHeight50));

// Load the ad in background.
ee::noAwait(ad->load());

// Show the ad.
ad->setVisible(true);

// Create an interstitial ad.
auto ad = std::make_shared<ee::MultiInterstitialAd>();
ad->addItem(admob->createInterstitialAd("ad_id"));
ad->addItem(facebook->createInterstitialAd("ad_id"));
ad->addItem(ironsource->createInterstitialAd("ad_id"));
ad->addItem(unity->createInterstitialAd("ad_id"));

// Load and show the ad.
ee::noAwait([]() -> ee::Task<> {
    co_await ad->load();
    co_await ad->show();
});

// Create a rewarded ad.
auto ad = std::make_shared<ee::MultiRewardedAd>();
ad->addItem(admob->createRewardedAd("ad_id"));
ad->addItem(facebook->createRewardedAd("ad_id"));
ad->addItem(ironsource->createRewardedAd("ad_id"));
ad->addItem(unity->createRewardedAd("ad_id"));

// Load and show the ad.
ee::noAwait([]() -> ee::Task<> {
    co_await ad->load();
    co_await ad->show();
});
```