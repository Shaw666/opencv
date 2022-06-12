cmake_minimum_required(VERSION 3.5)

project(FaceDetect VERSION 0.1 LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(platform "64" CACHE STRING "")
else()
    set(platform "32" CACHE STRING "")
endif()

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Qt/Qt5.15.2/5.15.2/msvc2019_64" "C:/Qt/5.15.2/msvc2019_64")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
# SET(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO "RelWithDebInfo;Release;")
find_package(OpenCV PATHS "../../opencv/install/x64/vc16/lib" REQUIRED)
set_target_properties(${OpenCV_LIBS} PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)
set_target_properties(${OpenCV_LIBS} PROPERTIES MAP_IMPORTED_CONFIG_MINSIZEREL RELEASE)

FILE(GLOB SRC_FILES *.cpp *.h)


set(PROJECT_SOURCES
    ${SRC_FILES}
    # ${RC_FILES}
    # ${TS_FILES}
    "mainwindow.ui"
)

# include_directories("../../opencv_contrib-4.5.5/modules/face/include")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FaceDetect
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FaceDetect APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(FaceDetect SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FaceDetect
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(FaceDetect PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})



set_target_properties(FaceDetect PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FaceDetect)
endif()

if(WIN32)
SET(EXECUTABLE_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/${platform})
else()
SET(EXECUTABLE_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/${platform}/${CMAKE_BUILD_TYPE})
endif()