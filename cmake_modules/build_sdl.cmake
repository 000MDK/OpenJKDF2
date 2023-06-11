set(SDL_ROOT ${CMAKE_BINARY_DIR}/SDL)

ExternalProject_Add(
    SDL
    SOURCE_DIR          ${CMAKE_SOURCE_DIR}/lib/SDL
    BINARY_DIR          ${SDL_ROOT}
    INSTALL_DIR         ${SDL_ROOT}
    UPDATE_DISCONNECTED TRUE
    CMAKE_ARGS          --toolchain ${CMAKE_TOOLCHAIN_FILE}
                        --install-prefix ${SDL_ROOT}
                        -DCMAKE_BUILD_TYPE:STRING=Release
                        -DSDL_SHARED:BOOL=FALSE
                        -DSDL_STATIC:BOOL=TRUE
                        -DSDL_TEST:BOOL=FALSE
)
# *Replicate* variables generated by `FindSDL`
set(SDL_FOUND TRUE)
set(SDL_INCLUDE_DIRS ${SDL_ROOT}/include/SDL2)
if(WIN32)
    set(SDL_LIBRARIES ${SDL_ROOT}/lib/${CMAKE_IMPORT_LIBRARY_PREFIX}SDL2${CMAKE_IMPORT_LIBRARY_SUFFIX})
else()
    set(SDL_LIBRARIES ${SDL_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()
set(SDL_VERSION 2.26.5)
string(REGEX MATCH "^[0-9]+" SDL_VERSION_MAJOR ${SDL_VERSION})
string(REGEX REPLACE "^[0-9]+\\.([0-9]+).*" "\\1" SDL_VERSION_MINOR ${SDL_VERSION})
string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" SDL_VERSION_PATCH ${SDL_VERSION})
message(SDL_VERSION_MAJOR\ ==\ ${SDL_VERSION_MAJOR})
message(SDL_VERSION_MINOR\ ==\ ${SDL_VERSION_MINOR})
message(SDL_VERSION_PATCH\ ==\ ${SDL_VERSION_PATCH})
# *Replicate* SDL target
if(NOT TARGET SDL::SDL)
    add_library(SDL::SDL STATIC IMPORTED)
endif()
add_dependencies(SDL::SDL SDL)
file(MAKE_DIRECTORY ${SDL_INCLUDE_DIRS})
set_target_properties(
    SDL::SDL PROPERTIES
    IMPORTED_LOCATION ${SDL_LIBRARIES}
    IMPORTED_IMPLIB ${SDL_LIBRARIES}
)
target_include_directories(SDL::SDL INTERFACE ${SDL_INCLUDE_DIRS})
target_link_directories(SDL::SDL INTERFACE ${SDL_ROOT}/lib)

# TODO why doesn't this work?
#if(TARGET_MACOS)
#    set(SDL_MACOS_FRAMEWORKS "-framework CoreAudio -framework Cocoa")
#    target_link_libraries(SDL::SDL INTERFACE ${SDL_MACOS_FRAMEWORKS})
#endif()
