cmake_minimum_required(VERSION 3.16)

project(CMakeTestApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
find_package(CURL REQUIRED)
qt_standard_project_setup()

include_directories(include)

set(SRC_FILES
    include/DataHandler.h
    src/DataHandler.cpp
    include/DecisionTree.h
    src/DecisionTree.cpp
    include/Node.h
    src/Node.cpp
    include/RandomForest.h
    src/RandomForest.cpp
    include/stdafx.h
    src/stdafx.cpp
    include/StressSolve.h
    src/StressSolve.cpp
    include/StressSolveApp.h
    src/StressSolveApp.cpp
    include/Student.h
    src/Student.cpp
    include/SuggestionMaker.h
    src/SuggestionMaker.cpp
    include/json.hpp
    include/StressSolveApp.ui
    src/main.cpp
)

qt_add_executable(CMakeTestApp WIN32 ${SRC_FILES}) 

file(GLOB RESOURCE_FILES "resources/*.qrc")
set(CMAKE_MACRO_RESOURCE ${RESOURCE_FILES})
qt_add_resources(CMakeTestApp ${CMAKE_MACRO_RESOURCE}) 

target_link_libraries(CMakeTestApp PRIVATE Qt6::Widgets Qt6::Core CURL::libcurl)
set_target_properties(CMakeTestApp PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
