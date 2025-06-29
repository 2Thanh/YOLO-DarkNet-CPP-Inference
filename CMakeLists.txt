cmake_minimum_required(VERSION 3.10)
project(CV_APP)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(cv_app
    src/main.cpp
    src/inference.cpp
)

target_link_libraries(cv_app ${OpenCV_LIBS})
