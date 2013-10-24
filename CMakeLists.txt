cmake_minimum_required(VERSION 2.8)

set (BUILD_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/build
)

if(NOT IS_DIRECTORY ${BUILD_DIR})
        file (MAKE_DIRECTORY
                ${BUILD_DIR}
        )
endif()

if(CMAKE_COMPILER_IS_GNUCXX )
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
endif()

set(EXECUTABLE_OUTPUT_PATH ${BUILD_DIR})

set (LIBRARY_OUTPUT_PATH ${BUILD_DIR})

add_subdirectory(src/)
add_subdirectory(test/)
add_subdirectory(samples/)
enable_testing ()
