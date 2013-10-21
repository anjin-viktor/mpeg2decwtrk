set(PROJECT mpeg2wtrkinfo)

project(${PROJECT})


set(SOURCES
	src/getbits.c
	src/getblk.c
	src/gethdr.c
	src/getpic.c
	src/getvlc.c
	src/getvlc.h
	src/idct.c
	src/idctref.c
	src/motion.c
	src/mpeg2dec.c
	src/mpeg2dec.h
	src/recon.c
	src/spatscal.c
	src/subspic.c
	src/systems.c
	src/store.c
)



set(LIBS
	m
	xml2
)

include_directories(
	/usr/include/libxml2
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

set (LIBRARY_OUTPUT_PATH
	${BUILD_DIR}
)

add_executable(${PROJECT} ${SOURCES}) 

target_link_libraries(${PROJECT} ${LIBS})
