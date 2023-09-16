add_executable(intro main.cpp)

target_link_libraries(
  intro
  PRIVATE rtacc::rtacc_options
          rtacc::rtacc_warnings)

target_link_system_libraries(
  intro
  PRIVATE
          CLI11::CLI11
          fmt::fmt
          spdlog::spdlog
          rtacc::ingest
          rtacc::processing
          AudioFile
)

target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
target_include_directories(intro PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/")
