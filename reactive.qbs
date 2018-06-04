import qbs

Project {
  minimumQbsVersion: "1.5.2"

  CppApplication {
    name: "tracer_demo"
    consoleApplication: true
    cpp.cxxLanguageVersion: "c++11"
    cpp.minimumMacosVersion: "10.7"
    files: [
      "typename.hpp",
      "ArgumentsPrinter.hpp",
      "Tracer.hpp",
      "tracer_demo.cpp"
    ]
  }

  CppApplication {
    name: "functional"
    consoleApplication: true
    cpp.cxxLanguageVersion: "c++11"
    cpp.minimumMacosVersion: "10.7"
    files: [
      "typename.hpp",
      "ArgumentsPrinter.hpp",
      "Tracer.hpp",
      "Subject.hpp",
      "functional.cpp"
    ]
  }

  CppApplication {
    name: "shared"
    consoleApplication: true
    cpp.cxxLanguageVersion: "c++11"
    cpp.minimumMacosVersion: "10.7"
    files: [
      "Subject.hpp",
      "shared.cpp"
    ]
  }
}
