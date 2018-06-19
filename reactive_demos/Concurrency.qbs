import qbs

CommonProduct {
  name: "concurrency_demo"
  type: "application"
  condition: project.use_boost

  Depends { name: "core" }
  Depends { name: "Boost.system" }

  files: [
    "concurrency_demo.cpp"
  ]
}
