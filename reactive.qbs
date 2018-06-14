import qbs

Project {
  property bool use_boost: false

  Properties {
    condition: qbs.targetOS.contains("macos")
    minimumQbsVersion: "1.5.2"
  }
  Properties {
    condition: !qbs.targetOS.contains("macos")
    minimumQbsVersion: "1.4.5"
  }

  qbsSearchPaths: "."
  references: [
    "core/Core.qbs",
    "reactive_demos/ReactiveDemos.qbs",
    "tracer_demo/TracerDemo.qbs"
  ]
}
