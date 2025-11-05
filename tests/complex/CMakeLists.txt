project(myjson-complex-test)

add_executable(process_json main.cpp)

target_link_libraries(process_json
    myjson_static
)

# add_custom_target(run_my_script
#         COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/my_script.cmake
#         COMMENT "Running a custom CMake script"
#     )

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})

# add_custom_command(
#     TARGET process_json
#     POST_BUILD
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#     COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run.sh
# )

add_custom_target(
    myjson-complex-test
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run.sh
    # COMMENT "Running my custom script..."
    DEPENDS process_json # Optional: specify dependencies if this script relies on other targets
)
