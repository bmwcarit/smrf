include("${CMAKE_CURRENT_LIST_DIR}/smrfTargets.cmake")

find_package(OpenSSL 1.0.2 REQUIRED)
include("${CMAKE_CURRENT_LIST_DIR}/CheckOpenSSLImportTargets.cmake")

find_package(Boost 1.58.0 REQUIRED COMPONENTS iostreams)
include("${CMAKE_CURRENT_LIST_DIR}/CheckBoostImportTargets.cmake")

