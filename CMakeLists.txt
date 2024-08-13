cmake_minimum_required(VERSION 3.5)

project(carousel VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

qt_add_executable(carousel MANUAL_FINALIZATION ${PROJECT_SOURCES}
    galleryview.h galleryview.cpp
    imageview.h imageview.cpp
    commonresources.qrc
    gallery.h gallery.cpp)

target_link_libraries(carousel PRIVATE Qt6::Widgets)
qt_finalize_executable(carousel)

