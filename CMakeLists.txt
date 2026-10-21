cmake_minimum_required(VERSION 3.20)
project(Lab03_DateSet_Variant4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

add_library(lab_03_core STATIC
    src/date.cpp
    src/date_set.cpp
)
target_include_directories(lab_03_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(MSVC)
    target_compile_options(lab_03_core PRIVATE /W4)
else()
    target_compile_options(lab_03_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang" AND NOT MSVC)
        target_compile_options(lab_03_core PUBLIC
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
            -g
        )
        target_link_options(lab_03_core PUBLIC -fsanitize=address,undefined)
    else()
        message(FATAL_ERROR "ENABLE_SANITIZERS requires GCC or Clang with ASan and UBSan support")
    endif()
endif()

add_executable(lab_03 src/main.cpp)
target_link_libraries(lab_03 PRIVATE lab_03_core)

include(CTest)

if(BUILD_TESTING)
    find_package(GTest QUIET)
    if(NOT GTest_FOUND)
        include(FetchContent)
        if(POLICY CMP0135)
            cmake_policy(SET CMP0135 NEW)
        endif()

        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
        FetchContent_Declare(googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
        )
        FetchContent_MakeAvailable(googletest)
    endif()

    add_executable(lab_03_tests
        tests/test_date.cpp
        tests/test_date_set.cpp
    )
    target_link_libraries(lab_03_tests PRIVATE lab_03_core GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(lab_03_tests)
endif()
