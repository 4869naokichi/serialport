add_library(serialport SHARED)
target_sources(serialport PRIVATE serialport.cpp)
target_include_directories(serialport PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(serialport speak)

add_library(speak SHARED IMPORTED GLOBAL)
set_target_properties(speak
    PROPERTIES
        IMPORTED_LOCATION ${PROJECT_BINARY_DIR}/out/libspeak.dll
        IMPORTED_IMPLIB ${PROJECT_BINARY_DIR}/out/libspeak.dll.a
        INTERFACE_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/include)