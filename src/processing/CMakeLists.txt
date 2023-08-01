include(GenerateExportHeader)

add_library(processing manager.cpp node.cpp asio_thread_pool_executor.cpp executor_factory.cpp)

add_library(rtacc::processing ALIAS processing)

target_link_libraries(processing PRIVATE rtacc_options rtacc_warnings asio)

target_include_directories(processing ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/>)
target_include_directories(processing PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/")
target_compile_features(processing PUBLIC cxx_std_20)
set_target_properties(
  processing
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(processing EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/rtacc/processing/processing_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(processing PUBLIC processing_STATIC_DEFINE)
endif()
