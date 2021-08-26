cmake_minimum_required(VERSION 3.15)
project(meshanimviewer)





#encoder.hpp
set(${OPENMP_LIB} "")
if(OPENMP)
find_package(OpenMP REQUIRED)
ADD_DEFINITIONS(-DOPENMP)
set(${OPENMP_LIB} OpenMP::OpenMP_CXX)
endif()

set(GLAB_THREAD_NUM $ENV{NUMBER_OF_PROCESSORS})

configure_file(thread_configure.h.in ${CMAKE_CURRENT_LIST_DIR}/thread_configure.h @ONLY)

SET(header encoder.h viewer.h filemanager.h alphanum.h thread_configure.h)

SET(src main.cpp viewer.cpp filemanager.cpp )



message("test ${PROJECT_NAME}")
message("test 2 ${ffmpeg_INCLUDE}")
message("test 2 ${ffmpeg_LIBRARIES}")
add_executable(${PROJECT_NAME} ${src} ${header} )
target_include_directories(${PROJECT_NAME} PUBLIC ${ffmpeg_INCLUDE})
target_link_libraries(${PROJECT_NAME}  igl::core igl::embree igl::png  ${ffmpeg_LIBRARIES} ${OPENMP_LIB})








