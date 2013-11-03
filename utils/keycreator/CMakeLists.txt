set(PROJECT keycreator)

project(${PROJECT})

set(SOURCES
	src/WatermarkInfo.cpp
	src/UsersInfo.cpp
	src/Key.cpp
	src/main.cpp
)


find_package(PkgConfig)
pkg_check_modules(GLIBMM glibmm-2.4)
pkg_check_modules(LIBXML++ libxml++-2.6)

find_package(Boost COMPONENTS program_options REQUIRED)

set(LIBS
	${GLIBMM_LIBRARIES}
	${LIBXML++_LIBRARIES}
	${Boost_LIBRARIES}
)

include_directories(
	${GLIBMM_INCLUDE_DIRS}
	${LIBXML++_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
)

add_executable(${PROJECT} ${SOURCES}) 
target_link_libraries(${PROJECT} ${LIBS})
