project(graphics_asgn2)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

# I added this
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include(FindPkgConfig)

pkg_check_modules(GLFW3 REQUIRED glfw3)
include_directories(${GLFW3_INCLUDE_DIRS})
target_link_libraries (${PROJECT_NAME} ${GLFW3_LIBRARIES})

pkg_check_modules(GLEW REQUIRED glew)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries (${PROJECT_NAME} ${GLEW_LIBRARIES})

# sdl2-config --cflags --libs

pkg_check_modules(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries (${PROJECT_NAME} ${SDL2_LIBRARIES})


PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

if (NOT APPLE)
  pkg_check_modules(GL REQUIRED gl)
  include_directories(${GL_INCLUDE_DIRS})
  target_link_libraries (${PROJECT_NAME} ${GL_LIBRARIES})
endif()

if (APPLE)
  target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

include_directories("${CMAKE_SOURCE_DIR}/src/common")

MESSAGE("${CMAKE_SOURCE_DIR}/src/common")

# pkg_check_modules(AO REQUIRED ao)
# include_directories(${AO_INCLUDE_DIRS})
# target_link_libraries (${PROJECT_NAME} ${AO_LIBRARIES})

# pkg_check_modules(LIBMPG123 REQUIRED libmpg123)
# include_directories(${LIBMPG123_INCLUDE_DIRS})
# target_link_libraries (${PROJECT_NAME} ${LIBMPG123_LIBRARIES})

configure_file(src/Sample_GL.vert Sample_GL.vert COPYONLY)
configure_file(src/Sample_GL.frag Sample_GL.frag COPYONLY)

configure_file(src/TextVertexShader.vertexshader TextVertexShader.vertexshader COPYONLY)
configure_file(src/TextVertexShader.fragmentshader TextVertexShader.fragmentshader COPYONLY)

# configure_file(assets/song.mp3 assets/song.mp3 COPYONLY)
