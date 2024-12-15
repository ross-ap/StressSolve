cmake_minimum_required(VERSION 3.16)

project(StressSolveApp VERSION 1.0.0 LANGUAGES CXX)

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

qt_add_executable(StressSolveApp ${SRC_FILES}) 
qt_add_resources(StressSolveApp "resources/StressSolveApp.qrc") 

target_link_libraries(StressSolveApp PRIVATE Qt6::Widgets Qt6::Core CURL::libcurl)
set_target_properties(StressSolveApp PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

add_custom_command(TARGET StressSolveApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/bearer_token.txt
        ${CMAKE_BINARY_DIR}/Debug/bearer_token.txt
)

