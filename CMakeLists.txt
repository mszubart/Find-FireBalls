########################################################
# FFB project for finding fireballs in video material
########################################################
project(ffb)

cmake_minimum_required(VERSION 2.6)

set(Boost_ADDITIONAL_VERSIONS 1.47.0)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

find_package(OpenCV REQUIRED )
find_package(Boost 1.47.0 COMPONENTS thread program_options system filesystem REQUIRED)

add_subdirectory(src)
