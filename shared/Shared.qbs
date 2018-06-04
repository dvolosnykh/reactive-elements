import qbs

CommonProduct {
  name: "shared"
  type: "application"
  consoleApplication: true

  Depends { name: "core" }

  files: [
    "shared.cpp"
  ]
}
