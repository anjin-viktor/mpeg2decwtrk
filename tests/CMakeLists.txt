set(PROJECT tests)

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

set(SOURCES
	main.cpp
	LFSR_test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../auxiliary/LFSR.cpp
)


set(LIBS
	${BUILD_DIR}/libboolcalc.a
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../auxiliary/
	${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/../externals/boolcalc/include/
)

if(CMAKE_COMPILER_IS_GNUCXX )
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++0x)
endif()

add_executable (tests ${SOURCES})
target_link_libraries (tests ${LIBS} ${Boost_LIBRARIES})


enable_testing()
add_test (tests ${BUILD_DIR}/tests)
