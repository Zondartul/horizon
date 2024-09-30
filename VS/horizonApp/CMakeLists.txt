cmake_minimum_required(VERSION 3.20)
#include(../horizon/CMake_utils.txt)

project(horizonApp)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/code/source/" *.cpp)
#SUBDIRLIST(HEADER_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/code/include/")

#include_directories(HEADER_DIRS)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/code/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../horizonGUI/code/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../horizonRender/code/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../horizonResource/code/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../horizonProgram/code/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/../horizonUtil/code/include/"
    )
# todo: figure out how to create a single link target
target_link_libraries(${PROJECT_NAME} PRIVATE 
    #horizonUtil::horizonUtil
    #horizonResource::horizonResource
    #horizonProgram::horizonProgram
    #horizonRender::horizonRender
    horizonGUI
    horizonRender
    horizonResource
    horizonProgram
    horizonUtil
    )

install(TARGETS ${PROJECT_NAME} DESTINATION "${CMAKE_SOURCE_DIR}/bin")
