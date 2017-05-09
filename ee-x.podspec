Pod::Spec.new do |spec|
    spec.name           = "ee-x"
    spec.version        = "0.0.3"
    spec.summary        = "ee-x"
    spec.description    = "ee-x"
  
    spec.homepage       = "https://github.com/enrevol/ee-x"
  
    # s.license         = { :type => "MIT", :file => "FILE_LICENSE" }
    spec.author         = "Hoang Hai"

    spec.ios.deployment_target = "7.0"
    spec.osx.deployment_target = "10.7"
  
    spec.source = {
        :git => "https://github.com/enrevol/ee-x.git",
        :branch => "master"
    }

    spec.ios.frameworks  = 'Foundation'

    spec.requires_arc = false

    spec.subspec 'core' do |s|
      s.source_files = "src/ee/core/LogLevel.cpp",
        'src/ee/Core.hpp',
        'src/ee/core/*.{cpp,h,hpp,m,mm}',
        'src/ee/core/**/*.{cpp,h,hpp,m,mm}',
        'src/ee/libs/nlohmann/*.{cpp,h,hpp,m,mm}'

      s.public_header_files =
        'src/ee/Core.hpp',
        'src/ee/core/Logger.hpp',
        'src/ee/core/LogLevel.hpp',
        'src/ee/core/EEPluginManager.h'
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

    spec.header_mappings_dir = 'src'
end
