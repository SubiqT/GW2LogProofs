# Cross-compiles the Windows x64 DLL from macOS or Linux with clang-cl, lld-link and the MSVC CRT and Windows SDK
# downloaded by xwin (https://github.com/Jake-Shadle/xwin). Override XWIN or LLVM for non-default locations.
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

if(NOT DEFINED XWIN)
    set(XWIN "$ENV{XWIN}")
endif()
if(NOT XWIN)
    set(XWIN "$ENV{HOME}/Library/Caches/cargo-xwin/xwin")
endif()
if(NOT EXISTS "${XWIN}/crt/include")
    message(FATAL_ERROR "No Windows SDK at ${XWIN}; run `xwin --accept-license splat --output ${XWIN}` or set XWIN.")
endif()

if(NOT DEFINED LLVM)
    set(LLVM "$ENV{LLVM}")
endif()
if(NOT LLVM)
    execute_process(COMMAND brew --prefix llvm OUTPUT_VARIABLE LLVM OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()
find_program(LLD_LINK lld-link HINTS "${LLVM}/bin" REQUIRED)

set(CMAKE_C_COMPILER "${LLVM}/bin/clang-cl")
set(CMAKE_CXX_COMPILER "${LLVM}/bin/clang-cl")
set(CMAKE_RC_COMPILER "${LLVM}/bin/llvm-rc")
set(CMAKE_LINKER "${LLD_LINK}")
set(CMAKE_MT "${LLVM}/bin/llvm-mt")

set(XWIN_INCLUDES "-imsvc \"${XWIN}/crt/include\" -imsvc \"${XWIN}/sdk/include/ucrt\" -imsvc \"${XWIN}/sdk/include/um\" -imsvc \"${XWIN}/sdk/include/shared\"")
set(CMAKE_C_FLAGS_INIT "--target=x86_64-pc-windows-msvc ${XWIN_INCLUDES}")
set(CMAKE_CXX_FLAGS_INIT "--target=x86_64-pc-windows-msvc ${XWIN_INCLUDES}")
set(CMAKE_RC_FLAGS_INIT "/I \"${XWIN}/sdk/include/um\" /I \"${XWIN}/sdk/include/shared\"")

set(XWIN_LIBPATHS "/LIBPATH:\"${XWIN}/crt/lib/x86_64\" /LIBPATH:\"${XWIN}/sdk/lib/um/x86_64\" /LIBPATH:\"${XWIN}/sdk/lib/ucrt/x86_64\"")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "${XWIN_LIBPATHS} /MANIFEST:NO")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${XWIN_LIBPATHS} /MANIFEST:NO")

# xwin downloads only the release CRT libraries by default, so compiler checks must not build Debug.
set(CMAKE_TRY_COMPILE_CONFIGURATION Release)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
