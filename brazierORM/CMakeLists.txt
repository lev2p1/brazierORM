cmake_minimum_required(VERSION 3.14)
project(brazier-orm VERSION 0.1.0 LANGUAGES CXX)

if(MSVC)
    add_compile_options(/utf-8 /bigobj)
endif()

find_package(PostgreSQL REQUIRED)

add_executable(brazierORM "brazierORM.cpp" "brazierORM.h" "src/Logger.cpp" "src/Database.cpp" "src/SQLString.cpp")

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET brazierORM PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(brazierORM PRIVATE PostgreSQL::PostgreSQL)

find_package(GTest CONFIG REQUIRED)

get_target_property(GTEST_INCLUDE_DIRS GTest::gtest INTERFACE_INCLUDE_DIRECTORIES)

file(GLOB_RECURSE TEST_SOURCES 
    "src/*.cpp"
    "tests/*.cpp"
)

add_executable(brazier-orm_tests ${TEST_SOURCES})

target_include_directories(brazier-orm_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
    ${PostgreSQL_INCLUDE_DIRS}
)

target_compile_definitions(brazier-orm_tests PRIVATE
    BOOST_ASIO_HAS_CO_AWAIT
    BOOST_ASIO_HAS_STD_COROUTINE
)

target_compile_features(brazier-orm_tests PRIVATE cxx_std_20)

target_link_libraries(brazier-orm_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
    PostgreSQL::PostgreSQL
)

add_custom_target(check
    COMMAND $<TARGET_FILE:brazier-orm_tests>
    DEPENDS brazier-orm_tests
    COMMENT "Running tests..."
)