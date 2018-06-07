import qbs

CommonProduct {
  name: "shared_demo"
  type: "application"
  consoleApplication: true

  Depends { name: "core" }

  files: [
    "shared_demo.cpp"
  ]
}
