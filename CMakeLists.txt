cmake_minimum_required(VERSION 3.23.2)

set (CMAKE_CXX_STANDARD 17)

project(main)

file(GLOB srcFiles 
    src/*.cpp 
    src/game/scripting/*.cpp 
    src/game/map/*.cpp
)

add_executable(${PROJECT_NAME}
    ${srcFiles}
)

# add_subdirectory(util)
# add_subdirectory(external/glfw)

# target_include_directories(${PROJECT_NAME} 
    # PUBLIC util 
    # PUBLIC external/glfw/include
# )

# target_link_directories(${PROJECT_NAME} 
    # PRIVATE util
    # PRIVATE external/glfw/src
# )

# target_link_libraries(${PROJECT_NAME} util glfw)