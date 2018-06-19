import qbs

Project{
  property bool shared_api: false

  references: [
    "Basic.qbs",
    "Hierarchy.qbs",
    "Concurrency.qbs"
  ]
}
