Pod::Spec.new do |s|
  s.name         = "libutki"
  s.version      = "1.0.8"
  s.summary      = "C++ utility library. Stuff missing from std:: namespace."
  s.homepage     = "https://github.com/igagis/utki"
  s.license      = 'MIT'
  s.author             = { "Ivan Gagis" => "igagis@gmail.com" }
  s.platform     = :ios
  # s.platform     = :ios, "5.0"

  #  When using multiple platforms
  # s.ios.deployment_target = "5.0"
  # s.osx.deployment_target = "10.7"
  # s.watchos.deployment_target = "2.0"
  # s.tvos.deployment_target = "9.0"

  s.source = { :git => "https://github.com/igagis/utki.git", :tag => s.version }

  s.source_files  = "Classes", "src/**/*.{hpp,cpp}"
end
