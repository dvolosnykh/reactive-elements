import qbs

CommonProduct {
  name: "hierarchy_demo"
  type: "application"
  consoleApplication: true

  Depends { name: "core" }

  files: [
    "hierarchy_demo.cpp"
  ]
}
