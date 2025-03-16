cmake_minimum_required(VERSION 3.16)

project(Renderer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 首先设置包含目录，确保可以找到头文件
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 收集源文件


# 设置项目源文件
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resource.qrc
    maths.cpp
    model.cpp
    tgaimage.cpp
    shader.cpp
    phong_shader.cpp
)

# 添加可执行文件
qt_add_executable(Renderer
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    resource.qrc
)

# 链接库
target_link_libraries(Renderer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# 其他属性设置
set_target_properties(Renderer PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 安装目标
include(GNUInstallDirs)
install(TARGETS Renderer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 完成设置
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Renderer)
endif()