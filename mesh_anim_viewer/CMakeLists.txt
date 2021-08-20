cmake_minimum_required(VERSION 3.15)
project(meshanimviewer)





#encoder.hpp
SET(header encoder.h viewer.h)
SET(src main.cpp viewer.cpp)


message("test ${PROJECT_NAME}")
message("test 2 ${ffmpeg_INCLUDE}")
message("test 2 ${ffmpeg_LIBRARIES}")
add_executable(${PROJECT_NAME} ${src} ${header})
target_include_directories(${PROJECT_NAME} PUBLIC ${ffmpeg_INCLUDE})
target_link_libraries(${PROJECT_NAME} ${ffmpeg_LIBRARIES}  igl::core )








