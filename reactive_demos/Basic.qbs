import qbs

CommonProduct {
  name: "basic_demo"
  type: "application"

  Depends { name: "core" }

  files: [
    "basic_demo.cpp"
  ]
}
