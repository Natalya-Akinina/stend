
cmake_minimum_required(VERSION 2.8)

project(stend)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pipe -std=c++11" CACHE string "" FORCE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${Qt5Widgets_INCLUDE_DIRS})

if(not ${UNIX})

	set(CMAKE_LIBRARY_PATH "C:/super_stend/windows/bin" CACHE string "" FORCE)
	link_directories("C:/super_stend/windows/bin")
	include_directories("C:/super_stend/windows/include")

endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "")

	#
	# Цель по умолчанию - Debug
	# Для оптимизированной цели: cmake -DCMAKE_BUILD_TYPE=Release ..
	#

	set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)

endif()

############################################################################

qt5_wrap_ui(dialogs ui/gui/resources/dialogs/main_window.ui ui/gui/resources/dialogs/about_dialog.ui)
qt5_add_resources(resources ui/gui/resources/resources.qrc)

set(sources_lib lib/lua.cpp lib/opencv.cpp lib/module.cpp lib/matrix.cpp lib/image.cpp)
set(sources_stat stat/stat.cpp)
set(sources_main_loop main_loop/main_loop.cpp)
set(sources ${sources_lib} ${sources_stat} ${sources_main_loop} config.cpp various.cpp)
set(sources_console ui/console/main.cpp ui/console/display.cpp)
set(sources_gui ${dialogs} ${resources} ui/gui/main.cpp ui/gui/ui/display.cpp ui/gui/ui/main_window.cpp ui/gui/ui/about_dialog.cpp ui/gui/ui/lua_highlighter.cpp)

############################################################################ 

if(${UNIX})

	find_package(PkgConfig REQUIRED)
	pkg_check_modules(PC_LUA QUIET lua)

	string(REPLACE ";" " " PC_LUA_CFLAGS " ${PC_LUA_CFLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PC_LUA_CFLAGS} -Wall -pipe -std=c++11" CACHE string "" FORCE)

	add_library(stend SHARED ${sources})
	qt5_use_modules(stend Core Test)
	target_link_libraries(stend ${OpenCV_LIBS} ${PC_LUA_LIBRARIES})

	add_executable(console ${sources_console})
	add_executable(gui ${sources_gui})

	set(lib_4_binary stend)

else()

	add_executable(console ${sources_console} ${sources})
	add_executable(gui ${sources_gui} ${sources})

	set(lib_4_binary ${OpenCV_LIBS} lua)

endif()

qt5_use_modules(console Core Test)
target_link_libraries(console ${lib_4_binary})

qt5_use_modules(gui Core Test Widgets)
target_link_libraries(gui ${lib_4_binary})

