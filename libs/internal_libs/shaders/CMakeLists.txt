file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_library(shaders ${SOURCES} ${HEADERS})
target_include_directories(shaders PUBLIC include)
target_link_libraries(shaders PRIVATE glad glfw PUBLIC glm)

add_subdirectory(test)