project( Scenegraph )
cmake_minimum_required(VERSION 2.6)

set( Scenegraph_BOOST_COMPONENTS system )
find_prerequests( Scenegraph REQUIRED Cuda Core Graphics OpenGL GLEW FreeImage Boost Property )

configure_project()

set( Scenegraph_SUBDIRECTORIES Materials Fresnel Lights Rendering )

make_cuda_library()

add_subdirectory( ffi )
