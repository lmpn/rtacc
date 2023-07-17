include(GenerateExportHeader)

add_library(ingest file_ingestor.cpp)

add_library(rtacc::ingest ALIAS ingest)

target_link_libraries(ingest PRIVATE rtacc_options rtacc_warnings)

target_include_directories(ingest ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/>)

target_compile_features(ingest PUBLIC cxx_std_20)

set_target_properties(
  ingest
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(ingest EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/rtacc/ingest/ingest_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(ingest PUBLIC ingest_STATIC_DEFINE)
endif()
