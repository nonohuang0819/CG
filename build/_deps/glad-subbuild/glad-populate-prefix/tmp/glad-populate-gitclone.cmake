
if(NOT "/home/nonohuang/CG/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-gitinfo.txt" IS_NEWER_THAN "/home/nonohuang/CG/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/home/nonohuang/CG/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/nonohuang/CG/build/_deps/glad-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/nonohuang/CG/build/_deps/glad-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout --config "advice.detachedHead=false" "https://github.com/Dav1dde/glad.git" "glad-src"
    WORKING_DIRECTORY "/home/nonohuang/CG/build/_deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/Dav1dde/glad.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout v0.1.36 --
  WORKING_DIRECTORY "/home/nonohuang/CG/build/_deps/glad-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'v0.1.36'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/home/nonohuang/CG/build/_deps/glad-src"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/nonohuang/CG/build/_deps/glad-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/home/nonohuang/CG/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-gitinfo.txt"
    "/home/nonohuang/CG/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/nonohuang/CG/build/_deps/glad-subbuild/glad-populate-prefix/src/glad-populate-stamp/glad-populate-gitclone-lastrun.txt'")
endif()

