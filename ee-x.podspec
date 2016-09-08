Pod::Spec.new do |spec|
    spec.name           = "ee-x"
    spec.version        = "0.0.1"
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

    spec.subspec 'core' do |core|
        core.source_files   = "core/src/**/*.{m,mm,cpp}",
                              "core/include/**/*.{h,hpp}"

        core.public_header_files = "core/include/ee/Logger.hpp",
                                   "core/include/ee/LogLevel.hpp"

        core.header_mappings_dir = "core/include"
    end                             

    spec.subspec 'crash' do |crash|
        crash.source_files  = "crashlytics/src/**/*.{m,cpp}",
                              "crashlytics/include/**/*.{h,hpp}"

        crash.public_header_files = "crashlytics/include/ee/CrashlyticsProtocol.hpp"

        crash.header_mappings_dir = "crashlytics/include"

        crash.dependency 'Crashlytics', '~> 3.7'
        crash.dependency 'Fabric', '~> 1.6'
    end
end
