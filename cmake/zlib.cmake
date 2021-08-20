if(TARGET zlib)
    return()
endif()

include(FetchContent)
FetchContent_Declare(
    zlib
    GIT_REPOSITORY https://github.com/madler/zlib.git
    GIT_TAG v1.2.11
)
FetchContent_GetProperties(zlib)
if(NOT  zlib_POPULATED)
    FetchContent_Populate(zlib)
message("${zlib_SOURCE_DIR}")
message("${zlib_BINARY_DIR}")
#add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
endif()
