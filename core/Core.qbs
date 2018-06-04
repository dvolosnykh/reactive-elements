import qbs
import qbs.FileInfo

CommonProduct {
  name: "core"
  type: "staticlibrary"

  files: [
    "typename.hpp",
    "ArgumentsPrinter.hpp",
    "Subject.hpp",
    "Tracer.hpp"
  ]

  Export {
    Depends { name: "cpp" }
    cpp.includePaths: FileInfo.joinPaths(product.sourceDirectory, "..")
  }
}
