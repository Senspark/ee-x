Pod::Spec.new do |spec|
  spec.name           = 'ee-x'
  spec.version        = '0.1.0'
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
    s.preserve_paths = 'third_party/nlohmann/include/**/*'

    s.xcconfig = {
      'HEADER_SEARCH_PATHS' =>
        '$(PODS_ROOT)/ee-x/third_party/nlohmann/include'
    }
  end

  spec.subspec 'firebase-cpp-sdk' do |s|
    s.platform = :ios

    s.preserve_paths = 'third_party/firebase_cpp_sdk/include/**/*'

    s.vendored_library = 'src/ee/libs/firebase_cpp_sdk/libs/ios/*.a'

    s.xcconfig = {
      'HEADER_SEARCH_PATHS' =>
        '$(PODS_ROOT)/ee-x/third_party/firebase_cpp_sdk/include'
    }
  end

  spec.subspec 'core' do |s|
    s.source_files =
      'src/ee/Core.hpp',
      'src/ee/core/*.{cpp,h,hpp,m,mm}',
      'src/ee/core/**/*.{cpp,h,hpp,m,mm}',

    s.public_header_files =
      'src/ee/Core.hpp',
      'src/ee/core/Logger.hpp',
      'src/ee/core/LogLevel.hpp',
      'src/ee/core/EEPluginManager.h'

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
      'src/ee/firebase/*.{cpp,hpp}'

    s.public_header_files =
      'src/ee/Firebase.hpp',
      'src/ee/firebase/FirebaseAnalytics.hpp',
      'src/ee/firebase/FirebaseRemoteConfig.hpp',
      'src/ee/firebase/FirebaseStorage.hpp'

    s.dependency 'ee-x/core'
    s.ios.dependency 'ee-x/firebase-cpp-sdk'
    s.ios.dependency 'Firebase/Core'
    s.ios.dependency 'Firebase/RemoteConfig'
    s.ios.dependency 'Firebase/Storage'
  end
end
