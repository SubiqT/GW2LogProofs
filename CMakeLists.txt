cmake_minimum_required(VERSION 3.21)

# Date-based version, matching what Nexus shows for releases: year.month.day.minute-of-day (UTC).
string(TIMESTAMP LOG_PROOFS_YEAR "%Y" UTC)
string(TIMESTAMP LOG_PROOFS_MONTH "%m" UTC)
string(TIMESTAMP LOG_PROOFS_DAY "%d" UTC)
string(TIMESTAMP LOG_PROOFS_HOUR "%H" UTC)
string(TIMESTAMP LOG_PROOFS_MINUTE "%M" UTC)
math(EXPR LOG_PROOFS_MONTH "${LOG_PROOFS_MONTH}")
math(EXPR LOG_PROOFS_DAY "${LOG_PROOFS_DAY}")
math(EXPR LOG_PROOFS_REVISION "${LOG_PROOFS_HOUR} * 60 + ${LOG_PROOFS_MINUTE}")

project(GW2LogProofs
    VERSION ${LOG_PROOFS_YEAR}.${LOG_PROOFS_MONTH}.${LOG_PROOFS_DAY}.${LOG_PROOFS_REVISION}
    LANGUAGES C CXX RC)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows" OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Log Proofs is a 64-bit Windows DLL; configure with a Windows x64 toolchain or preset.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Nexus addons ship as a single DLL, so link the C++ runtime statically.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

configure_file(cmake/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/generated/version.h @ONLY)

file(GLOB_RECURSE LOG_PROOFS_SOURCES CONFIGURE_DEPENDS
    src/core/*.cpp
    src/providers/*.cpp
    src/ui/*.cpp
    src/utils/*.cpp)

add_library(nexus_log_proofs SHARED
    src/entry.cpp
    ${LOG_PROOFS_SOURCES}
    src/imgui/imgui.cpp
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_widgets.cpp
    src/resource.rc)

target_include_directories(nexus_log_proofs PRIVATE src ${CMAKE_CURRENT_BINARY_DIR}/generated)
target_compile_definitions(nexus_log_proofs PRIVATE
    GW2LOGPROOFS_EXPORTS _WINDOWS _USRDLL _DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR)
target_compile_options(nexus_log_proofs PRIVATE /Zc:__cplusplus /utf-8 /Zi)
target_link_options(nexus_log_proofs PRIVATE /DEBUG $<$<NOT:$<CONFIG:Debug>>:/OPT:REF /OPT:ICF>)
target_link_libraries(nexus_log_proofs PRIVATE winhttp shell32)

install(TARGETS nexus_log_proofs RUNTIME DESTINATION .)
