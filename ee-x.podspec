Pod::Spec.new do |spec|
    spec.name           = "ee-x"
    spec.version        = "0.0.2"
    spec.summary        = "ee-x"
    spec.description    = "ee-x"
  
    spec.homepage       = "https://github.com/enrevol/ee-x"
  
    # s.license         = { :type => "MIT", :file => "FILE_LICENSE" }
    spec.author         = "Hoang Hai"

    spec.ios.deployment_target = "5.0"
    spec.osx.deployment_target = "10.7"
  
    spec.source = {
        :git => "https://github.com/enrevol/ee-x.git",
        :branch => "master"
    }

    spec.ios.frameworks  = 'Foundation'

    spec.requires_arc = false

    spec.subspec 'core' do |s|
      s.source_files =
        'src/ee/EEPluginManager.{h,m}',
        'src/ee/internal/EEDictionaryUtils.{h,m}',
        'src/ee/internal/EEJsonUtils.{h,m}',
        'src/ee/internal/EEMessageBridge.{h,m}',
        'src/ee/internal/EEUtils.{h,m}',
        'src/ee/internal/MessageBridge_iOS.mm',
        'src/ee/internal/MessageBridge.{cpp,hpp}',
        'src/ee/libs/nlohmann/json.hpp',
        'src/ee/Logger_iOS.mm',
        'src/ee/Logger.{cpp,hpp}',
        'src/ee/LogLevel.{cpp,hpp}'

      s.public_header_files =
        'src/ee/Logger.hpp',
        'src/ee/LogLevel.hpp',
        'src/ee/EEPluginManager.h'
    end

    spec.subspec 'crashlytics' do |s|
      s.source_files =
        'src/ee/Crashlytics.{cpp,hpp}',
        'src/ee/Crashlytics.{h,m}'

      s.public_header_files =
        'src/ee/Crashlytics.hpp'

      s.dependency 'Crashlytics', '~> 3.7'
      s.dependency 'Fabric', '~> 1.6'
    end

    spec.subspec 'notification' do |s|
      s.source_files =
        'src/ee/Notification.{cpp,hpp}',
        'src/ee/NotificationBuilder.{cpp,hpp}',
        'src/ee/EENotification.{h,m}'

      s.public_header_files = 
        'src/ee/Notification.hpp',
        'src/ee/NotificationBuilder.hpp'
    end

    spec.header_mappings_dir = 'src'
end
