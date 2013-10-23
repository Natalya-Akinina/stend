
cmake_minimum_required(VERSION 2.8)

project(stend)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_LUA QUIET lua)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
string(REPLACE ";" " " PC_LUA_CFLAGS " ${PC_LUA_CFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PC_LUA_CFLAGS} -Wall -pipe -std=c++11" CACHE string "" FORCE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${Qt5Widgets_INCLUDE_DIRS})

if("${CMAKE_BUILD_TYPE}" STREQUAL "")

	#
	# Цель по умолчанию - Debug
	# Для оптимизированной цели: cmake -DCMAKE_BUILD_TYPE=Release ..
	#

	set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)

endif()

############################################################################

set(sources_lib lib/lua.cpp lib/opencv.cpp lib/module.cpp lib/matrix.cpp lib/image.cpp)
set(sources_stat stat/stat.cpp)
set(sources ${sources_lib} ${sources_stat} config.cpp various.cpp)

add_library(stend SHARED ${sources})
qt5_use_modules(stend Core)
target_link_libraries(stend ${OpenCV_LIBS} ${PC_LUA_LIBRARIES})

############################################################################

add_executable(console ui/console/main.cpp)
target_link_libraries(console stend)

#############################################################################

qt5_wrap_ui(dialogs ui/gui/resources/dialogs/main_window.ui ui/gui/resources/dialogs/about_dialog.ui)
qt5_add_resources(resources ui/gui/resources/resources.qrc)
set(sources_gui ${dialogs} ${resources} ui/gui/main.cpp ui/gui/ui/main_window.cpp ui/gui/ui/about_dialog.cpp)
add_executable(gui ${sources_gui})
qt5_use_modules(gui Core Widgets)
target_link_libraries(gui stend)

