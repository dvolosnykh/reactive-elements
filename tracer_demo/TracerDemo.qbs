import qbs

CommonProduct {
  name: "tracer_demo"
  type: "application"

  Depends { name: "core" }

  files: [
    "tracer_demo.cpp"
  ]
}
