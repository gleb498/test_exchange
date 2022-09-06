project(config)
build_library()
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options)