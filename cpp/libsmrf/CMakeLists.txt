set(
    SMRF_SOURCES
    smrf.cpp
    compression.h
    MessagePrefix.h
)

set(
    SMRF_PUBLIC_HEADER_FILES
    exceptions.h
    ByteVector.h
    ByteArrayView.h
    CertificateIdentifier.h
    MessageSerializer.h
    MessageDeserializer.h
    Certificate.h
    PrivateKey.h
)

# prepend path to public header files
string(REGEX REPLACE "([^;]+)" "include/smrf/\\1" SMRF_PUBLIC_HEADERS "${SMRF_PUBLIC_HEADER_FILES}")

add_library(
    smrf
    ${SMRF_PUBLIC_HEADERS}
    ${SMRF_SOURCES}
)

set_target_properties(
    smrf
    PROPERTIES
    SOVERSION "${SMRF_SOVERSION}"
    VERSION "${SMRF_VERSION}"
)


target_link_libraries(
    smrf
    OpenSSL::Crypto
    Boost::iostreams
)

AddClangFormat(smrf)

add_dependencies(smrf flatbuffers_generated)
target_include_directories(
    smrf
    SYSTEM
    PRIVATE
    ${FLATBUFFERS_GENERATED_DIR}
    ${FLATBUFFERS_INCLUDE_DIR}
)

set(
    SMRF_PUBLIC_INCLUDE_DIRECTORY
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_include_directories(
    smrf
    PRIVATE ${SMRF_PUBLIC_INCLUDE_DIRECTORY}
)

target_include_directories(
    smrf
    INTERFACE
    "$<BUILD_INTERFACE:${SMRF_PUBLIC_INCLUDE_DIRECTORY}>"
    "$<INSTALL_INTERFACE:include>"
)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    TARGETS smrf
    EXPORT smrfTargets
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
