Pod::Spec.new do |spec|
  spec.name           = 'ee-x'
  spec.version        = '0.1.4'
  spec.summary        = 'ee-x'
  spec.description    = 'ee-x'

  spec.homepage       = 'https://github.com/enrevol/ee-x'

  # spec.license        = { :type => 'MIT', :file => 'FILE_LICENSE' }
  spec.author         = 'Hai Hoang'

  spec.ios.deployment_target = '7.0'
  spec.osx.deployment_target = '10.7'

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
      'src/ee/core/*.{cpp,h,hpp,m,mm}',
      'src/ee/core/**/*.{cpp,h,hpp,m,mm}'

    s.public_header_files =
      'src/ee/Macro.hpp',
      'src/ee/Core.hpp',
      'src/ee/core/Logger.hpp',
      'src/ee/core/LogLevel.hpp',
      'src/ee/core/EEPluginManager.h',
      'src/ee/core/Metrics.hpp',
      'src/ee/core/EEMetrics.h',
      'src/ee/core/Utils.hpp'

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

    s.ios.vendored_library =  'third_party/firebase_cpp_sdk/libs/ios/universal/libapp.a'
    s.dependency 'ee-x/core'
    s.ios.dependency 'ee-x/firebase-headers'
    s.ios.dependency 'Firebase/Core', '4.0.1'
  end

  spec.subspec 'firebase-analytics' do |s|
    s.source_files = 'src/ee/firebase/FirebaseAnalytics.*'
    s.public_header_files = 'src/ee/firebase/FirebaseAnalytics.hpp'
    s.ios.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libanalytics.a'
    s.dependency 'ee-x/firebase-core'
  end

  spec.subspec 'firebase-messaging' do |s|
    s.source_files = 'src/ee/firebase/FirebaseMessaging.*'
    s.public_header_files = 'src/ee/firebase/FirebaseMessaging.hpp'
    s.ios.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libmessaging.a'
    s.dependency 'ee-x/firebase-core'
    s.ios.dependency 'Firebase/Messaging', '4.0.1'
  end

  spec.subspec 'firebase-remote-config' do |s|
    s.source_files = 'src/ee/firebase/FirebaseRemoteConfig.*'
    s.public_header_files = 'src/ee/firebase/FirebaseRemoteConfig.hpp'
    s.ios.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libremote_config.a'
    s.dependency 'ee-x/firebase-core'
    s.ios.dependency 'Firebase/RemoteConfig', '4.0.1'
  end

  spec.subspec 'firebase-storage' do |s|
    s.source_files = 'src/ee/firebase/FirebaseStorage.*'
    s.public_header_files = 'src/ee/firebase/FirebaseStorage.hpp'
    s.ios.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libstorage.a'
    s.dependency 'ee-x/firebase-core'
    s.ios.dependency 'Firebase/Storage', '4.0.1'
  end

  spec.subspec 'firebase' do |s|
    s.dependency 'ee-x/firebase-analytics'
    s.dependency 'ee-x/firebase-messaging'
    s.dependency 'ee-x/firebase-remote-config'
    s.dependency 'ee-x/firebase-storage'
  end

  spec.subspec 'ads' do |s|
    s.source_files =
      'src/ee/Ads.hpp',
      'src/ee/ads/*.{cpp,hpp,mm,h,m}'
  end

  spec.subspec 'facebook-ads' do |s|
    s.source_files =
      'src/ee/FacebookAds.hpp',
      'src/ee/FacebookAdsFwd.hpp',
      'src/ee/facebook/*.{cpp,hpp,mm,h,m}',
      'src/ee/facebook/internal/*.{h,m}'

    s.public_header_files =
      'src/ee/FacebookAds.hpp',
      'src/ee/FacebookAdsFwd.hpp',
      'src/ee/facebook/FacebookAdsBridge.hpp',
      'src/ee/facebook/EEFacebookNativeAdView.h'

    s.dependency 'ee-x/core'
    s.dependency 'ee-x/ads'
    s.dependency 'FBAudienceNetwork'
  end

  spec.subspec 'unity-ads' do |s|
    s.source_files =
      'src/ee/UnityAds.hpp',
      'src/ee/UnityAdsFwd.hpp',
      'src/ee/unityads/*.{cpp,hpp,mm,h,m}'

    s.public_header_files =
      'src/ee/UnityAds.hpp',
      'src/ee/UnityAdsFwd.hpp',
      'src/ee/unityads/UnityAdsBridge.hpp',
      'src/ee/unityads/UnityRewardedVideo.hpp',
      'src/ee/unityads/UnityInterstitialAd.hpp'

    s.dependency 'ee-x/core'
    s.dependency 'ee-x/ads'

    # UnityAds conflicts with GoogleMobileAdsMediationUnity
    s.dependency 'GoogleMobileAdsMediationUnityAdsSDK'
  end

  spec.subspec 'ironsource' do |s|
    s.source_files =
      'src/ee/IronSource.hpp',
      'src/ee/IronSourceFwd.hpp',
      'src/ee/ironsource/*.{cpp,hpp,mm,h,m}'

    s.public_header_files =
      'src/ee/IronSource.hpp',
      'src/ee/IronSourceFwd.hpp',
      'src/ee/ironsource/IronSourceBridge.hpp'
      'src/ee/ironsource/IronSourceRewardedVideo.hpp'

    s.dependency 'ee-x/core'
    s.dependency 'ee-x/ads'
    s.dependency 'IronSourceSDK'
  end

  spec.subspec 'applovin' do |s|
    s.source_files =
      'src/ee/AppLovin.hpp',
      'src/ee/AppLovinFwd.hpp',
      'src/ee/applovin/*.{cpp,hpp,mm,h,m}'

    s.public_header_files =
      'src/ee/AppLovin.hpp',
      'src/ee/AppLovinFwd.hpp',
      'src/ee/applovin/AppLovinBridge.hpp'

    s.dependency 'ee-x/core'
    s.dependency 'AppLovinSDK'
  end

  spec.subspec 'vungle' do |s|
    s.source_files =
      'src/ee/Vungle.hpp',
      'src/ee/VungleFwd.hpp',
      'src/ee/vungle/*.{cpp,hpp,mm,h,m}'

    s.public_header_files =
      'src/ee/Vungle.hpp',
      'src/ee/VungleFwd.hpp',
      'src/ee/vungle/VungleBridge.hpp',
      'src/ee/vungle/VungleRewardedVideo.hpp'

    s.dependency 'ee-x/core'
    s.dependency 'VungleSDK-iOS', '4.1.0'
  end
  

end
