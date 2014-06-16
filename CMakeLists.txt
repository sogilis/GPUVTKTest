
cmake_minimum_required(VERSION 2.8)

PROJECT(GPUTest)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(GPUTest MACOSX_BUNDLE GPUTest)

if(VTK_LIBRARIES)
  target_link_libraries(GPUTest ${VTK_LIBRARIES})
else()
  target_link_libraries(GPUTest vtkHybrid vtkWidgets)
endif()
