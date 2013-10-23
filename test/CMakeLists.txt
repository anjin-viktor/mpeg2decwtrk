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
)

add_executable (${PROJECT} ${SOURCES})

target_link_libraries (${PROJECT} ${LIBS} ${Boost_LIBRARIES})

enable_testing()
add_test (tests ${BUILD_DIR}/${PROJECT})
