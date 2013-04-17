project( Property )
cmake_minimum_required(VERSION 2.6)

find_prerequests( Property REQUIRED Boost Eigen Core Burning ) 

configure_project()
set( Property_BUILD_EXAMPLES OFF )

make_library()

add_subdirectory( ffi )
