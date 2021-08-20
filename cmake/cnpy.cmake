if(TARGET cnpy)
    return()
endif()

include(FetchContent)
FetchContent_Declare(
    cnpy
    GIT_REPOSITORY https://github.com/RohJiHyun/cnpy.git
    GIT_TAG 9f85124cd9dd8cdf8d1762878216e535928493b2
)


FetchContent_GetProperties(cnpy)
if (NOT cnpy_POPULATED)
    FetchContent_Populate(cnpy)
    add_subdirectory(${cnpy_SOURCE_DIR} ${cnpy_BINARY_DIR})
endif ()
