
######################################################################################################################################
# FUNCTION DEFINITION
######################################################################################################################################
function(download_file url filename)# hash_type hash)
if(NOT EXISTS ${filename})
  file(DOWNLOAD ${url} ${filename}
      #TIMEOUT 5  # seconds
      SHOW_PROGRESS
      #EXPECTED_HASH ${hash_type}=${hash})
      )
endif()
endfunction()


function(unzip in_file output_file)
  get_filename_component(dir ${in_file} DIRECTORY)
  get_filename_component(f_name ${in_file} NAME_WLE)
  get_filename_component(dir_out ${output_file} DIRECTORY)
  get_filename_component(f_name_out ${output_file} NAME_WLE)
  if(NOT EXISTS ${dir_out}/${f_name_out})

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xvf ${in_file} 
    WORKING_DIRECTORY ${dir_out}
    )
  file(REMOVE ${in_file} ) #remove file
  endif()
 
  
  file(GLOB var "${dir_out}/${f_name_out}?*") # get file list
  foreach(item ${var})
      if(IS_DIRECTORY ${item})
        get_filename_component(old_name ${item} NAME)
        get_filename_component(dir ${item} DIRECTORY)
        file(RENAME ${dir}/${old_name} ${dir}/${f_name_out})
        break()
      endif()
    endforeach()
endfunction()
  
######################################################################################################################################



# DOWNLOAD
set(ffmpeg_github https://github.com/BtbN/FFmpeg-Builds/releases/download/autobuild-2021-08-18-12-37/ffmpeg-n4.4-80-gbf87bdd3f6-win64-lgpl-shared-4.4.zip)
download_file(
  ${ffmpeg_github}
  ${CMAKE_SOURCE_DIR}/external/ffmpeg.zip
  )
  

set(ffmpeg_result ${CMAKE_SOURCE_DIR}/external/ffmpeg)
# UNZIP
unzip(${CMAKE_SOURCE_DIR}/external/ffmpeg.zip ${CMAKE_SOURCE_DIR}/external/ffmpeg)
message("get ${CMAKE_SOURCE_DIR}")
  

set(ffmpeg_ROOT)
set(ffmpeg_INCLUDE)
set(ffmpeg_LIBRARIES )
set(ffmpeg_BINARY )

# add CMakeLists.txt to ffmpeg directory
configure_file(${CMAKE_CURRENT_LIST_DIR}/ffmpeg_CmakeLists.txt.in ${ffmpeg_result}/CMakeLists.txt @ONLY)
add_subdirectory(${ffmpeg_result})
