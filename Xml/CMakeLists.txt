project( Xml )

if( CMAKE_COMPILER_IS_GNUCXX )
  add_definitions(-Wno-strict-aliasing )
endif()

find_prerequests( Xml REQUIRED GLOG )

set( Xml_USE_PROFILING OFF )
configure_project()

set( Xml_BUILD_EXAMPLES OFF )
make_library()