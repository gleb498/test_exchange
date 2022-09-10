project(tests)
build_application()

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/3rd/catch2/single_include/catch2")

target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_MODULES} ${COMMON_MODULES} Threads::Threads)