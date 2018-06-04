import qbs

Project {
  minimumQbsVersion: "1.5.2"
  qbsSearchPaths: "."
  references: [
    "core/Core.qbs",
    "tracer_demo/TracerDemo.qbs",
    "functional/Functional.qbs",
    "shared/Shared.qbs"
  ]
}
