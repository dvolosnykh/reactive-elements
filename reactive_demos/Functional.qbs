import qbs

CommonProduct {
  name: "functional_demo"
  type: "application"

  Depends { name: "core" }

  files: [
    "functional_demo.cpp"
  ]
}
