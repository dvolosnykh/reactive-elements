import qbs

Module {
  Depends { name: "cpp" }
  Properties {
    condition: qbs.targetOS.contains("macos")
    cpp.includePaths: ["/opt/local/include"]
    cpp.libraryPaths: ["/opt/local/lib"]
  }
}
