project(RandomizedQueue)

add_library(${PROJECT_NAME} include/${PROJECT_NAME}.hpp
                            include/Subset.hpp               src/Subset.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_library(big1::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(tests tests/testIterator.hpp tests/testRandomizedQueue.cpp tests/testSubset.cpp)
target_link_libraries(tests PRIVATE GTest::GTest big1::${PROJECT_NAME})
gtest_discover_tests(tests)

if(COMPILE_OPTS)
    target_compile_options(${PROJECT_NAME} PUBLIC ${COMPILE_OPTS})
    target_link_options(${PROJECT_NAME} PUBLIC ${LINK_OPTS})

    target_compile_options(tests PUBLIC ${COMPILE_OPTS})
    target_link_options(tests PUBLIC ${LINK_OPTS})
endif()
