# CMake 工程，读者应该熟悉了，我就不一一注释了
cmake_minimum_required( VERSION 2.8 )
project( test_project )

set( CMAKE_BUILD_TYPE Release )
set( CMAKE_CXX_FLAGS "-std=c++11 -Wall -O2 -march=native" )

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )

find_package( OpenCV REQUIRED )

include_directories( 
${EIGEN3_INCLUDE_DIR} 
)

add_executable( test test.cpp )
target_link_libraries( test
    ${OpenCV_LIBS}
    )
