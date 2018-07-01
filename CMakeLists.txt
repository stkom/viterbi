cmake_minimum_required(VERSION 3.10)
project(viterbi)

find_package(SystemCLanguage CONFIG)

if (SystemCLanguage_FOUND)
    set(CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})
else ()
    set(CMAKE_CXX_STANDARD 14)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SYSTEMC REQUIRED systemc)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CXX_WARNINGS "-Weverything -Wno-c++98-compat -Wno-weak-vtables \
        -Wno-missing-noreturn")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CXX_WARNINGS "-Wall -Wextra -Wpedantic -Wformat=2 \
        -Wuninitialized -Wfloat-equal -Wundef -Wshadow -Wcast-qual \
        -Wcast-align -Wconversion -Wuseless-cast -Wlogical-op \
        -Wmissing-declarations -Wredundant-decls -Winline \
        -Wdisabled-optimization -Wnoexcept -Wnon-virtual-dtor \
        -Wold-style-cast -Woverloaded-virtual -Wsign-promo")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS}")

add_executable(viterbi
        main.cpp
        decoder.cpp decoder.h
        decoder_test.cpp decoder_test.h
        encoder.h
        encoder_test.cpp encoder_test.h)

if (SystemCLanguage_FOUND)
    target_link_libraries(viterbi SystemC::systemc)
else ()
    target_link_libraries(viterbi ${SYSTEMC_LIBRARIES})
    target_include_directories(viterbi PUBLIC ${SYSTEMC_INCLUDE_DIRS})
    target_compile_options(viterbi PUBLIC ${SYSTEMC_CFLAGS_OTHER})
endif ()
