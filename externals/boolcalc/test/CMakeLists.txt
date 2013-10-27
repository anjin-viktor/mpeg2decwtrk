set(PROJECT tests)

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

set(SOURCES
	main.cpp
	exec.cpp
	parser.cpp
	)

set(LIBS
	boolcalc
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../src/
	${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/../include/
)

add_executable (tests_lowlevel ${SOURCES})
target_link_libraries (tests_lowlevel ${LIBS} ${Boost_LIBRARIES})

add_executable (tests_hilevel hi_level.cpp main.cpp)
target_link_libraries (tests_hilevel boolcalc ${Boost_LIBRARIES})


enable_testing()
add_test (tests ${BUILD_DIR}/tests_lowlevel)
add_test (tests ${BUILD_DIR}/tests_hilevel)
