import qbs

CppApplication {
  cpp.cxxLanguageVersion: "c++11"
  Properties {
    condition: qbs.targetOS.contains("macos")
    cpp.minimumMacosVersion: "10.7"
  }
}
