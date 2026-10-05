file(GLOB_RECURSE CLIENT_SOURCES src/*.cpp src/*.hpp)

add_executable(client_aim-box ${CLIENT_SOURCES})

target_link_libraries(client_aim-box PRIVATE asio)

target_link_libraries(client_aim-box PRIVATE freetype)
target_link_libraries(client_aim-box PRIVATE sfml-graphics sfml-window sfml-system)
target_link_libraries(client_aim-box PRIVATE utils)

set_target_properties(client_aim-box PROPERTIES
    OUTPUT_NAME "aim-box_client"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/"
)

target_include_directories(client_aim-box PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
    add_custom_command(
        TARGET client_aim-box
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
        ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll 
        $<TARGET_FILE_DIR:client>
        VERBATIM
    )
endif()
