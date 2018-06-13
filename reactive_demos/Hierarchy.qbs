import qbs

CommonProduct {
  name: "hierarchy_demo"
  type: "application"

  Depends { name: "core" }

  files: [
    "hierarchy_demo.cpp"
  ]
}
