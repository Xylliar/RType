file(GLOB_RECURSE ECS_SOURCES src/*.cpp include/*.hpp)

add_library(ecs SHARED ${ECS_SOURCES})

target_include_directories(ecs
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(ecs PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    OUTPUT_NAME "ecs"
)

if(WIN32)
    set_target_properties(ecs PROPERTIES
        SUFFIX ".dll"
        IMPORT_PREFIX ""
        IMPORT_SUFFIX ".lib"
    )
endif()

add_library(ecs_headers INTERFACE)
target_include_directories(ecs_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
