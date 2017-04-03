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

    spec.ios.frameworks  = "Foundation"

    spec.requires_arc = false

    spec.source_files   = "src/ee/*.{h,hpp,m,mm,cpp}",
                          "src/ee/**/*.{h,hpp,m,mm,cpp}",
                          "src/ee/**/**/*.{h,hpp,m,mm,cpp}"

    spec.public_header_files = "src/ee/Logger.hpp",
                               "src/ee/LogLevel.hpp",
                               "src/ee/Notification.hpp",
                               "src/ee/NotificationBuilder.hpp",
                               "src/ee/Crashlytics.hpp",
                               "src/ee/EEPluginManager.h"

    spec.header_mappings_dir = "src"

    spec.dependency 'Crashlytics', '~> 3.7'
    spec.dependency 'Fabric', '~> 1.6'
end
