cmake_minimum_required(VERSION 3.16)
project(simple-2d-cad VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

file(GLOB_RECURSE SOURCES
    "*.cpp"
    "*.h"
)
list(FILTER SOURCES EXCLUDE REGEX ".*/CMakeFiles/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*/out/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*/build/.*")

file(GLOB_RECURSE UI_FILES "*.ui")
file(GLOB_RECURSE QRC_FILES "*.qrc")

add_executable(simple-2d-cad
    ${SOURCES}
    ${UI_FILES}
    ${QRC_FILES}
)

target_link_libraries(simple-2d-cad PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
