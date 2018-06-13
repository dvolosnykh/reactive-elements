import qbs

CommonProduct {
  name: "shared_demo"
  type: "application"

  Depends { name: "core" }

  files: [
    "shared_demo.cpp"
  ]
}
