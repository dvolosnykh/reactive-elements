import qbs
import qbs.FileInfo

CommonProduct {
  name: "core"

  files: [
    "FunctionArguments.hpp",
    "Subject.hpp",
    "Tracer.hpp",
    "typename.hpp"
  ]

  Export {
    Depends { name: "cpp" }
    Depends {
      condition: project.use_boost
      name: "Boost.core"
    }
    Properties {
      condition: project.use_boost
      cpp.defines: ["USE_BOOST"]
    }
    cpp.includePaths: FileInfo.joinPaths(product.sourceDirectory, "..")
  }
}
