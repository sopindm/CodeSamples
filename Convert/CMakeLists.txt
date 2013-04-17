project( Convert )
cmake_minimum_required(VERSION 2.6)

set( Convert_BOOST_COMPONENTS filesystem )
find_prerequests( Convert REQUIRED Scenegraph ASSIMP Profiling OpenSSL Boost Property Burning )

configure_project()
set( Convert_BUILD_EXAMPLES OFF )

make_library()

add_subdirectory( ffi )
