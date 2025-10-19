# ⚙️ Motor de Físicas 2D en C++

Un proyecto personal para aprender los fundamentos de la ingeniería de software y el desarrollo de videojuegos construyendo un motor de físicas 2D desde cero con C++ moderno y SDL3.

## 🚀 Acerca del Proyecto

Este repositorio documenta mi viaje en la construcción de un motor de físicas 2D. El objetivo principal no es crear el próximo gran motor comercial, sino profundizar en conceptos de bajo nivel, tales como:

* Álgebra lineal aplicada a la física de juegos.
* Arquitectura de software para sistemas en tiempo real.
* Gestión de memoria y optimización de rendimiento en C++.
* Implementación de algoritmos de detección y respuesta de colisiones.

Este proyecto sirve como una pieza fundamental de mi portafolio, demostrando mi capacidad para abordar problemas complejos y mi pasión por la tecnología detrás de los videojuegos.

## 🛠️ Tecnologías Utilizadas

* **Lenguaje:** C++17
* **Sistema de Compilación:** CMake
* **Librería Gráfica:** SDL3 (para renderizado y manejo de ventanas/eventos)

***

## 🗺️ Roadmap del Proyecto

El desarrollo del motor está planificado en varias fases incrementales.

-   [x] **Fase 0: Entorno y Ventana Básica**
    -   [x] Configuración de proyecto con CMake.
    -   [x] Integración de la librería SDL3.
    -   [x] Creación de una ventana y un bucle de juego básico.
-   [x] **Fase 1: Fundamentos Matemáticos**
    -   [x] Implementación de una clase `Vector2D` robusta.
    -   [x] Sobrecarga de operadores para operaciones vectoriales intuitivas.
-   [ ] **Fase 2: Arquitectura del Motor**
    -   [ ] Diseño de la clase `Body` para representar objetos físicos.
    -   [ ] Diseño de la clase `World` para gestionar la simulación.
-   [ ] **Fase 3: Movimiento e Integración**
    -   [ ] Implementación de un bucle de física con *timestep* fijo.
    -   [ ] Integrador de Euler para simular el movimiento.
-   [ ] **Fase 4: Detección de Colisiones**
    -   [ ] Algoritmos para formas simples (Círculo-Círculo, AABB-AABB).
-   [ ] **Fase 5: Respuesta a Colisiones**
    -   [ ] Implementación de resolución de colisiones basada en impulsos.
-   [ ] **Fase 6: Optimización y Mejoras**
    -   [ ] (Avanzado) Estructuras de datos espaciales (ej. Quadtree).
    -   [ ] (Avanzado) Implementación de un integrador más preciso (ej. Verlet).

***

## 🎮 Cómo Empezar

Sigue estos pasos para compilar y ejecutar el proyecto en tu máquina local.

### **Prerrequisitos**

Asegúrate de tener las siguientes herramientas instaladas:
* Un compilador de C++ que soporte C++17 (ej. GCC, Clang, MSVC).
* [CMake](https://cmake.org/download/) (versión 3.10 o superior).

La dependencia de **SDL3** está incluida en el repositorio (`/vendor`), por lo que no se requiere instalación manual.

### **Instrucciones de Compilación y Ejecución**

1.  **Clona el repositorio:**
    ```sh
    git clone [https://github.com/TU_USUARIO/TU_REPOSITORIO.git](https://github.com/TU_USUARIO/TU_REPOSITORIO.git)
    cd TU_REPOSITORIO
    ```

2.  **Genera los archivos del proyecto con CMake:**
    ```sh
    cmake -B build
    ```

3.  **Compila el proyecto:**
    ```sh
    cmake --build build
    ```

4.  **Ejecuta la aplicación:**
    El ejecutable se encontrará dentro de la carpeta `build/`.
    > **Nota para Windows:** Puede que necesites copiar el archivo `SDL3.dll` (ubicado en `vendor/SDL3/lib/x64`) a la misma carpeta donde se generó el `.exe` (ej. `build/Debug/`).

    ```sh
    # En Windows (ejemplo)
    ./build/Debug/PhysicsEngine.exe
    
    # En Linux/macOS
    ./build/PhysicsEngine
    ```