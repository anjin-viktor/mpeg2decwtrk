INCLUDE(FindPkgConfig)

set(PROJECT mpeg2wtrkinfosink)

project(${PROJECT})

pkg_search_module(GST_BASE REQUIRED gstreamer-base-1.0)
pkg_search_module(GST REQUIRED gstreamer-1.0)


set(SOURCES
	mpeg2wtrkinfosink.c
	gstmpegvideoparser/gstmpegvideoparser.c
	gstmpegvideoparser/parserutils.c
)


set(LIBS 
	${GST_LIBRARIES}
	${GST_BASE_LIBRARIES}
)


include_directories(
	${GST_INCLUDE_DIRS}
	${GST_BASE_INCLUDE_DIRS}
)

link_directories(
	${GST_LIBRARY_DIRS}
	${GST_BASE_LIBRARY_DIRS}
)

set (LIBRARY_OUTPUT_PATH
	${BUILD_DIR}
)


add_definitions(-DGST_USE_UNSTABLE_API)

add_library(${PROJECT} SHARED ${SOURCES}) 

target_link_libraries(${PROJECT} ${LIBS})
