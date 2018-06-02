import qbs

Project {
  minimumQbsVersion: "1.5.2"

  CppApplication {
    name: "tracer_demo"
    consoleApplication: true
    cpp.cxxLanguageVersion: "c++11"
    files: [
      "tracer.hpp",
      "tracer_demo.cpp"
    ]
  }

  CppApplication {
    name: "functional"
    consoleApplication: true
    cpp.cxxLanguageVersion: "c++11"
    files: [
      "subject.hpp",
      "tracer.hpp",
      "functional.cpp"
    ]
  }

  CppApplication {
    name: "shared"
    consoleApplication: true
    cpp.cxxLanguageVersion: "c++11"
    files: [
      "subject.hpp",
      "shared.cpp"
    ]
  }
}
