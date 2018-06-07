import qbs
import qbs.FileInfo

CommonProduct {
  name: "core"
  type: "staticlibrary"

  files: [
    "typename.hpp",
    "FunctionArguments.hpp",
    "Subject.hpp",
    "Tracer.hpp"
  ]

  Export {
    Depends { name: "cpp" }
    Depends { name: "boost" }
    cpp.includePaths: FileInfo.joinPaths(product.sourceDirectory, "..")
  }
}
