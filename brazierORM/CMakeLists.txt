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