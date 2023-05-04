cmake_minimum_required(VERSION 3.16)

project(RBTree VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(RBTree
    widget.ui
    application.cpp
    controller.cpp
    graphicsnode.cpp
    main.cpp
    rbtree.cpp
    widget.cpp
)

target_link_libraries(RBTree PRIVATE Qt6::Widgets)

set_target_properties(RBTree PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
