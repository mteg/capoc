cmake_minimum_required(VERSION 3.6)   # CMake version check
project(capoc2)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard
#set(WXWINDOWS_USE_GL 1)

find_package(OpenGL REQUIRED)
find_package(wxWidgets COMPONENTS core base gl REQUIRED)
include( "${wxWidgets_USE_FILE}" )


#set(SOURCE_FILES capaffine.cpp capmodel.cpp capmodel_off.cpp capdebug.cpp capmodel_selection.cpp cli.cpp cli_screen.cpp tcpserver.cpp capnvm.cpp svd.cpp caprenderer.cpp caprenderer_ancientgl.cpp glutcapoc.cpp capengine.cpp capcli.cpp capserver.cpp capmodel_transforms.cpp)            # Add main.cpp file of project root directory as source file
#add_executable(captest ${SOURCE_FILES})       # Add executable target with source files listed in SOURCE_FILES variable
#target_link_libraries(captest ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

#set(SOURCE_FILES example.cpp capaffine.cpp capmodel.cpp capmodel_off.cpp capdebug.cpp capmodel_selection.cpp cli.cpp cli_screen.cpp tcpserver.cpp capnvm.cpp svd.cpp caprenderer.cpp caprenderer_ancientgl.cpp glutcapoc.cpp capengine.cpp capcli.cpp capserver.cpp capmodel_transforms.cpp)            # Add main.cpp file of project root directory as source file
#add_executable(example ${SOURCE_FILES})       # Add executable target with source files listed in SOURCE_FILES variable
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
# Update if necessary
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")
endif()


set(SOURCE_FILES wxcapoc.cpp wxcapoc_canvas.cpp wxcapoc_main.cpp wxcapoc_keymaps.cpp wxcapoc_tree.cpp wxcapoc_console.cpp wxcapoc_editpoint.cpp wxcapoc_editmatrix.cpp wxcapoc_logbrowser.cpp capaffine.cpp capmodel.cpp capmodel_off.cpp capmodel_ply.cpp capmodel_3d.cpp capmodel_nvm.cpp ply_io.h ply_io.c capdebug.cpp capmodel_fitplane.cpp capmodel_selection.cpp cli.cpp cli_screen.cpp tcpserver.cpp capnvm.cpp svd.cpp caprenderer.cpp caprenderer_ancientgl.cpp capengine.cpp capcli.cpp capserver.cpp capmodel_transforms.cpp img.c lodepng.cpp)            # Add main.cpp file of project root directory as source file
add_executable(wxcapoc ${SOURCE_FILES})       # Add executable target with source files listed in SOURCE_FILES variable
target_link_libraries(wxcapoc ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
