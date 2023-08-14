cmake_minimum_required(VERSION 3.26)
project(AttendanceCheck_linux)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3 -D ${CMAKE_BUILD_TYPE}")
set(PROJECT_VERSION "1.0")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(CMAKE_PREFIX_PATH /home/jackie/Qt/5.15.2/gcc_64/lib/cmake)
elseif (${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(CROSS_LIB_PREFIX /home/jackie/Tools/Platforms)
    set(CMAKE_PREFIX_PATH
            ${CROSS_LIB_PREFIX}/qt-5.15.2-aarch64/lib/cmake
            ${CROSS_LIB_PREFIX}/opencv-4.8.0-aarch64/lib/cmake
            ${CROSS_LIB_PREFIX}/dlib-19.24-aarch64/lib/cmake
    )
endif ()

include_directories(gui utils)
file(GLOB_RECURSE PROJ_SRC gui/*.cpp gui/*.h utils/*.cpp utils/*.h)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
find_package(OpenCV REQUIRED)
find_package(dlib REQUIRED)

add_executable(AttendanceCheck_linux main.cpp ${PROJ_SRC})
target_link_libraries(AttendanceCheck_linux
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        ${OpenCV_LIBS}
        dlib::dlib
)

if (${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(PROJECT_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}_v${PROJECT_VERSION}")
    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_OUTPUT_PATH}")
    set(LIBRARY_OUTPUT_PATH "${PROJECT_OUTPUT_PATH}")

    # create data dir
    if (NOT EXISTS "${PROJECT_OUTPUT_PATH}/data")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "${PROJECT_OUTPUT_PATH}/data")
    endif ()

    # copy model files
    if (NOT EXISTS "${PROJECT_OUTPUT_PATH}/models")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "${PROJECT_OUTPUT_PATH}/models")
    endif ()
    file(GLOB MODELS models/*.dat)
    foreach (model ${MODELS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${model}" "${PROJECT_OUTPUT_PATH}/models")
    endforeach ()


endif ()