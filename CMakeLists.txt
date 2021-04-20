add_executable(p4 "main.cpp")
target_link_libraries(p4
LINK_PUBLIC
glfw
glad
glm
${CMAKE_DL_LIBS}
)