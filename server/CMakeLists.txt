file(GLOB_RECURSE SERVER_SOURCES src/*.cpp src/*.hpp)

add_executable(server ${SERVER_SOURCES})

target_link_libraries(server PRIVATE asio)

target_link_libraries(client PRIVATE freetype)
target_link_libraries(server PRIVATE utils)
target_link_libraries(server PRIVATE sfml-graphics sfml-window sfml-system)

add_dependencies(server ecs)
target_compile_definitions(server PRIVATE ECS_LIBRARY_PATH="$<TARGET_FILE:ecs>")

set_target_properties(server PROPERTIES
    OUTPUT_NAME "r-type_server"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/"
)

target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
    add_custom_command(
        TARGET server
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll 
        $<TARGET_FILE_DIR:server>
        VERBATIM
    )
endif()
