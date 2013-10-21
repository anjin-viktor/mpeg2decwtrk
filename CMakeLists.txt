cmake_minimum_required(VERSION 2.8)

set (BUILD_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/build
)

if(NOT IS_DIRECTORY ${BUILD_DIR})
	file (MAKE_DIRECTORY
		${BUILD_DIR}
	)
endif()


add_subdirectory(gst/mpeg2wtrkinfosink)

add_subdirectory(utils/)
