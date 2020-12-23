Pod::Spec.new do |spec|
  spec.name           = 'ee-x'
  spec.version        = '2.0.5'
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
    :tag => 'v2.0.5'
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
      'src/cpp/ee/firebase_crashlytics/**/*',
      'src/cpp/ee/firebase_performance/**/*',
      'src/cpp/ee/iron_source/**/*',
      'src/cpp/ee/notification/**/*',
      'src/cpp/ee/play/**/*',
      'src/cpp/ee/recorder/**/*',
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
      'CLANG_CXX_LANGUAGE_STANDARD' => 'c++2a',
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
        '${PODS_ROOT}/../../cocos2d/external/glfw3/include/mac' # Cocos2d-x
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
    s.dependency 'AdColony', '4.4.0'
  end

  spec.subspec 'ad-mob' do |s|
    s.source_files = 'src/ios/ee/ad_mob/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/ads'

    # https://developers.google.com/admob/ios/rel-notes
    # FIXME: use 7.68.0
    s.dependency 'Google-Mobile-Ads-SDK', '7.67.0'
  end

  spec.subspec 'ad-mob-mediation' do |s|
    s.dependency 'ee-x/ad-mob'

    # https://bintray.com/google/mobile-ads-adapters-ios

    # https://developers.google.com/admob/ios/mediation/adcolony#adcolony-ios-mediation-adapter-changelog
    # FIXME: hanging on iPad iOS 13.5.1.
    # s.dependency 'GoogleMobileAdsMediationAdColony', '4.4.0.0'

    # https://developers.google.com/admob/ios/mediation/applovin#applovin-ios-mediation-adapter-changelog
    # FIXME: use 6.14.6.0
    s.dependency 'GoogleMobileAdsMediationAppLovin', '6.14.4.0'

    # https://developers.google.com/admob/ios/mediation/facebook#facebook-ios-mediation-adapter-changelog
    # FIXME: use 6.2.0.0
    s.dependency 'GoogleMobileAdsMediationFacebook', '6.0.0.0'
    
    # https://developers.google.com/admob/ios/mediation/ironsource#ironsource-ios-mediation-adapter-changelog
    # FIXME: use 7.0.3.0
    s.dependency 'GoogleMobileAdsMediationIronSource', '7.0.2.0'

    # https://developers.google.com/admob/ios/mediation/unity#unity-ads-ios-mediation-adapter-changelog
    # FIXME: use 3.5.1.0
    s.dependency 'GoogleMobileAdsMediationUnity', '3.4.8.0'

    # https://developers.google.com/admob/ios/mediation/vungle#vungle-ios-mediation-adapter-changelog
    s.dependency 'GoogleMobileAdsMediationVungle', '6.8.1.0'
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
    s.dependency 'Adjust', '4.23.2'
  end

  spec.subspec 'app-lovin' do |s|
    s.source_files = 'src/ios/ee/app_lovin/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'

    # https://github.com/AppLovin/AppLovin-MAX-SDK-iOS/releases
    # FIXME: use 6.14.6
    s.dependency 'AppLovinSDK', '6.14.4'
  end

  spec.subspec 'apps-flyer' do |s|
    s.source_files = 'src/ios/ee/apps_flyer/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'
    s.dependency 'AppsFlyerFramework', '6.1.1'
  end

  spec.subspec 'core' do |s|
    s.source_files = 'src/ios/ee/core/**/*'
    s.header_mappings_dir = 'src/ios'
    s.user_target_xcconfig = {
      'LD_RUNPATH_SEARCH_PATHS' => [
        '/usr/lib/swift',
        # Fix: dyld: Library not loaded: @rpath/libswiftCore.dylib
        '@executable_path/Frameworks',
        '@loader_path/Frameworks'
      ].join(' '),
      'LIBRARY_SEARCH_PATHS' => [
        '$(TOOLCHAIN_DIR)/usr/lib/swift/$(PLATFORM_NAME)',
        '$(TOOLCHAIN_DIR)/usr/lib/swift-5.0/$(PLATFORM_NAME)',
        '$(SDKROOT)/usr/lib/swift' # Fix __swift_FORCE_LOAD_$_swiftCoreMIDI (XCode 12).
      ].join(' '),
      # https://forums.swift.org/t/undefined-symbol-swift-getfunctionreplacement/30495
      'DEAD_CODE_STRIPPING' => 'YES'
    }
    s.library = 'swiftCore'
    s.dependency 'ReachabilitySwift'
    s.dependency 'RxSwift'
  end

  spec.subspec 'facebook' do |s|
    s.source_files = 'src/ios/ee/facebook/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'

    # https://github.com/facebook/facebook-ios-sdk/releases
    s.dependency 'FBSDKCoreKit', '8.2.0'
    s.dependency 'FBSDKLoginKit', '8.2.0'
    s.dependency 'FBSDKShareKit', '8.2.0'
  end

  spec.subspec 'facebook-ads' do |s|
    s.source_files = 'src/ios/ee/facebook_ads/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/ads'

    # https://developers.facebook.com/docs/audience-network/changelog-ios/
    # FIXME: use 6.2.0
    s.dependency 'FBAudienceNetwork', '6.0.0'
  end

  spec.subspec 'firebase-x-core' do |s|
    s.source_files = 'src/ios/ee/firebase_core/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/core'
    s.dependency 'Firebase/Core', '6.34'
  end

  spec.subspec 'firebase-crashlytics' do |s|
    s.source_files = 'src/ios/ee/firebase_crashlytics/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/firebase-x-core'
    s.dependency 'Firebase/Crashlytics', '6.34'
  end

  spec.subspec 'firebase-performance' do |s|
    s.source_files = 'src/ios/ee/firebase_performance/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/firebase-x-core'
    s.dependency 'Firebase/Performance', '6.34'
  end

  spec.subspec 'iron-source' do |s|
    s.source_files = 'src/ios/ee/iron_source/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/ads'

    # https://developers.ironsrc.com/ironsource-mobile/ios/sdk-change-log/
    # FIXME: use 7.0.3.0
    s.dependency 'IronSourceSDK', '7.0.2.0'
  end

  spec.subspec 'iron-source-mediation' do |s|
    s.dependency 'ee-x/iron-source'

    # https://developers.ironsrc.com/ironsource-mobile/ios/adcolony-change-log/
    # FIXME: use 4.3.4.1
    s.dependency 'IronSourceAdColonyAdapter', '4.3.4.0'
    s.dependency 'AdColony', '4.4.0'

    # https://developers.ironsrc.com/ironsource-mobile/ios/admob-change-log/
    # Wait for supported adapter.
    # s.dependency 'IronSourceAdMobAdapter', '4.3.17.0'

    # https://developers.ironsrc.com/ironsource-mobile/ios/applovin-change-log/
    # FIXME: use 4.3.20.0
    s.dependency 'IronSourceAppLovinAdapter', '4.3.19.0'

    # https://developers.ironsrc.com/ironsource-mobile/ios/21769-2/
    # FIXME: use 4.3.21.0
    s.dependency 'IronSourceFacebookAdapter', '4.3.20.0'

    # https://developers.ironsrc.com/ironsource-mobile/ios/unityads-change-log/
    # FIXME: use 4.3.6.0
    s.dependency 'IronSourceUnityAdsAdapter', '4.3.4.3'

    # https://developers.ironsrc.com/ironsource-mobile/ios/vungle-change-log/
    # FIXME: use 4.3.7.1
    s.dependency 'IronSourceVungleAdapter', '4.3.7.0'
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
    s.dependency 'TPInAppReceipt'
  end

  spec.subspec 'unity-ads' do |s|
    s.source_files = 'src/ios/ee/unity_ads/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/ads'

    # https://github.com/Unity-Technologies/unity-ads-ios/releases
    # FIXME: use 3.5.1
    s.dependency 'UnityAds', '3.4.8'
  end

  spec.subspec 'vungle' do |s|
    s.source_files = 'src/ios/ee/vungle/**/*'
    s.header_mappings_dir = 'src/ios'
    s.dependency 'ee-x/ads'

    # https://github.com/Vungle/iOS-SDK/blob/master/CHANGELOG.md
    s.dependency 'VungleSDK-iOS', '6.8.0'
  end

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

  spec.subspec 'firebase-analytics' do |s|
    s.source_files = 'src/cpp/ee/firebase_analytics/*'

    s.exclude_files =
      'src/cpp/ee/firebase_analytics/Android.mk',
      'src/cpp/ee/firebase_analytics/CMakeLists.txt',
      'src/cpp/ee/firebase_analytics/generate.sh',
      'src/cpp/ee/firebase_analytics/sourcelist.cmake'

    s.header_mappings_dir = 'src/cpp'

    s.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_analytics.a'
    s.dependency 'ee-x/firebase-core'
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
    s.dependency 'Firebase/DynamicLinks', '6.34'
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
    s.dependency 'Firebase/Messaging', '6.34'
  end

  spec.subspec 'firebase-remote-config' do |s|
    s.source_files = 'src/cpp/ee/firebase_remote_config/*'

    s.exclude_files =
      'src/cpp/ee/firebase_remote_config/Android.mk',
      'src/cpp/ee/firebase_remote_config/CMakeLists.txt',
      'src/cpp/ee/firebase_remote_config/generate.sh',
      'src/cpp/ee/firebase_remote_config/sourcelist.cmake'

    s.header_mappings_dir = 'src/cpp'

    s.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_remote_config.a'
    s.dependency 'ee-x/firebase-core'
    s.dependency 'Firebase/RemoteConfig', '6.34'
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
    s.dependency 'Firebase/Storage', '6.34'
  end

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

  spec.subspec 'soomla-core' do |s|
    s.source_files = 'third_party/soomla/core/src/soomla/**/*'
    s.header_mappings_dir = 'third_party/soomla/core/src'

    s.xcconfig = {
      'HEADER_SEARCH_PATHS' => [
        # For cocos2d-x
        '${PODS_ROOT}/../../cocos2d',
        '${PODS_ROOT}/../../cocos2d/cocos',

        # For cocos creator.
        '${PODS_ROOT}/../../../cocos2d-x',
        '${PODS_ROOT}/../../../cocos2d-x/cocos'
      ].join(' ')
    }
    
    s.dependency 'ee-x/json'
    s.dependency 'ee-x/jansson'
  end

  spec.subspec 'soomla-store-ios' do |s|
    s.source_files = 'third_party/soomla/store/src/ios/**/*'
    s.private_header_files = 'third_party/soomla/store/src/ios/**/*.{h,hpp,inl}'
    s.header_mappings_dir = 'third_party/soomla/store/src'
    s.dependency 'ee-x/soomla-core-ios'
  end

  spec.subspec 'soomla-store' do |s|
    s.source_files = 'third_party/soomla/store/src/soomla/**/*'
    s.header_mappings_dir = 'third_party/soomla/store/src'
    s.xcconfig = {
      'CLANG_CXX_LANGUAGE_STANDARD' => 'c++2a' # For std::optional.
    }
    s.dependency 'ee-x/soomla-core'
    s.ios.dependency 'ee-x/soomla-store-ios'
  end

=begin
  spec.subspec 'jsb-core' do |s|
    s.source_files =
      'src/ee/jsb/jsb_core*',
      'src/ee/jsb/jsb_fwd.hpp',
      'src/ee/jsb/core/*'

    s.xcconfig = {
      'HEADER_SEARCH_PATHS' => [
        '${PODS_ROOT}/../../../cocos2d-x',
        '${PODS_ROOT}/../../../cocos2d-x/cocos',
        '${PODS_ROOT}/../../../cocos2d-x/cocos/editor-support',
        '${PODS_ROOT}/../../../cocos2d-x/external/sources',

        # For macos.
        '${PODS_ROOT}/../../../cocos2d-x/external/mac/include/v8'
      ].join(' ')
    }
    
    s.dependency 'ee-x/core'
  end

  spec.subspec 'jsb-ads' do |s|
    s.source_files =
      'src/ee/jsb/jsb_ads*',
      'src/ee/jsb/ads/*'

    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/ads'
  end

  spec.subspec 'jsb-admob' do |s|
    s.source_files =
      'src/ee/jsb/jsb_admob*',
      'src/ee/jsb/admob/*'

    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/admob'
  end

  spec.subspec 'jsb-facebook-ads' do |s|
    s.source_files =
      'src/ee/jsb/jsb_facebook_ads*',
      'src/ee/jsb/facebook_ads/*'

    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/facebook-ads'
  end

  spec.subspec 'jsb-iron-source' do |s|
    s.source_files =
      'src/ee/jsb/jsb_iron_source*',
      'src/ee/jsb/iron_source/*'

    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/iron-source'
  end

  spec.subspec 'jsb-unity-ads' do |s|
    s.source_files =
      'src/ee/jsb/jsb_unity_ads*',
      'src/ee/jsb/unity_ads/*'

    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/unity-ads'
  end

  spec.subspec 'jsb-vungle' do |s|
    s.source_files =
      'src/ee/jsb/jsb_vungle*',
      'src/ee/jsb/vungle/*'

    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/vungle'
  end

  spec.subspec 'jsb-crashlytics' do |s|
    s.source_files =
      'src/ee/jsb/jsb_crashlytics*',
      'src/ee/jsb/crashlytics/*'
    
    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/crashlytics'
  end

  spec.subspec 'jsb-facebook' do |s|
    s.source_files =
      'src/ee/jsb/jsb_facebook*',
      'src/ee/jsb/facebook/*'

    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/facebook'
  end

  spec.subspec 'jsb-firebase' do |s|
    s.source_files =
      'src/ee/jsb/jsb_firebase*',
      'src/ee/jsb/firebase/*'

    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/firebase-analytics'
    s.dependency 'ee-x/firebase-performance'
    s.dependency 'ee-x/firebase-remote-config'
  end

  spec.subspec 'jsb-google-analytics' do |s|
    s.source_files =
      'src/ee/jsb/jsb_google_analytics*',
      'src/ee/jsb/google/*'
    
    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/google-analytics'
  end
  
  spec.subspec 'jsb-notification' do |s|
    s.source_files =
      'src/ee/jsb/jsb_notification*',
      'src/ee/jsb/notification/*'

    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/notification'
  end

  spec.subspec 'jsb-recorder' do |s|
    s.source_files =
      'src/ee/jsb/jsb_recorder.*',
      'src/ee/jsb/recorder/*'

    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/recorder'
  end
  
  spec.subspec 'jsb-soomla-store' do |s|
    s.source_files =
      'src/ee/jsb/jsb_soomla*',
      'src/ee/jsb/soomla/*'
    
    s.dependency 'ee-x/jsb-core'
    s.dependency 'ee-x/soomla-cocos2dx-store'
  end
=end
end