include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(rtacc_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project
  cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

  find_package(Threads REQUIRED)

  CPMAddPackage("gh:axboe/liburing#liburing-2.4@2.4")
  if(NOT TARGET uring)
    add_library(uring INTERFACE)
    MESSAGE("here ${liburing_SOURCE_DIR}")
    target_include_directories(uring SYSTEM INTERFACE ${liburing_SOURCE_DIR}/src/include)
  endif()

  CPMAddPackage("gh:chriskohlhoff/asio#asio-1-28-0@1.28.0")

  # ASIO doesn't use CMake, we have to configure it manually. Extra notes for using on Windows:
  #
  # 1) If _WIN32_WINNT is not set, ASIO assumes _WIN32_WINNT=0x0501, i.e. Windows XP target, which is
  # definitely not the platform which most users target.
  #
  # 2) WIN32_LEAN_AND_MEAN is defined to make Winsock2 work.
  if(asio_ADDED)
    add_library(asio INTERFACE)

    target_include_directories(asio SYSTEM INTERFACE ${asio_SOURCE_DIR}/asio/include)

    target_compile_definitions(asio INTERFACE ASIO_STANDALONE ASIO_NO_DEPRECATED)

    target_link_libraries(asio INTERFACE Threads::Threads)

    if(WIN32)
      # macro see @ https://stackoverflow.com/a/40217291/1746503
      macro(get_win32_winnt version)
        if(CMAKE_SYSTEM_VERSION)
          set(ver ${CMAKE_SYSTEM_VERSION})
          string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
          string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
          # Check for Windows 10, b/c we'll need to convert to hex 'A'.
          if("${verMajor}" MATCHES "10")
            set(verMajor "A")
            string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
          endif("${verMajor}" MATCHES "10")
          # Remove all remaining '.' characters.
          string(REPLACE "." "" ver ${ver})
          # Prepend each digit with a zero.
          string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
          set(${version} "0x${ver}")
        endif()
      endmacro()

      if(NOT DEFINED _WIN32_WINNT)
        get_win32_winnt(ver)
        set(_WIN32_WINNT ${ver})
      endif()

      message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")

      target_compile_definitions(asio INTERFACE _WIN32_WINNT=${_WIN32_WINNT} WIN32_LEAN_AND_MEAN)
    endif()
  endif()

  if(NOT TARGET fmtlib::fmtlib)
    cpmaddpackage("gh:fmtlib/fmt#9.1.0")
  endif()

  if(NOT TARGET spdlog::spdlog)
    cpmaddpackage(
      NAME
      spdlog
      VERSION
      1.11.0
      GITHUB_REPOSITORY
      "gabime/spdlog"
      OPTIONS
      "SPDLOG_FMT_EXTERNAL ON")
  endif()

  if(NOT TARGET Catch2::Catch2WithMain)
    cpmaddpackage("gh:catchorg/Catch2@3.3.2")
  endif()

  if(NOT TARGET CLI11::CLI11)
    cpmaddpackage("gh:CLIUtils/CLI11@2.3.2")
  endif()

endfunction()
