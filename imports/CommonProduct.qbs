import qbs 1.0

CppApplication {
  cpp.cxxLanguageVersion: "c++11"
  cpp.warningLevel: "all"

  Properties {
    condition: qbs.targetOS.contains("macos")
    cpp.minimumMacosVersion: "10.7"
  }
}
