cmake_minimum_required(VERSION 3.10)
project(ARProject)

# Encontrar paquetes necesarios
find_package(OpenCV REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

# Incluir directorios
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLFW3_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

# Añadir el archivo ejecutable
add_executable(ARProject main.cpp)

# Vincular bibliotecas
target_link_libraries(ARProject ${OpenCV_LIBS} ${GLEW_LIBRARIES} glfw OpenGL::GL)


