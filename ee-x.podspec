Pod::Spec.new do |spec|
  spec.name           = 'ee-x'
  spec.version        = '2.10.3'
  spec.summary        = 'ee-x'
  spec.description    = 'Cross-platform library for cocos2d-x'
  spec.module_name    = 'ee'

  spec.homepage       = 'https://github.com/Senspark/ee-x'

  spec.license        = { :type => 'MIT', :file => 'LICENSE' }
  spec.author         = 'Hai Hoang'

  spec.swift_version = '5.0'
  spec.ios.deployment_target = '12.0'
  spec.osx.deployment_target = '10.14'

  spec.source = {
    :git => 'https://github.com/Senspark/ee-x.git',
    :tag => 'v2.10.3'
  }

  spec.framework = 'Foundation'
  spec.static_framework = true

  spec.requires_arc = false

  spec.subspec 'json' do |s|
    s.source_files = 'third_party/nlohmann/include/**/*'
    s.private_header_files = 'third_party/nlohmann/include/**/*'
    s.header_mappings_dir = 'third_party/nlohmann/include'
  end

  spec.subspec 'cpp' do |s|
    s.source_files =
      'src/cpp/ee/Cpp*',
      'src/cpp/ee/ad_colony/**/*',
      'src/cpp/ee/ad_mob/**/*',
      'src/cpp/ee/adjust/**/*',
      'src/cpp/ee/ads/**/*',
      'src/cpp/ee/app_lovin/**/*',
      'src/cpp/ee/apps_flyer/**/*',
      'src/cpp/ee/cocos/**/*',
      'src/cpp/ee/core/**/*',
      'src/cpp/ee/facebook/**/*',
      'src/cpp/ee/facebook_ads/**/*',
      'src/cpp/ee/firebase_analytics/**/*',
      'src/cpp/ee/firebase_crashlytics/**/*',
      'src/cpp/ee/firebase_performance/**/*',
      'src/cpp/ee/firebase_remote_config/**/*',
      'src/cpp/ee/iron_source/**/*',
      'src/cpp/ee/notification/**/*',
      'src/cpp/ee/play/**/*',
      'src/cpp/ee/recorder/**/*',
      'src/cpp/ee/services/**/*',
      'src/cpp/ee/store/**/*',
      'src/cpp/ee/unity_ads/**/*',
      'src/cpp/ee/vungle/**/*'
    
    s.private_header_files =
      'src/cpp/ee/**/internal/*',
      'src/cpp/ee/**/private/*'

    s.exclude_files =
      'src/cpp/ee/core/**/*Android*',
      'src/cpp/ee/core/**/Jni*',
      'src/cpp/ee/**/Android.mk',
      'src/cpp/ee/**/CMakeLists.txt',
      'src/cpp/ee/**/generate.sh',
      'src/cpp/ee/**/sourcelist.cmake'

    s.header_mappings_dir = 'src/cpp'

    s.xcconfig = {
      'CLANG_CXX_LANGUAGE_STANDARD' => 'c++20',
      'OTHER_CPLUSPLUSFLAGS' => '-fcoroutines-ts',
      'GCC_PREPROCESSOR_DEFINITIONS' => [
        'USE_FILE32API' # Used in unzip library in cocos2d-x.
      ].join(' '),
      'GCC_PREPROCESSOR_DEFINITIONS[config=Debug]' => [
        '$(inherited)',
        'COCOS2D_DEBUG=1'
      ].join(' '),
      'GCC_PREPROCESSOR_DEFINITIONS[config=Release]' => [
        '$(inherited)',
        'NDEBUG'
      ].join(' '),
      'HEADER_SEARCH_PATHS' => [
        '${PODS_ROOT}/../../cocos2d',
        '${PODS_ROOT}/../../cocos2d/cocos',
        '${PODS_ROOT}/../../cocos2d/cocos/editor-support',
        '${PODS_ROOT}/../../cocos2d/external',
        '${PODS_ROOT}/../../cocos2d/extensions',

        # For macos.
        '${PODS_ROOT}/../../cocos2d/external/mac/include/glfw3', # Cocos Creator
        '${PODS_ROOT}/../../cocos2d/external/glfw3/include/mac', # Cocos2d-x

        # JS.
        '${PODS_ROOT}/../../../cocos2d-x',
        '${PODS_ROOT}/../../../cocos2d-x/cocos',
        '${PODS_ROOT}/../../../cocos2d-x/external/sources',
        '${PODS_ROOT}/../../../cocos2d-x/external/mac/include/v8'
      ].join(' ')
    }

    s.resources = 'res/*' # For AdMob native ad.
    s.library =
      'iconv',
      'z'
    s.framework = 
      'GameController',
      'OpenAL'
    s.ios.framework =
      'CoreMotion'

    s.dependency 'ee-x/json'
  end

  spec.subspec 'ad-colony' do |s|
    s.source_files = 'src/ios/ee/ad_colony/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/ads'

    # https://github.com/AdColony/AdColony-iOS-SDK
    s.dependency 'AdColony', '4.9.0'
  end

  spec.subspec 'ad-mob' do |s|
    s.source_files = 'src/ios/ee/ad_mob/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/ads'

    # https://developers.google.com/admob/ios/rel-notes
    s.dependency 'Google-Mobile-Ads-SDK', '9.14.0'
  end

  spec.subspec 'ad-mob-mediation' do |s|
    s.dependency 'ee-x/ad-mob-mediation-ad-colony'
    s.dependency 'ee-x/ad-mob-mediation-app-lovin'
    s.dependency 'ee-x/ad-mob-mediation-facebook'
    s.dependency 'ee-x/ad-mob-mediation-inmobi'
    s.dependency 'ee-x/ad-mob-mediation-iron-source'
    s.dependency 'ee-x/ad-mob-mediation-pangle'
    s.dependency 'ee-x/ad-mob-mediation-tapjoy'
    s.dependency 'ee-x/ad-mob-mediation-unity'
    s.dependency 'ee-x/ad-mob-mediation-vungle'
  end

  spec.subspec 'ad-mob-mediation-ad-colony' do |s|
    s.dependency 'ee-x/ad-mob'

    # https://developers.google.com/admob/ios/mediation/adcolony#adcolony-ios-mediation-adapter-changelog
    # FIXME: hanging on iPad iOS 13.5.1, need test.
    s.dependency 'GoogleMobileAdsMediationAdColony', '4.9.0.0'
  end

  spec.subspec 'ad-mob-mediation-app-lovin' do |s|
    s.dependency 'ee-x/ad-mob'

    # https://cocoapods.org/pods/GoogleMobileAdsMediationAppLovin
    # https://developers.google.com/admob/ios/mediation/applovin#applovin-ios-mediation-adapter-changelog
    s.dependency 'GoogleMobileAdsMediationAppLovin', '11.6.1.0'
  end

  spec.subspec 'ad-mob-mediation-facebook' do |s|
    # https://cocoapods.org/pods/GoogleMobileAdsMediationFacebook
    # https://developers.google.com/admob/ios/mediation/facebook#facebook-ios-mediation-adapter-changelog
    s.dependency 'GoogleMobileAdsMediationFacebook', '6.12.0.0'
  end

  spec.subspec 'ad-mob-mediation-inmobi' do |s|
    s.dependency 'ee-x/ad-mob'

    # https://developers.google.com/admob/ios/mediation/inmobi#inmobi-ios-mediation-adapter-changelog
    s.dependency 'GoogleMobileAdsMediationInMobi', '10.1.2.1'
  end

  spec.subspec 'ad-mob-mediation-iron-source' do |s|
    s.dependency 'ee-x/ad-mob'
    
    # https://developers.google.com/admob/ios/mediation/ironsource#ironsource-ios-mediation-adapter-changelog
    s.dependency 'GoogleMobileAdsMediationIronSource', '7.2.7.0.0'
  end

  spec.subspec 'ad-mob-mediation-pangle' do |s|
    s.dependency 'ee-x/ad-mob'

    # https://developers.google.com/admob/ios/mediation/pangle#pangle-ios-mediation-adapter-changelog
    s.dependency 'GoogleMobileAdsMediationPangle', '4.9.0.8.0'
  end

  spec.subspec 'ad-mob-mediation-tapjoy' do |s|
    s.dependency 'ee-x/ad-mob'

    # https://cocoapods.org/pods/GoogleMobileAdsMediationTapjoy
    # https://developers.google.com/admob/ios/mediation/tapjoy#tapjoy-ios-mediation-adapter-changelog
    s.dependency 'GoogleMobileAdsMediationTapjoy', '12.11.0.0'
  end

  spec.subspec 'ad-mob-mediation-unity' do |s|
    s.dependency 'ee-x/ad-mob'

    # https://cocoapods.org/pods/GoogleMobileAdsMediationUnity
    # https://developers.google.com/admob/ios/mediation/unity#unity-ads-ios-mediation-adapter-changelog
    s.dependency 'GoogleMobileAdsMediationUnity', '4.5.0.0'
  end

  spec.subspec 'ad-mob-mediation-vungle' do |s|
    s.dependency 'ee-x/ad-mob'

    # https://cocoapods.org/pods/GoogleMobileAdsMediationVungle
    # https://developers.google.com/admob/ios/mediation/vungle#vungle-ios-mediation-adapter-changelog
    s.dependency 'GoogleMobileAdsMediationVungle', '6.12.1.1'
  end

  spec.subspec 'ads' do |s|
    s.source_files = 'src/ios/ee/ads/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'
  end

  spec.subspec 'adjust' do |s|
    s.source_files = 'src/ios/ee/adjust/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'

    # https://github.com/adjust/ios_sdk
    s.dependency 'Adjust', '4.33.0'
  end

  spec.subspec 'app-lovin' do |s|
    s.source_files = 'src/ios/ee/app_lovin/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'

    # https://github.com/AppLovin/AppLovin-MAX-SDK-iOS/releases
    s.dependency 'AppLovinSDK', '11.6.1'
  end

  spec.subspec 'apps-flyer' do |s|
    s.source_files = 'src/ios/ee/apps_flyer/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'

    # https://cocoapods.org/pods/AppsFlyerFramework
    s.dependency 'AppsFlyerFramework', '6.12.2'
    s.dependency 'AppsFlyer-AdRevenue'
    s.dependency 'PurchaseConnector', '6.12.2'
  end

  spec.subspec 'core' do |s|
    s.source_files = 'src/ios/ee/core/**/*'
    s.header_mappings_dir = 'src/ios/ee/core' # Must be non-path headers to fix umbrella.h
    s.user_target_xcconfig = {
      'LD_RUNPATH_SEARCH_PATHS' => [
        '/usr/lib/swift',
        # Fix: dyld: Library not loaded: @rpath/libswiftCore.dylib
        '@executable_path/Frameworks',
        '@loader_path/Frameworks'
      ].join(' '),
      'LIBRARY_SEARCH_PATHS' => [
        '$(TOOLCHAIN_DIR)/usr/lib/swift/$(PLATFORM_NAME)',
        # '$(TOOLCHAIN_DIR)/usr/lib/swift-5.0/$(PLATFORM_NAME)', Must comment to fix Swift._ArrayBuffer._copyContents (XCode 12.5)
        '$(SDKROOT)/usr/lib/swift' # Fix __swift_FORCE_LOAD_$_swiftCoreMIDI (XCode 12).
      ].join(' '),
      # https://forums.swift.org/t/undefined-symbol-swift-getfunctionreplacement/30495
      'DEAD_CODE_STRIPPING' => 'YES'
    }
    s.framework = 'AppTrackingTransparency'
    s.library = 'swiftCore'

    # https://cocoapods.org/pods/ReachabilitySwift
    s.dependency 'ReachabilitySwift', '5.0.0'

    # https://cocoapods.org/pods/RxSwift
    s.dependency 'RxSwift', '5.1.1'
  end

  spec.subspec 'facebook' do |s|
    s.source_files = 'src/ios/ee/facebook/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'

    # https://github.com/facebook/facebook-ios-sdk/releases
    s.dependency 'FBSDKCoreKit', '13.2.0'
    s.dependency 'FBSDKLoginKit', '13.2.0'
    s.dependency 'FBSDKShareKit', '13.2.0'
  end

  spec.subspec 'facebook-ads' do |s|
    s.source_files = 'src/ios/ee/facebook_ads/**/*'
    s.header_mappings_dir = 'src/ios/ee/facebook_ads' # Must be non-path headers to fix umbrella.h
    s.dependency 'ee-x/ads'

    # https://developers.facebook.com/docs/audience-network/changelog-ios/
    s.dependency 'FBAudienceNetwork', '6.12.0'
  end

  spec.subspec 'firebase-core' do |s|
    s.source_files = 'src/ios/ee/firebase_core/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'

    # https://cocoapods.org/pods/FirebaseCore
    s.dependency 'Firebase/Core', '9.6.0'
  end

  spec.subspec 'firebase-analytics' do |s|
    s.source_files = 'src/ios/ee/firebase_analytics/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/firebase-core'

    # https://cocoapods.org/pods/FirebaseAnalytics
    s.dependency 'Firebase/Analytics', '9.6.0'
  end

  spec.subspec 'firebase-crashlytics' do |s|
    s.source_files = 'src/ios/ee/firebase_crashlytics/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/firebase-core'

    # https://cocoapods.org/pods/FirebaseCrashlytics
    s.dependency 'Firebase/Crashlytics', '9.6.0'
  end

  spec.subspec 'firebase-performance' do |s|
    s.source_files = 'src/ios/ee/firebase_performance/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/firebase-core'

    # https://cocoapods.org/pods/FirebasePerformance
    s.dependency 'Firebase/Performance', '9.6.0'
  end

  spec.subspec 'firebase-remote-config' do |s|
    s.source_files = 'src/ios/ee/firebase_remote_config/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/firebase-core'

    # https://cocoapods.org/pods/FirebaseRemoteConfig
    s.dependency 'Firebase/RemoteConfig', '9.6.0'
  end

  spec.subspec 'iron-source-private' do |s|
    s.source_files = 'src/ios/ee/iron_source/private/*'
    s.header_mappings_dir = 'src/ios/ee/iron_source/private' # Must be non-path headers to fix umbrella.h
  end

  spec.subspec 'iron-source' do |s|
    s.source_files = 'src/ios/ee/iron_source/**'
    s.private_header_files = 'src/ios/ee/iron_source/*.h' # Don't include in umbrella.h
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/ads'
    s.dependency 'ee-x/iron-source-private'

    # https://developers.ironsrc.com/ironsource-mobile/ios/sdk-change-log/
    s.dependency 'IronSourceSDK', '7.2.7.0'
  end

  spec.subspec 'iron-source-mediation' do |s|
    s.dependency 'ee-x/iron-source-mediation-ad-colony'
    s.dependency 'ee-x/iron-source-mediation-ad-mob'
    s.dependency 'ee-x/iron-source-mediation-app-lovin'
    s.dependency 'ee-x/iron-source-mediation-facebook'
    s.dependency 'ee-x/iron-source-mediation-inmobi'
    s.dependency 'ee-x/iron-source-mediation-pangle'
    s.dependency 'ee-x/iron-source-mediation-tapjoy'
    s.dependency 'ee-x/iron-source-mediation-unity'
    s.dependency 'ee-x/iron-source-mediation-vungle'
  end

  spec.subspec 'iron-source-mediation-ad-colony' do |s|
    s.dependency 'ee-x/iron-source'

    # https://cocoapods.org/pods/IronSourceAdColonyAdapter
    # https://developers.is.com/ironsource-mobile/ios/adcolony-change-log/
    s.dependency 'IronSourceAdColonyAdapter', '4.3.15.4'
  end

  spec.subspec 'iron-source-mediation-ad-mob' do |s|
    s.dependency 'ee-x/iron-source'

    # https://cocoapods.org/pods/IronSourceAdMobAdapter
    # https://developers.is.com/ironsource-mobile/ios/admob-change-log/
    s.dependency 'IronSourceAdMobAdapter', '4.3.40.0'
  end

  spec.subspec 'iron-source-mediation-app-lovin' do |s|
    s.dependency 'ee-x/iron-source'

    # https://cocoapods.org/pods/IronSourceAppLovinAdapter
    # https://developers.is.com/ironsource-mobile/ios/applovin-change-log/
    s.dependency 'IronSourceAppLovinAdapter', '4.3.37.1'
  end

  spec.subspec 'iron-source-mediation-facebook' do |s|
    s.dependency 'ee-x/iron-source'

    # https://cocoapods.org/pods/IronSourceFacebookAdapter
    # https://developers.is.com/ironsource-mobile/ios/meta-audience-network-change-log/
    s.dependency 'IronSourceFacebookAdapter', '4.3.38.4'
  end

  spec.subspec 'iron-source-mediation-inmobi' do |s|
    s.dependency 'ee-x/iron-source'

    # https://cocoapods.org/pods/IronSourceInMobiAdapter
    # https://developers.is.com/ironsource-mobile/ios/inmobi-change-log/
    s.dependency 'IronSourceInMobiAdapter', '4.3.16.0'
  end

  spec.subspec 'iron-source-mediation-pangle' do |s|
    s.dependency 'ee-x/iron-source'

    # https://cocoapods.org/pods/IronSourcePangleAdapter
    # https://github.com/CocoaPods/Specs/tree/master/Specs/0/c/4/IronSourcePangleAdapter
    # https://developers.is.com/ironsource-mobile/ios/pangle-change-log/
    # FIXME: conflict with admob.
    # s.dependency 'IronSourcePangleAdapter', '4.3.18.0'
  end

  spec.subspec 'iron-source-mediation-tapjoy' do |s|
    s.dependency 'ee-x/iron-source'

    # https://cocoapods.org/pods/IronSourceTapjoyAdapter
    # https://developers.is.com/ironsource-mobile/ios/tapjoy-change-log/
    # FIXME: conflict with admob.
    # s.dependency 'IronSourceTapjoyAdapter', '4.1.23.1'
  end

  spec.subspec 'iron-source-mediation-unity' do |s|
    s.dependency 'ee-x/iron-source'

    # https://cocoapods.org/pods/IronSourceUnityAdsAdapter
    # https://developers.is.com/ironsource-mobile/ios/unityads-change-log/
    # FIXME: conflict with admob.
    # s.dependency 'IronSourceUnityAdsAdapter', '4.3.25.2'
  end

  spec.subspec 'iron-source-mediation-vungle' do |s|
    s.dependency 'ee-x/iron-source'

    # https://cocoapods.org/pods/IronSourceVungleAdapter
    # https://developers.is.com/ironsource-mobile/ios/vungle-change-log/
    s.dependency 'IronSourceVungleAdapter', '4.3.24.0'
  end

  spec.subspec 'notification' do |s|
    s.source_files = 'src/ios/ee/notification/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'
  end

  spec.subspec 'play' do |s|
    s.source_files = 'src/ios/ee/play/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'
  end

  spec.subspec 'recorder' do |s|
    s.source_files = 'src/ios/ee/recorder/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'
  end

  spec.subspec 'store' do |s|
    s.source_files = 'src/ios/ee/store/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'

    # https://cocoapods.org/pods/TPInAppReceipt
    s.dependency 'TPInAppReceipt', '3.3.4'
  end

  spec.subspec 'unity-ads' do |s|
    s.source_files = 'src/ios/ee/unity_ads/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/ads'

    # https://github.com/Unity-Technologies/unity-ads-ios/releases
    s.dependency 'UnityAds', '4.5.0'
  end

  spec.subspec 'vungle' do |s|
    s.source_files = 'src/ios/ee/vungle/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/ads'

    # https://github.com/Vungle/iOS-SDK/blob/master/CHANGELOG.md
    s.dependency 'VungleSDK-iOS', '6.12.1'
  end

=begin
  # Fix duplicated UUID since there are many common.h files.
  spec.subspec 'firebase-headers-internal' do |s|
    s.source_files = 'third_party/firebase_cpp_sdk/include/firebase/internal/*'
    s.private_header_files = 'third_party/firebase_cpp_sdk/include/firebase/internal/*'
    s.header_mappings_dir = 'third_party/firebase_cpp_sdk/include'
  end

  spec.subspec 'firebase-headers-storage' do |s|
    s.source_files = 'third_party/firebase_cpp_sdk/include/firebase/storage/*'
    s.private_header_files = 'third_party/firebase_cpp_sdk/include/firebase/storage/*'
    s.header_mappings_dir = 'third_party/firebase_cpp_sdk/include'
  end

  spec.subspec 'firebase-headers' do |s|
    s.source_files = 'third_party/firebase_cpp_sdk/include/firebase/**/*'

      # Don't use source_files: all dirs may be included in header map.
      # Conflict google_play_services/availability.h and macOS Availability.h
      # 'third_party/firebase_cpp_sdk/include/google_play_services/availability.h'

    s.private_header_files = 'third_party/firebase_cpp_sdk/include/firebase/**/*'

    s.exclude_files =
      'third_party/firebase_cpp_sdk/include/firebase/auth/*',
      'third_party/firebase_cpp_sdk/include/firebase/database/*',
      'third_party/firebase_cpp_sdk/include/firebase/functions/*',
      'third_party/firebase_cpp_sdk/include/firebase/internal/*',
      'third_party/firebase_cpp_sdk/include/firebase/storage/*'

    s.header_mappings_dir = 'third_party/firebase_cpp_sdk/include'
    s.platform = :ios
    s.dependency 'ee-x/firebase-headers-internal'
    s.dependency 'ee-x/firebase-headers-storage'
  end

  spec.subspec 'firebase-core' do |s|
    s.source_files =
      'src/cpp/ee/Firebase*',
      'src/cpp/ee/firebase_core/**/*'

    s.private_header_files =
      'src/cpp/ee/firebase_core/private/*.{h,hpp,inl}'

    s.exclude_files =
      'src/cpp/ee/firebase_core/Android.mk',
      'src/cpp/ee/firebase_core/CMakeLists.txt',
      'src/cpp/ee/firebase_core/generate.sh',
      'src/cpp/ee/firebase_core/sourcelist.cmake',
      'src/cpp/ee/firebase_core/*Android*'

    s.header_mappings_dir = 'src/cpp'

    s.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_app.a'
    s.dependency 'ee-x/core'
    s.dependency 'ee-x/firebase-x-core'
    s.dependency 'ee-x/firebase-headers'
  end

  spec.subspec 'firebase-dynamic-link' do |s|
    s.source_files = 'src/cpp/ee/firebase_dynamic_link/*'

    s.exclude_files =
      'src/cpp/ee/firebase_dynamic_link/Android.mk',
      'src/cpp/ee/firebase_dynamic_link/CMakeLists.txt',
      'src/cpp/ee/firebase_dynamic_link/generate.sh',
      'src/cpp/ee/firebase_dynamic_link/sourcelist.cmake'

    s.header_mappings_dir = 'src/cpp'

    s.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_dynamic_links.a'
    s.dependency 'ee-x/firebase-core'
    s.dependency 'Firebase/DynamicLinks', '7.5.0'
  end

  spec.subspec 'firebase-messaging' do |s|
    s.source_files = 'src/cpp/ee/firebase_messaging/*'

    s.exclude_files =
      'src/cpp/ee/firebase_messaging/Android.mk',
      'src/cpp/ee/firebase_messaging/CMakeLists.txt',
      'src/cpp/ee/firebase_messaging/generate.sh',
      'src/cpp/ee/firebase_messaging/sourcelist.cmake'
    
    s.header_mappings_dir = 'src/cpp'

    s.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_messaging.a'
    s.dependency 'ee-x/firebase-core'
    s.dependency 'Firebase/Messaging', '7.5.0'
  end

  spec.subspec 'firebase-storage' do |s|
    s.source_files = 'src/cpp/ee/firebase/storage/*'

    s.exclude_files =
      'src/cpp/ee/firebase_storage/Android.mk',
      'src/cpp/ee/firebase_storage/CMakeLists.txt',
      'src/cpp/ee/firebase_storage/generate.sh',
      'src/cpp/ee/firebase_storage/sourcelist.cmake'

    s.header_mappings_dir = 'src/cpp'

    s.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_storage.a'
    s.dependency 'ee-x/firebase-core'
    s.dependency 'Firebase/Storage', '7.5.0'
  end
=end

  spec.subspec 'jansson' do |s|
    s.source_files = 'third_party/jansson/src/**/*'
    s.private_header_files = 'third_party/jansson/src/**/*.{h,hpp,inl}'
    s.header_mappings_dir = 'third_party/jansson/src'
  end

  spec.subspec 'keeva' do |s|
    s.source_files = 'third_party/keeva/src/**/*'
    s.private_header_files = 'third_party/keeva/src/**/*.{h,hpp,inl}'
    s.header_mappings_dir = 'third_party/keeva/src'
  end

  spec.subspec 'soomla-core-ios' do |s|
    s.source_files = 'third_party/soomla/core/src/ios/**/*'
    s.private_header_files = 'third_party/soomla/core/src/ios/**/*.{h,hpp,inl}'
    s.header_mappings_dir = 'third_party/soomla/core/src'
    s.dependency 'ee-x/keeva'
  end

  spec.subspec 'soomla-core-internal' do |s|
    s.source_files = 'third_party/soomla/core/src/soomla/**/*'
    s.header_mappings_dir = 'third_party/soomla/core/src'    
    s.dependency 'ee-x/json'
    s.dependency 'ee-x/jansson'
    s.dependency 'ee-x/cpp'
  end

  spec.subspec 'soomla-store-ios' do |s|
    s.source_files = 'third_party/soomla/store/src/ios/**/*'
    s.private_header_files = 'third_party/soomla/store/src/ios/**/*.{h,hpp,inl}'
    s.header_mappings_dir = 'third_party/soomla/store/src'
    s.dependency 'ee-x/soomla-core-ios'
  end

  spec.subspec 'soomla-store-internal' do |s|
    s.source_files = 'third_party/soomla/store/src/soomla/**/*'
    s.header_mappings_dir = 'third_party/soomla/store/src'
    s.dependency 'ee-x/soomla-core-internal'
    s.ios.dependency 'ee-x/soomla-store-ios'
  end

  spec.subspec 'soomla-store' do |s|
    s.source_files = 'src/cpp/ee/soomla/**/*'
    s.private_header_files = 'src/cpp/ee/**/internal/*'
    s.exclude_files =
      'src/cpp/ee/**/Android.mk',
      'src/cpp/ee/**/CMakeLists.txt',
      'src/cpp/ee/**/generate.sh',
      'src/cpp/ee/**/sourcelist.cmake'
    s.header_mappings_dir = 'src/cpp'
    s.dependency 'ee-x/soomla-store-internal'
  end
end
