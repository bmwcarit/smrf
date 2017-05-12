message(STATUS "variable Boost_FOUND=${Boost_FOUND}")
message(STATUS "variable Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}")
message(STATUS "variable Boost_LIBRARY_DIRS=${Boost_LIBRARY_DIRS}")
message(STATUS "variable Boost_LIBRARIES=${Boost_LIBRARIES}")
message(STATUS "variable Boost_VERSION=${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")

if(NOT TARGET Boost::boost)
    message(STATUS "Boost::boost target not defined. Creating IMPORTED target.")
    message(STATUS "variable Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}")
    add_library(Boost::boost UNKNOWN IMPORTED GLOBAL)
    set_target_properties(Boost::boost PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}"
    )
endif(NOT TARGET Boost::boost)

if(NOT TARGET Boost::iostreams)
    message(STATUS "Boost::iostreams target not defined. Creating IMPORTED target.")
    message(STATUS "variable Boost_IOSTREAMS_LIBRARY=${Boost_IOSTREAMS_LIBRARY}")
    add_library(Boost::iostreams SHARED IMPORTED GLOBAL)
    set_target_properties(Boost::iostreams PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
        IMPORTED_LOCATION "${Boost_IOSTREAMS_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}"
    )
endif(NOT TARGET Boost::iostreams)
