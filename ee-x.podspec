Pod::Spec.new do |spec|
  spec.name           = 'ee-x'
  spec.version        = '0.1.5'
  spec.summary        = 'ee-x'
  spec.description    = 'ee-x'

  spec.homepage       = 'https://github.com/Senspark/ee-x'

  # spec.license        = { :type => 'MIT', :file => 'FILE_LICENSE' }
  spec.author         = 'Hai Hoang'

  spec.swift_version = '5.0'
  spec.ios.deployment_target = '12.0'
  spec.osx.deployment_target = '10.14'

  spec.source = {
    :git => 'https://github.com/Senspark/ee-x.git',
    :branch => 'master'
  }

  spec.framework = 'Foundation'

  spec.requires_arc = false

  spec.header_mappings_dir = 'src'

  spec.subspec 'json' do |s|
    s.source_files = 'third_party/nlohmann/include/**/*'
    s.private_header_files = 'third_party/nlohmann/include/**/*'
    s.header_mappings_dir = 'third_party/nlohmann/include'
  end

  spec.subspec 'core' do |s|
    s.source_files =
      'src/ee/Macro.hpp',
      'src/ee/Core*',
      'src/ee/core/**/*'

    s.private_header_files =
      'src/ee/core/private/**'
    
    s.exclude_files =
      'src/ee/core/Android.mk',
      'src/ee/core/CMakeLists.txt',
      'src/ee/core/generate.sh',
      'src/ee/core/sourcelist.cmake',
      'src/ee/core/**/*Android*',
      'src/ee/core/**/Jni*'

    s.xcconfig = {
      'CLANG_CXX_LANGUAGE_STANDARD' => 'c++2a',
      'OTHER_CPLUSPLUSFLAGS' => '-fcoroutines-ts',
      'GCC_PREPROCESSOR_DEFINITIONS[config=Release]' => [
        'NDEBUG'
      ].join(' ')
    }

    # Fix linking errors with Facebook SDK 7.
    s.user_target_xcconfig = {
      'LD_RUNPATH_SEARCH_PATHS' => [
        '/usr/lib/swift'
      ].join(' '),
      'LIBRARY_SEARCH_PATHS' => [
        '$(TOOLCHAIN_DIR)/usr/lib/swift/$(PLATFORM_NAME)',
        '$(TOOLCHAIN_DIR)/usr/lib/swift-5.0/$(PLATFORM_NAME)'
      ].join(' '),
      # https://forums.swift.org/t/undefined-symbol-swift-getfunctionreplacement/30495
      'DEAD_CODE_STRIPPING' => 'YES'
    }

    s.dependency 'ee-x/json'
    s.dependency 'ReachabilitySwift'
    s.dependency 'RxSwift'
  end

  spec.subspec 'ads' do |s|
    s.source_files =
      'src/ee/Ads*',
      'src/ee/ads/**/*'

    s.private_header_files =
      'src/ee/ads/internal/*',
      'src/ee/ads/private/*'

    s.exclude_files =
      'src/ee/ads/Android.mk',
      'src/ee/ads/CMakeLists.txt',
      'src/ee/ads/generate.sh',
      'src/ee/ads/sourcelist.cmake'

    s.dependency 'ee-x/core'
  end

  spec.subspec 'admob' do |s|
    s.source_files =
      'src/ee/AdMob*',
      'src/ee/admob/**/*'

    s.private_header_files =
      'src/ee/admob/internal/*',
      'src/ee/admob/private/*'

    s.exclude_files =
      'src/ee/admob/Android.mk',
      'src/ee/admob/CMakeLists.txt',
      'src/ee/admob/generate.sh',
      'src/ee/admob/sourcelist.cmake'

    s.resources = 'res/*'
    s.dependency 'ee-x/ads'
    s.dependency 'Google-Mobile-Ads-SDK'
  end

  spec.subspec 'app-lovin' do |s|
    s.source_files =
      'src/ee/AppLovin*',
      'src/ee/app_lovin/**/*'

    s.private_header_files =
      'src/ee/app_lovin/internal/*',
      'src/ee/app_lovin/private/*'
    
    s.exclude_files =
      'src/ee/app_lovin/Android.mk',
      'src/ee/app_lovin/CMakeLists.txt',
      'src/ee/app_lovin/generate.sh',
      'src/ee/app_lovin/sourcelist.cmake'

    s.dependency 'ee-x/ads'
    s.dependency 'AppLovinSDK'
  end

  spec.subspec 'app-lovin-mediation' do |s|
    s.dependency 'ee-x/app-lovin'
    s.dependency 'ee-x/iron-source-mediation-base'
    s.dependency 'IronSourceAppLovinAdapter'
  end

  spec.subspec 'facebook-ads' do |s|
    s.source_files =
      'src/ee/FacebookAds*',
      'src/ee/facebook_ads/**/*'

    s.private_header_files =
      'src/ee/facebook_ads/internal/*',
      'src/ee/facebook_ads/private/*'

    s.exclude_files =
      'src/ee/facebook_ads/Android.mk',
      'src/ee/facebook_ads/CMakeLists.txt',
      'src/ee/facebook_ads/generate.sh',
      'src/ee/facebook_ads/sourcelist.cmake'
      
    s.dependency 'ee-x/ads'
    s.dependency 'FBAudienceNetwork'
  end

  spec.subspec 'iron-source-mediation-base' do |s|
    s.preserve_path = 'dummy_path'
    s.pod_target_xcconfig = {
      'GCC_PREPROCESSOR_DEFINITIONS' => [
        # Used to determine which headers to include.
        'EE_X_USE_IRON_SOURCE_MEDIATION'
      ].join(' ')
    }
  end

  spec.subspec 'iron-source' do |s|
    s.source_files =
      'src/ee/IronSource*',
      'src/ee/iron_source/**/*'

    s.private_header_files =
      'src/ee/iron_source/internal/*',
      'src/ee/iron_source/private/*'

    s.exclude_files =
      'src/ee/iron_source/Android.mk',
      'src/ee/iron_source/CMakeLists.txt',
      'src/ee/iron_source/generate.sh',
      'src/ee/iron_source/sourcelist.cmake'

    s.dependency 'ee-x/ads'
    s.dependency 'IronSourceSDK'
  end

  spec.subspec 'unity-ads-base' do |s|
    s.source_files =
      'src/ee/UnityAds*',
      'src/ee/unity_ads/**/*'

    s.private_header_files =
      'src/ee/unity_ads/internal/*',
      'src/ee/unity_ads/private/*'

    s.exclude_files =
      'src/ee/unity_ads/Android.mk',
      'src/ee/unity_ads/CMakeLists.txt',
      'src/ee/unity_ads/generate.sh',
      'src/ee/unity_ads/sourcelist.cmake'

    s.dependency 'ee-x/ads'
  end

  spec.subspec 'unity-ads' do |s|
    s.dependency 'ee-x/unity-ads-base'
    s.dependency 'UnityAds'
  end

  spec.subspec 'unity-ads-mediation' do |s|
    s.dependency 'ee-x/unity-ads-base'
    s.dependency 'ee-x/iron-source-mediation-base'

    # UnityAds is included in IronSourceUnityAdsAdapter.
    s.dependency 'IronSourceUnityAdsAdapter'
  end

  spec.subspec 'vungle-base' do |s|
    s.source_files =
      'src/ee/Vungle*',
      'src/ee/vungle/**/*'

    s.private_header_files =
      'src/ee/vungle/internal/*',
      'src/ee/vungle/private/*'

    s.exclude_files =
      'src/ee/vungle/Android.mk',
      'src/ee/vungle/CMakeLists.txt',
      'src/ee/vungle/generate.sh',
      'src/ee/vungle/sourcelist.cmake'

    s.dependency 'ee-x/ads'
  end

  spec.subspec 'vungle' do |s|
    s.dependency 'ee-x/vungle-base'
    s.dependency 'VungleSDK-iOS'
  end

  spec.subspec 'vungle-mediation' do |s|
    s.dependency 'ee-x/vungle-base'
    s.dependency 'ee-x/iron-source-mediation-base'

    # VungleSDK-iOS is included in IronSourceVungleAdapter.
    s.dependency 'IronSourceVungleAdapter'
  end

  spec.subspec 'apps-flyer' do |s|
    s.source_files =
      'src/ee/AppsFlyer*',
      'src/ee/apps_flyer/**/*'

    s.private_header_files =
      'src/ee/apps_flyer/internal/*',
      'src/ee/apps_flyer/private/*'

    s.exclude_files =
      'src/ee/apps_flyer/Android.mk',
      'src/ee/apps_flyer/CMakeLists.txt',
      'src/ee/apps_flyer/generate.sh',
      'src/ee/apps_flyer/sourcelist.cmake'

    s.dependency 'ee-x/core'
    s.dependency 'AppsFlyerFramework'
  end

  spec.subspec 'campaign-receiver' do |s|
    s.source_files =
      'src/ee/CampaignReceiver*',
      'src/ee/campaign_receiver/**/*'

    s.exclude_files =
      'src/ee/campaign_receiver/Android.mk',
      'src/ee/campaign_receiver/CMakeLists.txt',
      'src/ee/campaign_receiver/generate.sh',
      'src/ee/campaign_receiver/sourcelist.cmake'

    s.dependency 'ee-x/core'
  end

  spec.subspec 'crashlytics' do |s|
    s.source_files =
      'src/ee/Crashlytics*',
      'src/ee/crashlytics/**/*'

    s.exclude_files =
      'src/ee/crashlytics/Android.mk',
      'src/ee/crashlytics/CMakeLists.txt',
      'src/ee/crashlytics/generate.sh',
      'src/ee/crashlytics/sourcelist.cmake'

    s.dependency 'ee-x/core'
    s.dependency 'Crashlytics'
    s.dependency 'Fabric'
  end

  spec.subspec 'cocos' do |s|
    s.source_files =
      'src/ee/Cocos*',
      'src/ee/cocos/*'

    s.exclude_files =
      'src/ee/cocos/Android.mk',
      'src/ee/cocos/CMakeLists.txt',
      'src/ee/cocos/generate.sh',
      'src/ee/cocos/sourcelist.cmake'

    s.xcconfig = {
      'GCC_PREPROCESSOR_DEFINITIONS' => [
        'USE_FILE32API' # Used in unzip library in cocos2d-x.
      ].join(' '),
      'GCC_PREPROCESSOR_DEFINITIONS[config=Debug]' => [
        'COCOS2D_DEBUG=1'
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

    s.library = 'iconv'
    s.framework = 'OpenAL'
    s.osx.library = 'z'
    s.osx.framework = 'GameController'
    s.dependency 'ee-x/core'
  end

  spec.subspec 'facebook' do |s|
    s.source_files =
      'src/ee/Facebook{,Fwd}.*',
      'src/ee/facebook/**/*'

    s.private_header_files =
      'src/ee/facebook/internal/*',
      'src/ee/facebook/private/*'

    s.exclude_files =
      'src/ee/facebook/Android.mk',
      'src/ee/facebook/CMakeLists.txt',
      'src/ee/facebook/generate.sh',
      'src/ee/facebook/sourcelist.cmake'

    s.dependency 'ee-x/core'
    s.dependency 'FBSDKCoreKit'
    s.dependency 'FBSDKLoginKit'
    s.dependency 'FBSDKShareKit'
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

    s.private_header_files = 'third_party/firebase_cpp_sdk/include/**/*'

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
      'src/ee/Firebase*',
      'src/ee/firebase/core/*'

    s.exclude_files =
      'src/ee/firebase/core/Android.mk',
      'src/ee/firebase/core/CMakeLists.txt',
      'src/ee/firebase/core/generate.sh',
      'src/ee/firebase/core/sourcelist.cmake',
      'src/ee/firebase/core/*Android*'

    s.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_app.a'
    s.dependency 'ee-x/core'
    s.dependency 'ee-x/firebase-headers'
    s.dependency 'Firebase/Core'
  end

  spec.subspec 'firebase-analytics' do |s|
    s.source_files = 'src/ee/firebase/analytics/*'

    s.exclude_files =
      'src/ee/firebase/analytics/Android.mk',
      'src/ee/firebase/analytics/CMakeLists.txt',
      'src/ee/firebase/analytics/generate.sh',
      'src/ee/firebase/analytics/sourcelist.cmake'

    s.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_analytics.a'
    s.dependency 'ee-x/firebase-core'
  end

  spec.subspec 'firebase-crashlytics' do |s|
    s.source_files = 'src/ee/firebase/crashlytics/**/*'

    s.exclude_files =
      'src/ee/firebase/crashlytics/Android.mk',
      'src/ee/firebase/crashlytics/CMakeLists.txt',
      'src/ee/firebase/crashlytics/generate.sh',
      'src/ee/firebase/crashlytics/sourcelist.cmake'

    s.dependency 'ee-x/firebase-core'
    s.dependency 'Firebase/Crashlytics'
  end

  spec.subspec 'firebase-dynamic-link' do |s|
    s.source_files = 'src/ee/firebase/dynamic_link/*'

    s.exclude_files =
      'src/ee/firebase/dynamic_link/Android.mk',
      'src/ee/firebase/dynamic_link/CMakeLists.txt',
      'src/ee/firebase/dynamic_link/generate.sh',
      'src/ee/firebase/dynamic_link/sourcelist.cmake'

    s.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_dynamic_links.a'
    s.dependency 'ee-x/firebase-core'
    s.dependency 'Firebase/DynamicLinks'
  end

  spec.subspec 'firebase-messaging' do |s|
    s.source_files = 'src/ee/firebase/messaging/*'

    s.exclude_files =
      'src/ee/firebase/messaging/Android.mk',
      'src/ee/firebase/messaging/CMakeLists.txt',
      'src/ee/firebase/messaging/generate.sh',
      'src/ee/firebase/messaging/sourcelist.cmake'

    s.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_messaging.a'
    s.dependency 'ee-x/firebase-core'
    s.dependency 'Firebase/Messaging'
  end

  spec.subspec 'firebase-remote-config' do |s|
    s.source_files = 'src/ee/firebase/remote_config/*'

    s.exclude_files =
      'src/ee/firebase/remote_config/Android.mk',
      'src/ee/firebase/remote_config/CMakeLists.txt',
      'src/ee/firebase/remote_config/generate.sh',
      'src/ee/firebase/remote_config/sourcelist.cmake'

    s.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_remote_config.a'
    s.dependency 'ee-x/firebase-core'
    s.dependency 'Firebase/RemoteConfig'
  end

  spec.subspec 'firebase-storage' do |s|
    s.source_files = 'src/ee/firebase/storage/*'

    s.exclude_files =
      'src/ee/firebase/storage/Android.mk',
      'src/ee/firebase/storage/CMakeLists.txt',
      'src/ee/firebase/storage/generate.sh',
      'src/ee/firebase/storage/sourcelist.cmake'

    s.vendored_library = 'third_party/firebase_cpp_sdk/libs/ios/universal/libfirebase_storage.a'
    s.dependency 'ee-x/firebase-core'
    s.dependency 'Firebase/Storage'
  end

  spec.subspec 'firebase-performance' do |s|
    s.source_files = 'src/ee/firebase/performance/**/*'
    s.private_header_files = 'src/ee/firebase/performance/private/*'

    s.exclude_files =
      'src/ee/firebase/performance/Android.mk',
      'src/ee/firebase/performance/CMakeLists.txt',
      'src/ee/firebase/performance/generate.sh',
      'src/ee/firebase/performance/sourcelist.cmake'
    
    s.dependency 'ee-x/firebase-core'
    s.dependency 'Firebase/Performance'
  end

  spec.subspec 'google-analytics' do |s|
    s.source_files =
      'src/ee/GoogleAnalytics*',
      'src/ee/google/**/*'

    s.private_header_files =
      'src/ee/google/internal/*',
      'src/ee/google/private/*'

    s.exclude_files =
      'src/ee/google/Android.mk',
      'src/ee/google/CMakeLists.txt',
      'src/ee/google/generate.sh',
      'src/ee/google/sourcelist.cmake'

    s.platform = :ios
    s.dependency 'ee-x/core'
    s.dependency 'GoogleAnalytics'
  end

  spec.subspec 'notification' do |s|
    s.source_files =
      'src/ee/Notification*',
      'src/ee/notification/**/*'

    s.exclude_files =
      'src/ee/notification/Android.mk',
      'src/ee/notification/CMakeLists.txt',
      'src/ee/notification/generate.sh',
      'src/ee/notification/sourcelist.cmake'

    s.platform = :ios
    s.dependency 'ee-x/core'
  end

  spec.subspec 'play' do |s|
    s.source_files =
      'src/ee/Play*',
      'src/ee/play/*'

    s.exclude_files =
      'src/ee/play/Android.mk',
      'src/ee/play/CMakeLists.txt',
      'src/ee/play/generate.sh',
      'src/ee/play/sourcelist.cmake'

    s.dependency 'ee-x/core'
  end

  spec.subspec 'recorder' do |s|
    s.source_files =
      'src/ee/Recorder*',
      'src/ee/recorder/*'

    s.exclude_files =
      'src/ee/recorder/Android.mk',
      'src/ee/recorder/CMakeLists.txt',
      'src/ee/recorder/generate.sh',
      'src/ee/recorder/sourcelist.cmake'

    s.dependency 'ee-x/core'
  end

  spec.subspec 'tenjin' do |s|
    s.source_files =
      'src/ee/Tenjin*',
      'src/ee/tenjin/**/*'

    s.private_header_files =
      'src/ee/tenjin/private/*'

    s.exclude_files =
      'src/ee/tenjin/Android.mk',
      'src/ee/tenjin/CMakeLists.txt',
      'src/ee/tenjin/generate.sh',
      'src/ee/tenjin/sourcelist.cmake'

    s.dependency 'ee-x/core'
    s.dependency 'TenjinSDK'
  end

  spec.subspec 'twitter' do |s|
    s.source_files =
      'src/ee/Twitter*',
      'src/ee/twitter/**/*'

    s.dependency 'ee-x/core'
    s.dependency 'TwitterKit'
  end

  spec.subspec 'jansson' do |s| 
    s.source_files = 'third_party/jansson/src/**/*'
    s.private_header_files = 'third_party/jansson/src/**/*'
    s.header_mappings_dir = 'third_party/jansson/src'
  end

  spec.subspec 'keeva' do |s|
    s.source_files = 'third_party/keeva/src/**/*'
    s.private_header_files = 'third_party/keeva/src/**/*'
    s.header_mappings_dir = 'third_party/keeva/src'
  end

  spec.subspec 'soomla-core-ios' do |s|
    s.source_files = 'third_party/soomla/core/src/ios/**/*'
    s.private_header_files = 'third_party/soomla/core/src/ios/**/*'
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
    s.private_header_files = 'third_party/soomla/store/src/ios/**/*'
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

  spec.subspec 'store' do |s|
    s.source_files =
      'src/ee/Store*',
      'src/ee/store/**/*'

    s.dependency 'ee-x/core'
  end

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
end

#  post_install do |installer_representation|
#    installer_representation.project.targets.each do |target|
#      if target.name == "FBSDKShareKit"
#        target.build_configurations.each do |config|
#          config.build_settings['GCC_PREPROCESSOR_DEFINITIONS'] ||= ['$(inherited)', 'BUCK']
#        end
#      end
#    end
#  end
