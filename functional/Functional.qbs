import qbs

CommonProduct {
  name: "functional"
  type: "application"
  consoleApplication: true

  Depends { name: "core" }

  files: [
    "functional.cpp"
  ]
}
