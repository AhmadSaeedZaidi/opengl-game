file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_library(window ${SOURCES} ${HEADERS})
target_include_directories(window PUBLIC include)
target_link_libraries(window PRIVATE glad glfw shaders stb_image shape glm)


add_subdirectory(test)