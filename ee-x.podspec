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
    
    s.exclude_files =
      'src/ee/core/CMakeLists.txt'

    s.xcconfig = {
      'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17'
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

    s.exclude_files =
      'src/ee/crashlytics/CMakeLists.txt'

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
    
    s.exclude_files =
      'src/ee/google/CMakeLists.txt'
    
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

    s.exclude_files =
      'src/ee/firebase/CMakeLists.txt'

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

    s.exclude_files =
      'src/ee/ads/CMakeLists.txt'

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
    
    s.exclude_files =
      'src/ee/admob/CMakeLists.txt'

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
    
    s.exclude_files =
      'src/ee/applovin/CMakeLists.txt'

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

    s.exclude_files =
      'src/ee/facebookads/CMakeLists.txt'
      
    s.dependency 'ee-x/ads'
  end

  spec.subspec 'facebook-ads' do |s|
   s.dependency 'ee-x/facebook-ads-base'
    s.dependency 'FBAudienceNetwork', '4.28.1'
    s.dependency 'GoogleMobileAdsMediationFacebook'
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
      'src/ee/ironsource/*.{h,m,hpp,cpp}',
      'src/ee/ironsource/internal/*.{hpp,cpp}'

    s.public_header_files =
      'src/ee/IronSource.hpp',
      'src/ee/IronSourceFwd.hpp',
      'src/ee/ironsource/*.{h,hpp}'

    s.dependency 'ee-x/ads'
    s.dependency 'IronSourceSDK', '6.7.12.0'
  end

  spec.subspec 'ironsource-mediation' do |s|
    s.dependency 'ee-x/ironsource'
    s.dependency 'ee-x/ironsource-mediation-base'
    s.dependency 'IronSourceAppLovinAdapter','4.1.4.3'
    s.dependency 'IronSourceFacebookAdapter','4.1.3.4'
    s.dependency 'IronSourceUnityAdsAdapter','4.1.1.3'
    s.dependency 'IronSourceVungleAdapter','4.1.5.6'
    s.dependency 'IronSourceAdColonyAdapter','4.1.2.1'
    s.dependency 'IronSourceAdMobAdapter','4.1.5.0'
  end

  spec.subspec 'unity-ads-base' do |s|
    s.source_files =
      'src/ee/UnityAds.hpp',
      'src/ee/UnityAdsFwd.hpp',
      'src/ee/unityads/*.{h,hpp,m,cpp}',
      'src/ee/unityads/internal/*.{hpp,cpp}'

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
      'src/ee/vungle/*.{h,m,hpp,cpp}',
      'src/ee/vungle/internal/*.{hpp,cpp}'

    s.public_header_files =
      'src/ee/Vungle.hpp',
      'src/ee/VungleFwd.hpp',
      'src/ee/vungle/*.{h,hpp}'

    s.dependency 'ee-x/ads'
  end

  spec.subspec 'vungle' do |s|
    s.dependency 'ee-x/vungle-base'
    s.dependency 'VungleSDK-iOS', '6.2.0'
    s.dependency 'GoogleMobileAdsMediationVungle'
  end

  spec.subspec 'vungle-mediation' do |s|
    s.dependency 'ee-x/vungle-base'
    s.dependency 'ee-x/ironsource-mediation-base'
    s.dependency 'IronSourceVungleAdapter','4.1.5.6'
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
  
  spec.subspec 'cocos' do |s|
    s.source_files = 
      'src/ee/cocos/*.{hpp,cpp}'
    s.public_header_files = 
      'src/ee/cocos/*.hpp'
    s.xcconfig =  {
      'HEADER_SEARCH_PATHS' => [
        '${PODS_ROOT}/../../cocos2d ',
        '${PODS_ROOT}/../../cocos2d/cocos ',
        '${PODS_ROOT}/../../cocos2d/cocos/editor-support ',
        '${PODS_ROOT}/../../cocos2d/external ',
        '${PODS_ROOT}/../../cocos2d/extensions'
      ].join(' ')
    }
  end

  spec.subspec 'jsb-core' do |s|
    s.source_files =
      'src/ee/jsb/jsb_core.hpp',
      'src/ee/jsb/jsb_core.cpp',
      'src/ee/jsb/jsb_fwd.hpp',
      'src/ee/jsb/core/*'
    s.public_header_files =
      'src/ee/jsb/jsb_fwd.hpp',
      'src/ee/jsb/jsb_core.hpp',
      'src/ee/jsb/core/*.{h,hpp}'

    s.xcconfig = {
      'HEADER_SEARCH_PATHS' => [
        '${PODS_ROOT}/../../../cocos2d-x',
        '${PODS_ROOT}/../../../cocos2d-x/cocos',
        '${PODS_ROOT}/../../../cocos2d-x/cocos/editor-support',
        '${PODS_ROOT}/../../../cocos2d-x/external/sources'
        ].join(' ')
    }
    
    s.dependency 'ee-x/core'
  end

  spec.subspec 'jansson' do |s| 
    s.header_mappings_dir = 'third_party/jansson'
    s.public_header_files = 'third_party/jansson/*.h'
    s.source_files = 'third_party/jansson/*.{h,c}'
  end

  spec.subspec 'jsb-ads' do |s|
    s.source_files =
      'src/ee/jsb/jsb_ads.hpp',
      'src/ee/jsb/jsb_ads.cpp',
      'src/ee/jsb/ads/*'
    s.public_header_files =
      'src/ee/jsb/jsb_ads.hpp',
      'src/ee/jsb/ads/*.{h,hpp}'

    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/ads'
  end

  spec.subspec 'jsb-admob' do |s|
    s.source_files =
      'src/ee/jsb/jsb_admob.hpp',
      'src/ee/jsb/jsb_admob_fwd.hpp',
      'src/ee/jsb/jsb_admob.cpp',
      'src/ee/jsb/admob/*'
    s.public_header_files =
      'src/ee/jsb/jsb_admob.hpp',
      'src/ee/jsb/admob/*.{h,hpp}'

    s.dependency 'ee-x/jsb-ads'
    s.dependency 'ee-x/admob'
  end

  spec.subspec 'jsb-crashlytics' do |s|
    s.source_files =
      'src/ee/jsb/jsb_crashlytics.hpp',
      'src/ee/jsb/jsb_crashlytics.cpp',
      'src/ee/jsb/crashlytics/*'
    s.public_header_files =
      'src/ee/jsb/jsb_crashlytics.hpp',
      'src/ee/jsb/crashlytics/*.{h,hpp}'
    
    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/crashlytics'
  end

  spec.subspec 'jsb-google-analytics' do |s|
    s.source_files =
      'src/ee/jsb/jsb_google_analytics.hpp',
      'src/ee/jsb/jsb_google_analytics.cpp',
      'src/ee/jsb/google/*'
    s.public_header_files =
      'src/ee/jsb/jsb_google_analytics.hpp',
      'src/ee/jsb/google/*.{h,hpp}'
    
    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/google-analytics'
  end
  
  spec.subspec 'jsb-soomla-store' do |s|
    s.source_files =
      'src/ee/jsb/jsb_soomla.hpp',
      'src/ee/jsb/jsb_soomla.cpp',
      'src/ee/jsb/soomla/*'
    s.public_header_files =
      'src/ee/jsb/jsb_soomla.hpp',
      'src/ee/jsb/soomla/*.{h,hpp}'

    s.dependency 'ee-x/soomla-cocos2dx-store'
    s.dependency 'ee-x/jsb-core'
  end

  spec.subspec 'keeva' do |s|
    s.header_mappings_dir = 'third_party/keeva'
    s.public_header_files = 'third_party/keeva/*.h'
    s.source_files = 'third_party/keeva/*.{h,m}'
  end

  spec.subspec 'soomla-ios-core' do |s|
    s.header_mappings_dir = 
      'third_party/soomla/SoomlaiOSCore'
    s.public_header_files = 
      'third_party/soomla/SoomlaiOSCore/**/*.h'
    s.source_files  = 
      'third_party/soomla/SoomlaiOSCore/**/*.{h,m}'

    s.dependency 'ee-x/keeva'
  end

  spec.subspec 'soomla-cocos2dx-core' do |s|
    s.header_mappings_dir = 
      'third_party/soomla/SoomlaCocos2dxCore'
    s.public_header_files = 
      'third_party/soomla/SoomlaCocos2dxCore/**/*.h'
    s.source_files  = 
      'third_party/soomla/SoomlaCocos2dxCore/**/*.{h,mm,cpp}'
    s.exclude_files =
      'third_party/soomla/SoomlaCocos2dxCore/Soomla/jsb/*'

    s.xcconfig = {
      'HEADER_SEARCH_PATHS' => [
        '${PODS_ROOT}/../../cocos2d',
        '${PODS_ROOT}/../../cocos2d/cocos',
        '${PODS_ROOT}/../../../cocos2d-x',
        '${PODS_ROOT}/../../../cocos2d-x/cocos',
        "${PODS_ROOT}/Headers/Public/#{spec.name}/Soomla/**"
        ].join(' ')
    }
    
    s.dependency 'ee-x/keeva'
    s.dependency 'ee-x/jansson'
    s.dependency 'ee-x/soomla-ios-core'
    s.dependency 'ee-x/nlohmann-json'
  end

  spec.subspec 'soomla-ios-store' do |s| 
    s.header_mappings_dir = 
      'third_party/soomla/SoomlaiOSStore'
    s.source_files  = 
      'third_party/soomla/SoomlaiOSStore', 
      'third_party/soomla/SoomlaiOSStore/**/*.{h,m}'
    s.public_header_files  = 
      'third_party/soomla/SoomlaiOSStore', 
      'third_party/soomla/SoomlaiOSStore/**/*.h'

    s.dependency 'ee-x/keeva'
    s.dependency 'ee-x/soomla-ios-core'
  end

  spec.subspec 'soomla-cocos2dx-store' do |s| 
    s.header_mappings_dir = 
      'third_party/soomla/SoomlaCocos2dxStore'
    s.source_files = 
      'third_party/soomla/SoomlaCocos2dxStore/**/*.{h,mm,cpp}'
    s.public_header_files = 
      'third_party/soomla/SoomlaCocos2dxStore/**/*.h'
    s.xcconfig = {
      'HEADER_SEARCH_PATHS' => [
        '${PODS_ROOT}/../../cocos2d',
        '${PODS_ROOT}/../../cocos2d/cocos',
        '${PODS_ROOT}/../../../cocos2d-x',
        '${PODS_ROOT}/../../../cocos2d-x/cocos',
        ].join(' ')
    }
    s.dependency 'ee-x/soomla-cocos2dx-core'
    s.dependency 'ee-x/soomla-ios-store'
      'src/ee/jsb/facebookads/*.{h,hpp}'
  end

  spec.subspec 'jsb-facebook-ads' do |s|
    s.source_files =
      'src/ee/jsb/jsb_facebook_ads.hpp',
      'src/ee/jsb/jsb_facebook_ads_fwd.hpp',
      'src/ee/jsb/jsb_facebook_ads.cpp',
      'src/ee/jsb/facebookads/*'
    s.public_header_files =
      'src/ee/jsb/jsb_facebook_ads.hpp',
      'src/ee/jsb/facebookads/*.{h,hpp}'
    
    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/facebook-ads'
  end

  spec.subspec 'jsb-firebase' do |s|
    s.source_files =
      'src/ee/jsb/jsb_firebase.hpp',
      'src/ee/jsb/jsb_firebase.cpp',
      'src/ee/jsb/firebase/*'
    s.public_header_files =
      'src/ee/jsb/jsb_firebase.hpp',
      'src/ee/jsb/firebase/*.{h,hpp}'
    
    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/firebase-analytics'
    s.dependency 'ee-x/firebase-remote-config'
  end
  
  spec.subspec 'jsb-notification' do |s|
    s.platform = :ios
    s.source_files =
      'src/ee/jsb/jsb_notification.hpp',
      'src/ee/jsb/jsb_notification.cpp',
      'src/ee/jsb/notification/*'
    s.public_header_files =
      'src/ee/jsb/jsb_notification.hpp',
      'src/ee/jsb/notification/*.{h,hpp}'
    
    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/notification'
  end
  
  spec.subspec 'jsb-unity-ads' do |s|
    s.source_files =
      'src/ee/jsb/jsb_unity_ads.hpp',
      'src/ee/jsb/jsb_unity_ads.cpp',
      'src/ee/jsb/unityads/*'
    s.public_header_files =
      'src/ee/jsb/jsb_unity_ads.hpp',
      'src/ee/jsb/unityads/*.{h,hpp}'
    
    s.dependency 'ee-x/jsb-ads'
    s.dependency 'ee-x/unity-ads'
  end

  spec.subspec 'jsb-ironsource' do |s|
    s.source_files =
      'src/ee/jsb/jsb_ironsource.hpp',
      'src/ee/jsb/jsb_ironsource.cpp',
      'src/ee/jsb/ironsource/*'
    s.public_header_files =
      'src/ee/jsb/jsb_ironsource.hpp',
      'src/ee/jsb/ironsource/*.{h,hpp}'
    
    s.dependency 'ee-x/jsb-ads'
    s.dependency 'ee-x/ironsource'
  end

  spec.subspec 'jsb-vungle' do |s|
    s.source_files =
      'src/ee/jsb/jsb_vungle.hpp',
      'src/ee/jsb/jsb_vungle.cpp',
      'src/ee/jsb/vungle/*'
    s.public_header_files =
      'src/ee/jsb/jsb_vungle.hpp',
      'src/ee/jsb/vungle/*.{h,hpp}'
    
    s.dependency 'ee-x/jsb-ads'
    s.dependency 'ee-x/vungle'
  end

end

