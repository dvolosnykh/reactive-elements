import qbs

CommonProduct {
  name: "functional_hierarchy_demo"
  type: "application"

  Depends { name: "core" }

  files: [
    "functional_hierarchy_demo.cpp"
  ]
}
