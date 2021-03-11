get_filename_component(MAIN_APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src DIRECTORY)

file(GLOB MAIN_APP_SOURCES
  ${MAIN_APP_DIR}/*.cpp
  ${MAIN_APP_DIR}/*.c
  ${MAIN_APP_DIR}/*.m
  ${MAIN_APP_DIR}/*.hpp
  ${MAIN_APP_DIR}/*.h
)

file(GLOB_RECURSE LIB_FILES "../include/stb/*.h")

set(MAIN_APP_LIBRARIES
  imgui
  glfw
  glad
  glm
)

set(NAME Engine)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/${NAME})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/${NAME})

add_executable(${NAME} ${MAIN_APP_SOURCES} ${LIB_FILES} ../.editorconfig)
install(TARGETS ${NAME} DESTINATION bin)
target_link_libraries(${NAME} ${MAIN_APP_LIBRARIES})
