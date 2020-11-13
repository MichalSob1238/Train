# CMake entry point.
# Note that this CMakeLists.txt in fully independent from the one in the parent directory
# (since that's the goal of this tutorial !)

# For an introduction to CMake, see
# http://www.cmake.org/cmake/help/cmake_tutorial.html (at least steps 1 and 2)

cmake_minimum_required (VERSION 2.6)
project (gl_project)
#add_compile_options()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})
set(OpenGL_GL_PREFERENCE LEGACY )#GLVND or LEGACY

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(SOIL REQUIRED)
find_package(GLM REQUIRED)



# Ask the user to give the paths to GLFW, GLEW and GLM.
# On Linux this can be found automatically with pkg-config; 
# see http://stackoverflow.com/questions/6889264/cmake-flags-for-opengl-using-glew-and-glfw, for instance.

set(GLFW_INCLUDE_PATH "" CACHE PATH "The directory that contains GL/glfw.h" )
set(GLFW_LIBRARY "" CACHE FILEPATH "libglfw.a or glfw.lib" )

set(GLEW_INCLUDE_PATH "" CACHE PATH "The directory that contains GL/glew.h" )
set(GLEW_LIBRARY "" CACHE FILEPATH "libglew.a or glew.lib" )

set(GLM_INCLUDE_PATH "" CACHE PATH "The directory that contains glm/glm.hpp" )


file(GLOB_RECURSE MY_HEADER_FILES ${CMAKE_SOURCE_DIR} "files/*.cpp")
file(GLOB_RECURSE MY_SOURCE_FILES ${CMAKE_SOURCE_DIR} "*.h*")


include_directories(
	${GLFW_INCLUDE_PATH}
	${GLEW_INCLUDE_PATH}
	${SOIL_INCLUDE_PATH}/SOIL
	${GLM_INCLUDE_PATH}
)
add_executable(gl_project
	${MY_HEADER_FILES}
	${MY_SOURCE_FILES}
)
target_link_libraries(gl_project
	${OPENGL_LIBRARY} # filled by "find_package(OpenGL REQUIRED)"
	${GLFW_LIBRARIES}
	${GLEW_LIBRARY}
	${OPENGL_LIBRARIES}
	${SOIL_LIBRARY}
)



# HOW-TO. You can safely remove anything below.  

# test if linux, mac or windows : 

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("You're on Mac OS !")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("You're on Linux !")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("You're on Windows !")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # Yes this is weird but you have to repeat the "if" here.

#message(${MY_HEADER_FILES})
#message(${MY_SOURCE_FILES})
#message(LIB ${GLFW_LIBRARIES})
message(XD ${GLFW_LIBRARIES})
