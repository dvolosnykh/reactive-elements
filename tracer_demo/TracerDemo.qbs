import qbs

CommonProduct {
  name: "tracer_demo"
  type: "application"
  consoleApplication: true

  Depends { name: "core" }

  files: [
    "tracer_demo.cpp"
  ]
}
