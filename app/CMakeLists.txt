get_all_sources("${CMAKE_CURRENT_SOURCE_DIR}/src" APP_SOURCES)
add_executable(ByteSurgeon ${APP_SOURCES})

target_link_libraries(ByteSurgeon PRIVATE my_library)

# Тесты
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()