file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_library(shape ${SOURCES} ${HEADERS})
target_include_directories(shape PUBLIC include)
target_link_libraries(shape PRIVATE glad glfw shaders stb_image window)

