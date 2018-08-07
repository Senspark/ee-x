Pod::Spec.new do |spec|
  spec.name           = 'ee-x'
  spec.version        = '0.1.5'
  spec.summary        = 'ee-x'
  spec.description    = 'ee-x'

  spec.homepage       = 'https://github.com/enrevol/ee-x'

  # spec.license        = { :type => 'MIT', :file => 'FILE_LICENSE' }
  spec.author         = 'Hai Hoang'

  spec.ios.deployment_target = '7.0'
  spec.osx.deployment_target = '10.8'

  spec.source = {
    :git => 'https://github.com/enrevol/ee-x.git',
    :branch => 'master'
  }

  spec.frameworks  = 'Foundation'

  spec.requires_arc = false

  spec.header_mappings_dir = 'src'

  spec.subspec 'nlohmann-json' do |s|
    s.preserve_path = 'third_party/nlohmann/include'
    s.public_header_files = 'third_party/nlohmann/include/**/*'
    s.header_mappings_dir = 'third_party/nlohmann/include'
  end

  spec.subspec 'core' do |s|
    s.source_files =
      'src/ee/Macro.hpp',
      'src/ee/Core.hpp',
      'src/ee/CoreFwd.hpp',
      'src/ee/core/*.{cpp,h,hpp,m,mm}',
      'src/ee/core/**/*.{cpp,h,hpp,m,mm}'

    s.public_header_files =
      'src/ee/Macro.hpp',
      'src/ee/Core.hpp',
      'src/ee/CoreFwd.hpp',
      'src/ee/core/*.{h,hpp}'

    s.xcconfig = {
      'CLANG_CXX_LANGUAGE_STANDARD' => 'c++14'
    }

    s.dependency 'ee-x/nlohmann-json'
  end

  spec.subspec 'crashlytics' do |s|
    s.source_files =
      'src/ee/Crashlytics.hpp',
      'src/ee/CrashlyticsFwd.hpp',
      'src/ee/crashlytics/*.{cpp,h,hpp,m}'

    s.public_header_files =
      'src/ee/Crashlytics.hpp',
      'src/ee/CrashlyticsFwd.hpp',
      'src/ee/crashlytics/CrashlyticsBridge.hpp'

    s.dependency 'ee-x/core'
    s.dependency 'Crashlytics'
    s.dependency 'Fabric'
  end
  
  spec.subspec 'campaign-receiver' do |s|
    s.source_files =
      'src/ee/CampaignReceiver.hpp',
      'src/ee/CampaignReceiverFwd.hpp',
      'src/ee/campaignreceiver/*'

    s.public_header_files =
      'src/ee/CampaignReceiver.hpp',
      'src/ee/CampaignReceiverFwd.hpp',
      'src/ee/campaignreceiver/*.{h,hpp}'

    s.dependency 'ee-x/core'   
  end

  spec.subspec 'facebook' do |s|
    s.source_files =
      'src/ee/Facebook.hpp',
      'src/ee/FacebookFwd.hpp',
      'src/ee/facebook/*',
      'src/ee/facebook/internal/*'

    s.public_header_files =
      'src/ee/Facebook.hpp',
      'src/ee/FacebookFwd.hpp',
      'src/ee/facebook/*.{h,hpp}'

    s.dependency 'ee-x/core'
    s.dependency 'FBSDKLoginKit'
    s.dependency 'FBSDKShareKit'    
  end

  spec.subspec 'notification' do |s|
    s.platform = :ios

    s.source_files =
      'src/ee/Notification.hpp',
      'src/ee/NotificationFwd.hpp',
      'src/ee/notification/*.{cpp,h,hpp,m}'

    s.public_header_files =
      'src/ee/Notification.hpp',
      'src/ee/NotificationFwd.hpp',
      'src/ee/notification/NotificationBridge.hpp',
      'src/ee/notification/NotificationBuilder.hpp'

    s.dependency 'ee-x/core'
  end

    spec.subspec 'google-analytics' do |s|
    s.source_files =
      'src/ee/GoogleAnalytics.hpp',
      'src/ee/GoogleAnalyticsFwd.hpp',
      'src/ee/google/*',
      'src/ee/google/internal/*'

    s.public_header_files =
      'src/ee/GoogleAnalytics.hpp',
      'src/ee/GoogleAnalyticsFwd.hpp',
      'src/ee/google/*.{h,hpp}'

    s.dependency 'ee-x/core'
    s.ios.dependency 'GoogleAnalytics'
  end

  spec.subspec 'firebase-headers' do |s|
    s.platform = :ios
    s.preserve_path = 'third_party/firebase_cpp_sdk/include'
    s.public_header_files = 'third_party/firebase_cpp_sdk/include/**/*'
    s.header_mappings_dir = 'third_party/firebase_cpp_sdk/include'
  end

  spec.subspec 'firebase-core' do |s|
    s.source_files =
      'src/ee/Firebase.hpp',
      'src/ee/FirebaseFwd.hpp',
      'src/ee/firebase/FirebaseApp.*',
      'src/ee/firebase/FirebaseScheduler.hpp',
      'src/ee/firebase/FirebaseScheduler_inl.hpp'

    s.public_header_files =
      'src/ee/Firebase.hpp',
      'src/ee/FirebaseFwd.hpp',
      'src/ee/firebase/FirebaseApp.hpp'

    s.ios.vendored_library =  'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_app.a'
    s.dependency 'ee-x/core'
    s.ios.dependency 'ee-x/firebase-headers'
    s.ios.dependency 'Firebase/Core', '4.13.0'
  end

  spec.subspec 'firebase-analytics' do |s|
    s.source_files = 'src/ee/firebase/FirebaseAnalytics.*'
    s.public_header_files = 'src/ee/firebase/FirebaseAnalytics.hpp'
    s.ios.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_analytics.a'
    s.dependency 'ee-x/firebase-core'
  end

  spec.subspec 'firebase-dynamic-link' do |s|
    s.source_files = 'src/ee/firebase/FirebaseDynamicLink.*'
    s.public_header_files = 'src/ee/firebase/FirebaseDynamicLink.hpp'
    s.ios.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_dynamic_links.a'
    s.dependency 'ee-x/firebase-core'
    s.ios.dependency 'Firebase/DynamicLinks', '4.13.0'
  end

  spec.subspec 'firebase-messaging' do |s|
    s.source_files = 'src/ee/firebase/FirebaseMessaging.*'
    s.public_header_files = 'src/ee/firebase/FirebaseMessaging.hpp'
    s.ios.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_messaging.a'
    s.dependency 'ee-x/firebase-core'
    s.ios.dependency 'Firebase/Messaging', '4.13.0'
  end

  spec.subspec 'firebase-remote-config' do |s|
    s.source_files = 'src/ee/firebase/FirebaseRemoteConfig.*'
    s.public_header_files = 'src/ee/firebase/FirebaseRemoteConfig.hpp'
    s.ios.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_remote_config.a'
    s.dependency 'ee-x/firebase-core'
    s.ios.dependency 'Firebase/RemoteConfig', '4.13.0'
  end

  spec.subspec 'firebase-storage' do |s|
    s.source_files = 'src/ee/firebase/FirebaseStorage.*'
    s.public_header_files = 'src/ee/firebase/FirebaseStorage.hpp'
    s.ios.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_storage.a'
    s.dependency 'ee-x/firebase-core'
    s.ios.dependency 'Firebase/Storage', '4.13.0'
  end

  spec.subspec 'ads' do |s|
    s.source_files =
      'src/ee/Ads.hpp',
      'src/ee/AdsFwd.hpp',
      'src/ee/ads/*',
      'src/ee/ads/internal/*'

    s.public_header_files =
      'src/ee/Ads.hpp',
      'src/ee/AdsFwd.hpp',
      'src/ee/ads/*'

    s.dependency 'ee-x/core'
  end

  spec.subspec 'ironsource-mediation-base' do |s|
    s.preserve_path = 'dummy_path'
    s.xcconfig = {
      'GCC_PREPROCESSOR_DEFINITIONS' => [
        'EE_X_USE_IRON_SOURCE_MEDIATION'
      ].join(' ')
    }
  end

  spec.subspec 'admob' do |s|
    s.source_files =
      'src/ee/AdMob.hpp',
      'src/ee/AdMobFwd.hpp',
      'src/ee/admob/*',
      'src/ee/admob/internal/*'

    s.public_header_files =
      'src/ee/AdMob.hpp',
      'src/ee/AdMobFwd.hpp',
      'src/ee/admob/*.{h,hpp}'

    s.resources = 'res/*'
    s.dependency 'ee-x/ads'
    s.dependency 'Google-Mobile-Ads-SDK'
  end

  spec.subspec 'applovin-base' do |s|
    s.source_files =
      'src/ee/AppLovin.hpp',
      'src/ee/AppLovinFwd.hpp',
      'src/ee/applovin/*',
      'src/ee/applovin/internal/*'

    s.public_header_files =
      'src/ee/AppLovin.hpp',
      'src/ee/AppLovinFwd.hpp',
      'src/ee/applovin/*.{h,hpp}'

    s.dependency 'ee-x/ads'
  end

  spec.subspec 'applovin' do |s|
    s.dependency 'ee-x/applovin-base'
    s.dependency 'AppLovinSDK'
  end

  spec.subspec 'applovin-mediation' do |s|
    s.dependency 'ee-x/applovin-base'
    s.dependency 'ee-x/ironsource-mediation-base'
    s.dependency 'IronSourceAppLovinAdapter','4.1.4.3'
  end

  spec.subspec 'facebook-ads-base' do |s|
    s.source_files =
      'src/ee/FacebookAds.hpp',
      'src/ee/FacebookAdsFwd.hpp',
      'src/ee/facebookads/*',
      'src/ee/facebookads/internal/*'

    s.public_header_files =
      'src/ee/FacebookAds.hpp',
      'src/ee/FacebookAdsFwd.hpp',
      'src/ee/facebookads/*.{h,hpp}'

    s.dependency 'ee-x/ads'
  end

  spec.subspec 'facebook-ads' do |s|
    s.dependency 'ee-x/facebook-ads-base'
    s.dependency 'FBAudienceNetwork', '4.17.0'
  end

  spec.subspec 'facebook-ads-mediation' do |s|
    s.public_header_files = 'third_party/facebook-ads-mediation/FBAudienceNetwork/*'
    s.header_mappings_dir = 'third_party/facebook-ads-mediation'
    s.dependency 'ee-x/facebook-ads-base'
    s.dependency 'ee-x/ironsource-mediation-base'
    s.dependency 'IronSourceFacebookAdapter','4.1.3.4'
  end

  spec.subspec 'ironsource' do |s|
    s.source_files =
      'src/ee/IronSource.hpp',
      'src/ee/IronSourceFwd.hpp',
      'src/ee/ironsource/*',
      'src/ee/ironsource/internal/*'

    s.public_header_files =
      'src/ee/IronSource.hpp',
      'src/ee/IronSourceFwd.hpp',
      'src/ee/ironsource/*.{h,hpp}'

    s.dependency 'ee-x/ads'
    s.dependency 'IronSourceSDK', '6.7.9.3'
  end

  spec.subspec 'ironsource-mediation' do |s|
    s.dependency 'ee-x/ironsource'
    s.dependency 'ee-x/ironsource-mediation-base'
    s.dependency 'IronSourceAppLovinAdapter','4.1.4.3'
    s.dependency 'IronSourceFacebookAdapter','4.1.3.4'
    s.dependency 'IronSourceUnityAdsAdapter','4.1.1.3'
    s.dependency 'IronSourceVungleAdapter','4.1.5.3'
    s.dependency 'IronSourceAdColonyAdapter','4.1.2.1'
    s.dependency 'IronSourceAdMobAdapter','4.1.5.0'
  end

  spec.subspec 'unity-ads-base' do |s|
    s.source_files =
      'src/ee/UnityAds.hpp',
      'src/ee/UnityAdsFwd.hpp',
      'src/ee/unityads/*',
      'src/ee/unityads/internal/*'

    s.public_header_files =
      'src/ee/UnityAds.hpp',
      'src/ee/UnityAdsFwd.hpp',
      'src/ee/unityads/*.{h,hpp}'

    s.dependency 'ee-x/ads'
  end

  spec.subspec 'unity-ads' do |s|
    s.dependency 'ee-x/unity-ads-base'

    # UnityAds conflicts with GoogleMobileAdsMediationUnity
    s.dependency 'GoogleMobileAdsMediationUnityAdsSDK'
  end

  spec.subspec 'unity-ads-mediation' do |s|
    s.dependency 'ee-x/unity-ads-base'
    s.dependency 'ee-x/ironsource-mediation-base'
    s.dependency 'IronSourceUnityAdsAdapter','4.1.1.3'
  end

  spec.subspec 'vungle-base' do |s|
    s.source_files =
      'src/ee/Vungle.hpp',
      'src/ee/VungleFwd.hpp',
      'src/ee/vungle/*',
      'src/ee/vungle/internal/*'

    s.public_header_files =
      'src/ee/Vungle.hpp',
      'src/ee/VungleFwd.hpp',
      'src/ee/vungle/*.{h,hpp}'

    s.dependency 'ee-x/ads'
  end

  spec.subspec 'vungle' do |s|
    s.dependency 'ee-x/vungle-base'
    s.dependency 'VungleSDK-iOS', '6.2.0'
  end

  spec.subspec 'vungle-mediation' do |s|
    s.dependency 'ee-x/vungle-base'
    s.dependency 'ee-x/ironsource-mediation-base'
    s.dependency 'IronSourceVungleAdapter','4.1.5.3'
  end

  spec.subspec 'recorder' do |s|
    s.source_files =
      'src/ee/Recorder.hpp',
      'src/ee/RecorderFwd.hpp',
      'src/ee/recorder/*'

    s.public_header_files =
      'src/ee/Recorder.hpp',
      'src/ee/RecorderFwd.hpp',
      'src/ee/recorder/*.{h,hpp}'

    s.dependency 'ee-x/core'
  end

  spec.subspec 'game-center' do |s|
    s.source_files =
      'src/ee/GameCenter.hpp',
      'src/ee/GameCenterFwd.hpp',
      'src/ee/gamecenter/*'
      
    s.public_header_files = 
      'src/ee/GameCenter.hpp',
      'src/ee/GameCenterFwd.hpp',
      'src/ee/gamecenter/*.{h,hpp}'
    s.dependency 'ee-x/core'
  end

  spec.subspec 'appsflyer' do |s|
    s.source_files =
      'src/ee/AppsFlyer.hpp',
      'src/ee/AppsFlyerFwd.hpp',
      'src/ee/appsflyer/*',
      'src/ee/appsflyer/internal/*'

    s.public_header_files =
      'src/ee/AppsFlyer.hpp',
      'src/ee/AppsFlyerFwd.hpp',
      'src/ee/appsflyer/*.{h,hpp}'

    s.dependency 'ee-x/core'
    s.dependency 'AppsFlyerFramework'
  end
end
