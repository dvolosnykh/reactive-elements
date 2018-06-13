import qbs

CommonProduct {
  name: "shared_hierarchy_demo"
  type: "application"

  Depends { name: "core" }

  files: [
    "shared_hierarchy_demo.cpp"
  ]
}
