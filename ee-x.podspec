Pod::Spec.new do |spec|
  spec.name           = 'ee-x'
  spec.version        = '0.1.3'
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

  spec.subspec 'firebase-cpp-sdk' do |s|
    s.platform = :ios

    s.preserve_path = 'third_party/firebase_cpp_sdk/include'
    s.public_header_files = 'third_party/firebase_cpp_sdk/include/**/*'

    s.vendored_library = 
      'third_party/firebase_cpp_sdk/libs/ios/universal/libapp.a',
      'third_party/firebase_cpp_sdk/libs/ios/universal/libanalytics.a',
      'third_party/firebase_cpp_sdk/libs/ios/universal/libremote_config.a',
      'third_party/firebase_cpp_sdk/libs/ios/universal/libstorage.a',
      'third_party/firebase_cpp_sdk/libs/ios/universal/libmessaging.a'

    s.header_mappings_dir = 'third_party/firebase_cpp_sdk/include'
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
      'src/ee/core/EEPluginManager.h'

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

  spec.subspec 'firebase' do |s|
    s.source_files =
      'src/ee/Firebase.hpp',
      'src/ee/FirebaseFwd.hpp',
      'src/ee/firebase/*.{cpp,hpp,mm}'

    s.public_header_files =
      'src/ee/Firebase.hpp',
      'src/ee/FirebaseFwd.hpp',
      'src/ee/firebase/FirebaseApp.hpp',
      'src/ee/firebase/FirebaseAnalytics.hpp',
      'src/ee/firebase/FirebaseRemoteConfig.hpp',
      'src/ee/firebase/FirebaseStorage.hpp',
      'src/ee/firebase/FirebaseMessaging.hpp'

    s.exclude_files =
      'src/ee/firebase/FirebaseApp.cpp'

    s.dependency 'ee-x/core'
    s.ios.dependency 'ee-x/firebase-cpp-sdk'
    s.ios.dependency 'Firebase/Core'
    s.ios.dependency 'Firebase/RemoteConfig'
    s.ios.dependency 'Firebase/Storage'
    s.ios.dependency 'Firebase/Messaging'
  end

  spec.subspec 'facebook-ads' do |s|
    s.source_files =
      'src/ee/FacebookAds.hpp',
      'src/ee/FacebookAdsFwd.hpp',
      'src/ee/facebook/*.{cpp,hpp,mm,h,m}'

    s.public_header_files =
      'src/ee/FacebookAds.hpp',
      'src/ee/FacebookAdsFwd.hpp',
      'src/ee/facebook/FacebookAdsBridge.hpp'

    s.dependency 'ee-x/core'
    s.dependency 'FBAudienceNetwork'
  end

  spec.subspec 'unity-ads' do |s|
    s.source_files =
      'src/ee/UnityLibs.hpp',
      'src/ee/UnityAdsFwd.hpp',
      'src/ee/unityads/*.{cpp,hpp,mm,h,m}'

    s.public_header_files =
      'src/ee/UnityLibs.hpp',
      'src/ee/UnityAdsFwd.hpp',
      'src/ee/unityads/UnityAdsBridge.hpp'

    s.dependency 'ee-x/core'
  end

  spec.subspec 'ironsrc' do |s|
    s.source_files =
      'src/ee/IronSrc.hpp',
      'src/ee/IronSrcFwd.hpp',
      'src/ee/ironsrc/*.{cpp,hpp,mm,h,m}'

    s.public_header_files =
      'src/ee/IronSrc.hpp',
      'src/ee/IronSrcFwd.hpp',
      'src/ee/ironsrc/IronSrcBridge.hpp'

    s.dependency 'ee-x/core'
    s.dependency 'IronSourceSDK'
  end
end
