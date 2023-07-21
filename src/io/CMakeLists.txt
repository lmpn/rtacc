include(GenerateExportHeader)

add_library(io file.cpp)

add_library(rtacc::io ALIAS io)

target_link_libraries(io PRIVATE rtacc_options rtacc_warnings asio)

target_include_directories(io ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/>)
target_include_directories(intro PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/")
target_compile_features(io PUBLIC cxx_std_20)

set_target_properties(
  io
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(io EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/rtacc/io/io_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(io PUBLIC io_STATIC_DEFINE)
endif()
