if(TARGET yaml)
    return()
endif()

include(FetchContent)
FetchContent_Declare(
    yaml
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG yaml-cpp-0.7.0
)


FetchContent_GetProperties(yaml)
if (NOT yaml_POPULATED)
    FetchContent_Populate(yaml)
    add_subdirectory(${yaml_SOURCE_DIR} ${yaml_BINARY_DIR})
endif ()
