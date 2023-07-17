include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(rtacc_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(rtacc_setup_options)
  option(rtacc_ENABLE_HARDENING "Enable hardening" ON)
  option(rtacc_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    rtacc_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    rtacc_ENABLE_HARDENING
    OFF)

  rtacc_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR rtacc_PACKAGING_MAINTAINER_MODE)
    option(rtacc_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(rtacc_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(rtacc_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(rtacc_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(rtacc_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(rtacc_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(rtacc_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(rtacc_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(rtacc_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(rtacc_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(rtacc_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(rtacc_ENABLE_PCH "Enable precompiled headers" OFF)
    option(rtacc_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(rtacc_ENABLE_IPO "Enable IPO/LTO" ON)
    option(rtacc_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(rtacc_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(rtacc_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(rtacc_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(rtacc_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(rtacc_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(rtacc_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(rtacc_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(rtacc_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(rtacc_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(rtacc_ENABLE_PCH "Enable precompiled headers" OFF)
    option(rtacc_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      rtacc_ENABLE_IPO
      rtacc_WARNINGS_AS_ERRORS
      rtacc_ENABLE_USER_LINKER
      rtacc_ENABLE_SANITIZER_ADDRESS
      rtacc_ENABLE_SANITIZER_LEAK
      rtacc_ENABLE_SANITIZER_UNDEFINED
      rtacc_ENABLE_SANITIZER_THREAD
      rtacc_ENABLE_SANITIZER_MEMORY
      rtacc_ENABLE_UNITY_BUILD
      rtacc_ENABLE_CLANG_TIDY
      rtacc_ENABLE_CPPCHECK
      rtacc_ENABLE_COVERAGE
      rtacc_ENABLE_PCH
      rtacc_ENABLE_CACHE)
  endif()

  rtacc_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (rtacc_ENABLE_SANITIZER_ADDRESS OR rtacc_ENABLE_SANITIZER_THREAD OR rtacc_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(rtacc_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(rtacc_global_options)
  if(rtacc_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    rtacc_enable_ipo()
  endif()

  rtacc_supports_sanitizers()

  if(rtacc_ENABLE_HARDENING AND rtacc_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR rtacc_ENABLE_SANITIZER_UNDEFINED
       OR rtacc_ENABLE_SANITIZER_ADDRESS
       OR rtacc_ENABLE_SANITIZER_THREAD
       OR rtacc_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${rtacc_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${rtacc_ENABLE_SANITIZER_UNDEFINED}")
    rtacc_enable_hardening(rtacc_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(rtacc_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(rtacc_warnings INTERFACE)
  add_library(rtacc_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  rtacc_set_project_warnings(
    rtacc_warnings
    ${rtacc_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(rtacc_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    configure_linker(rtacc_options)
  endif()

  include(cmake/Sanitizers.cmake)
  rtacc_enable_sanitizers(
    rtacc_options
    ${rtacc_ENABLE_SANITIZER_ADDRESS}
    ${rtacc_ENABLE_SANITIZER_LEAK}
    ${rtacc_ENABLE_SANITIZER_UNDEFINED}
    ${rtacc_ENABLE_SANITIZER_THREAD}
    ${rtacc_ENABLE_SANITIZER_MEMORY})

  set_target_properties(rtacc_options PROPERTIES UNITY_BUILD ${rtacc_ENABLE_UNITY_BUILD})

  if(rtacc_ENABLE_PCH)
    target_precompile_headers(
      rtacc_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(rtacc_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    rtacc_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(rtacc_ENABLE_CLANG_TIDY)
    rtacc_enable_clang_tidy(rtacc_options ${rtacc_WARNINGS_AS_ERRORS})
  endif()

  if(rtacc_ENABLE_CPPCHECK)
    rtacc_enable_cppcheck(${rtacc_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(rtacc_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    rtacc_enable_coverage(rtacc_options)
  endif()

  if(rtacc_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(rtacc_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(rtacc_ENABLE_HARDENING AND NOT rtacc_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR rtacc_ENABLE_SANITIZER_UNDEFINED
       OR rtacc_ENABLE_SANITIZER_ADDRESS
       OR rtacc_ENABLE_SANITIZER_THREAD
       OR rtacc_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    rtacc_enable_hardening(rtacc_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
