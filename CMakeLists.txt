cmake_minimum_required (VERSION 3.17.0)


################################################################################
## General settings
################################################################################

# Project name
project(PointLightShadowDemo)

# Prevent in source builds
include(cmake/PreventInSourceBuilds.cmake)

# Default settings
# Link this 'library' to set the c++ standard & compile-time options requested
include(cmake/StandardProjectSettings.cmake)
add_library(project_options INTERFACE)

# Use C++17 features
target_compile_features(project_options INTERFACE cxx_std_17)

# Warnings
# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
include(cmake/CompilerWarnings.cmake)
add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)


################################################################################
## Libraries
################################################################################

# OpenMP
find_package(OpenMP)

# GLFW
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "GLFW_BUILD_DOCS"     FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "GLFW_BUILD_TESTS"    FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW_BUILD_EXAMPLES" FORCE)
add_subdirectory(dep/glfw EXCLUDE_FROM_ALL)

# GLEW
set(BUILD_UTILS OFF CACHE BOOL "BUILD_UTILS" FORCE)
add_subdirectory(dep/glew/build/cmake EXCLUDE_FROM_ALL)

# GLM
add_subdirectory(dep/glm EXCLUDE_FROM_ALL)

# spdlog
add_subdirectory(dep/spdlog EXCLUDE_FROM_ALL)


################################################################################
## Executable 
################################################################################

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/log.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/mainApplication.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/mainApplication.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/input.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/input.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/glUtil.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/glUtil.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/camera.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/renderer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/renderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gameObject/gameObject.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/gameObject/gameObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/vertex.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/vertex.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/mesh.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/mesh.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/material.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/material.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/sceneGraph.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/sceneGraph.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/lightSource.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/lightSource.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/primitive.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/scene/primitive.cpp
)

# add executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# set include directories
target_include_directories(${CMAKE_PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/src
	       ${PROJECT_SOURCE_DIR}/dep/glew/include
	       ${PROJECT_SOURCE_DIR}/dep/stb_image
	       ${PROJECT_SOURCE_DIR}/dep/tiny_obj_loader
)

# link the libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
	PUBLIC glfw
	       glew_s
		   glm::glm
		   spdlog::spdlog
	       OpenMP::OpenMP_CXX
	PRIVATE project_options
	        project_warnings
)


################################################################################
## Visual Studio source  groups
################################################################################

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/.." FILES ${SOURCE_FILES})


################################################################################
## Copy assets
################################################################################

add_custom_target(copy_assets COMMAND 
	${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CopyAssets.cmake
)
add_dependencies(${CMAKE_PROJECT_NAME} copy_assets)


################################################################################
## Installation
################################################################################
install(TARGETS ${CMAKE_PROJECT_NAME}
        RUNTIME DESTINATION "${CMAKE_PROJECT_NAME}"
        ARCHIVE DESTINATION "${CMAKE_PROJECT_NAME}"
        LIBRARY DESTINATION "${CMAKE_PROJECT_NAME}"
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets 
	    DESTINATION "${CMAKE_PROJECT_NAME}"
)