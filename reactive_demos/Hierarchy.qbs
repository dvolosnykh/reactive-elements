import qbs

CommonProduct {
  name: "hierarchy_demo"
  type: "application"

  Depends { name: "core" }

  Properties {
    condition: project.shared_api
    cpp.defines: ["SHARED_API"]
  }

  files: [
    "hierarchy_demo.cpp"
  ]
}
