set(PROJECT cipher)

project(${PROJECT})

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/../../auxiliary/Key.cpp
	src/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../../auxiliary/LFSR.cpp
)


find_package(PkgConfig)
pkg_check_modules(GLIBMM glibmm-2.4)
pkg_check_modules(LIBXML++ libxml++-2.6)

find_package(Boost COMPONENTS program_options REQUIRED)

set(LIBS
	${GLIBMM_LIBRARIES}
	${LIBXML++_LIBRARIES}
	${Boost_LIBRARIES}
	${BUILD_DIR}/libboolcalc.a
)

include_directories(
	${GLIBMM_INCLUDE_DIRS}
	${LIBXML++_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/../../auxiliary/
	${CMAKE_CURRENT_SOURCE_DIR}/../../externals/boolcalc/include/
)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++0x)
endif()

add_executable(${PROJECT} ${SOURCES}) 
target_link_libraries(${PROJECT} ${LIBS})
add_dependencies(${PROJECT} boolcalc)
