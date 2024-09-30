cmake_minimum_required(VERSION 3.20)
#include(../horizon/CMake_utils.txt)
project(horizonRender)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
#these checks aren't necessary
#see https://cmake.org/cmake/help/latest/manual/cmake-modules.7.html#manual:cmake-modules(7)
#for a list of variables generated when find_package
#succeeds.
if(NOT DEFINED OPENGL_LIBRARIES)
    message( SEND_ERROR "OpenGL not found")
endif()
if(NOT DEFINED GLEW_LIBRARIES)
    message( SEND_ERROR "GLEW not found")
endif()

# depend only on the previous layer of the engine
#add_subdirectory(../horizonResource buildResource)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/code/source/" *.cpp)
#SUBDIRLIST(HEADER_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/code/include/")
#include_directories(HEADER_DIRS)

#include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE USE_GLEW)
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/code/include/"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../horizonGUI/code/include/"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../horizonRender/code/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../horizonResource/code/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../horizonProgram/code/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../horizonUtil/code/include/"
    )


#if(WIN32)
#    SET(LIB_GLEW glew32)
#endif(WIN32)
#if(UNIX)
#    SET(LIB_GLEW libGLEW.a)
#endif(UNIX)
link_directories(bin)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    horizonResource
    horizonUtil
#    ${LIB_GLEW}
    ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES}
    )

#install(TARGETS ${PROJECT_NAME} DESTINATION "${CMAKE_SOURCE_DIR}/bin")
