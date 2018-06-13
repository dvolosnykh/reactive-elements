import qbs

CommonProduct {
  name: "basic_demo"
  type: "application"

  Depends { name: "core" }

  Properties {
    condition: project.shared_api
    cpp.defines: ["SHARED_API"]
  }

  files: [
    "basic_demo.cpp"
  ]
}
