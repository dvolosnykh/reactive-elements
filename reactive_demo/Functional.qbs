import qbs

CommonProduct {
  name: "functional_demo"
  type: "application"
  consoleApplication: true

  Depends { name: "core" }

  files: [
    "functional_demo.cpp"
  ]
}
