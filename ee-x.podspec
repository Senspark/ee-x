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
        core.source_files   = "core/src/ee/*.{h,hpp,m,mm,cpp}",
                              "core/src/ee/**/*.{h,hpp,m,mm,cpp}",
                              "core/src/ee/**/**/*.{h,hpp,m,mm,cpp}"

        core.public_header_files = "core/src/ee/Logger.hpp",
                                   "core/src/ee/LogLevel.hpp"

        core.header_mappings_dir = "core/src"
    end                             

    spec.subspec 'crash' do |crash|
        crash.source_files  = "crashlytics/src/ee/*.{h,hpp,m,mm,cpp}",
                              "crashlytics/src/ee/**/*.{h,hpp,m,mm,cpp}"

        crash.public_header_files = "crashlytics/src/ee/CrashlyticsProtocol.hpp"

        crash.header_mappings_dir = "crashlytics/src"

        crash.dependency 'Crashlytics', '~> 3.7'
        crash.dependency 'Fabric', '~> 1.6'
    end
end
