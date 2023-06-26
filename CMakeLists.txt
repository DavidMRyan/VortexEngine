cmake_minimum_required(VERSION 3.25)
project(VortexEngine C)

set(CMAKE_C_STANDARD 23)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(VortexEngine
        main.c
        src/include/util.h
        src/common/util.c
        src/include/math_3d.h
        src/common/math_3d.c
        src/include/world_transform.h
        src/common/world_transform.c)

# Linker Stuff
target_link_libraries(VortexEngine ${OPENGL_LIBRARIES})
target_link_libraries(VortexEngine ${GLUT_LIBRARIES})
target_link_libraries(VortexEngine -lglew32s -lglu32 -lopengl32 -lfreeglut)