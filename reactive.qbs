import qbs

Project {
  minimumQbsVersion: "1.5.2"
  qbsSearchPaths: "."
  references: [
    "core/Core.qbs",
    "reactive_demo/ReactiveDemo.qbs",
    "tracer_demo/TracerDemo.qbs"
  ]
}
