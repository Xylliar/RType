file(GLOB_RECURSE CLIENT_SOURCES src/*.cpp src/*.hpp)

add_executable(client ${CLIENT_SOURCES})

target_link_libraries(client PRIVATE asio)

target_link_libraries(client PRIVATE freetype)
target_link_libraries(client PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
target_link_libraries(client PRIVATE utils)

set_target_properties(client PROPERTIES
    OUTPUT_NAME "r-type_client"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/"
)

target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
    add_custom_command(
        TARGET client
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
        ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll 
        $<TARGET_FILE_DIR:client>
        VERBATIM
    )
endif()
