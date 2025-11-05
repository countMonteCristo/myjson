project(myjson-unit-test)

# Find CppUnit
find_package(CppUnit REQUIRED)

IF(${CPPUNIT_FOUND} MATCHES "NO")
    message(FATAL_ERROR "Can not find cppunit library for testing")
ENDIF(${CPPUNIT_FOUND} MATCHES "NO")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(TEST_SOURCES ${TEST_SOURCES})

add_executable(myjson-unit-test
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${TEST_SOURCES}
)

target_link_libraries(myjson-unit-test
    ${CPPUNIT_LIBRARIES}
    myjson_static
)

# Add the test to CTest
add_test(NAME myjson-unit-tests COMMAND myjson-unit-test)