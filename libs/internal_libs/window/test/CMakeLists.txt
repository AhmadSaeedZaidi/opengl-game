file(GLOB TEST_SOURCES "*.cpp")

# Loop over each test source file
foreach(test_src ${TEST_SOURCES})
    # Get the file name without extension to name the test
    get_filename_component(test_name ${test_src} NAME_WE)

    # Create executable for each test
    add_executable(${test_name} ${test_src})

    # Link with your main library if needed
    target_link_libraries(${test_name} window gtest gtest_main glad glfw)

    # Register with CTest
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
