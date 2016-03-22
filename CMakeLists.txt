cmake_minimum_required(VERSION 3.3)
project(OpenGL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#添加依赖库,glfw在Mac OS X中需要使用这些库
IF(APPLE)
    INCLUDE_DIRECTORIES ( /Developer/Headers/FlatCarbon )
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(QYARTZCORE_LIBRARY QuartzCore )
    FIND_LIBRARY(IOKIT_SERVICES_LIBRARY IOKit )
    FIND_LIBRARY(FOUNDATION_SERVICES_LIBRARY Foundation )
    FIND_LIBRARY(OPENGL_LIBRARY OpenGL )
    MARK_AS_ADVANCED (  COCOA_LIBRARY
            QYARTZCORE_LIBRARY
            IOKIT_SERVICES_LIBRARY
            FOUNDATION_SERVICES_LIBRARY
            OPENGL_LIBRARY)
    SET(EXTRA_LIBS ${COCOA_LIBRARY} ${QYARTZCORE_LIBRARY} ${IOKIT_SERVICES_LIBRARY} ${FOUNDATION_SERVICES_LIBRARY} ${OPENGL_LIBRARY})
ENDIF (APPLE)

#指定根目录所在的位置
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#工程目录的文件组织,确定其他资源的路径

#资源目录(图像,着色器文件等)
set(RESOURCE_DIR ${ROOT_DIR}/resource)

#用户源代码路径
set(SOURCE_FILE_DIR ${ROOT_DIR}/qyt)
aux_source_directory(${SOURCE_FILE_DIR} CUSTOM_SOURCE_FILES)

#添加编译文件
set(SOURCE_FILES
        main.cpp
        ${ROOT_DIR}/GL/glew.c
        ${CUSTOM_SOURCE_FILES})

#添加头文件包含目录
include_directories(${ROOT_DIR})

#添加目标
add_executable(OpenGL ${SOURCE_FILES})

#添加外部依赖库
target_link_libraries(OpenGL ${ROOT_DIR}/lib/libglfw3.a ${EXTRA_LIBS})

#设置编译输出目录
SET_TARGET_PROPERTIES(
        OpenGL
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${SOURCE_FILE_DIR}/bin")