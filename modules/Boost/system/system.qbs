import qbs 1.0
import ".." as Boost

Boost.BoostSubmodule {
  Properties {
    condition: !qbs.targetOS.contains("macos")
    cpp.dynamicLibraries: ["boost_system"]
  }
  Properties {
    condition: qbs.targetOS.contains("macos")
    cpp.dynamicLibraries: ["boost_system-mt"]
  }
}
