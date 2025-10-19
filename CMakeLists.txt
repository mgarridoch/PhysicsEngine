#version minima de cmake requerida. Buena practica.
cmake_minimum_required(VERSION 3.10)

#Nombre del proyecto
project(PhysicsEngine)

#Especificar la version de C++ a utilizar
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Le decimos a Cmake donde estan las cabeceras .h de SDL3 las cuales se encuentran en la carpeta "include" dentro de este proyecto / vendor / SDL3 / include
include_directories(vendor/SDL3/include)

# Le decimos a CMake donde encontrar los archivos .lib de SDL3
link_directories(vendor/SDL3/lib/x64)

# Le decumos al enlazador (linker) que bibliotecas utilizar
set(SDL3_LIBS SDL3)

# Agregamos el ejecutable

add_executable(PhysicsEngine src/main.cpp)

# Linkeamos las bibliotecas de SDL3
target_link_libraries(PhysicsEngine ${SDL3_LIBS})

