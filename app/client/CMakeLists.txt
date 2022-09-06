project(client)
build_application()

target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_MODULES} Threads::Threads)