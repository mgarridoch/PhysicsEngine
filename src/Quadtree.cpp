#include "Quadtree.h"

// Constructor que inicializa el nivel y los límites del nodo
Quadtree::Quadtree(int level, SDL_FRect bounds)
    : m_level(level), 
      m_bounds(bounds),
      m_northWest(nullptr), 
      m_northEast(nullptr), 
      m_southWest(nullptr), 
      m_southEast(nullptr) 
{
    // El cuerpo puede estar vacío, la lista de inicialización ya hizo todo el trabajo.
}

// Limpia el Quadtree, eliminando todos los objetos y sub-nodos.
void Quadtree::Clear() {
    m_objects.clear();

    if (m_northWest) {
        m_northWest->Clear();
        m_northEast->Clear();
        m_southWest->Clear();
        m_southEast->Clear();

        m_northWest.reset();
        m_northEast.reset();
        m_southWest.reset();
        m_southEast.reset();
    }
}

// Funcion para dividir el nodo actual en cuatro sub-nodos.
void Quadtree::Subdivide() {
    // Calculamos las dimensiones de los hijos
    float subWidth = m_bounds.w / 2.0f;
    float subHeight = m_bounds.h / 2.0f;
    float x = m_bounds.x;
    float y = m_bounds.y;

    // Creamos y asignamos los cuatro hijos usando std::make_unique
    m_northWest = std::make_unique<Quadtree>(m_level + 1, SDL_FRect{x, y, subWidth, subHeight});
    m_northEast = std::make_unique<Quadtree>(m_level + 1, SDL_FRect{x + subWidth, y, subWidth, subHeight});
    m_southWest = std::make_unique<Quadtree>(m_level + 1, SDL_FRect{x, y + subHeight, subWidth, subHeight});
    m_southEast = std::make_unique<Quadtree>(m_level + 1, SDL_FRect{x + subWidth, y + subHeight, subWidth, subHeight});
}

// Funcion que determina en qué cuadrante hijo cabe un cuerpo.
// Devuelve -1 si no cabe completamente en ninguno y debe quedarse en el nodo padre.
int Quadtree::GetIndex(Body* body) {
    int index = -1;
    float verticalMidpoint = m_bounds.x + (m_bounds.w / 2.0f);
    float horizontalMidpoint = m_bounds.y + (m_bounds.h / 2.0f);

    // Comprueba si el objeto cabe completamente en el cuadrante superior
    bool topQuadrant = (body->position.y + body->height / 2.0f < horizontalMidpoint);
    // Comprueba si el objeto cabe completamente en el cuadrante inferior
    bool bottomQuadrant = (body->position.y - body->height / 2.0f > horizontalMidpoint);

    // Comprueba si el objeto cabe completamente en el cuadrante izquierdo
    if (body->position.x + body->width / 2.0f < verticalMidpoint) {
        if (topQuadrant) {
            index = 0; // Cuadrante Noroeste
        } else if (bottomQuadrant) {
            index = 2; // Cuadrante Suroeste
        }
    }
    // Comprueba si el objeto cabe completamente en el cuadrante derecho
    else if (body->position.x - body->width / 2.0f > verticalMidpoint) {
        if (topQuadrant) {
            index = 1; // Cuadrante Noreste
        } else if (bottomQuadrant) {
            index = 3; // Cuadrante Sureste
        }
    }
    return index;
}

// --- FUNCIÓN INSERT PRINCIPAL ---
void Quadtree::Insert(Body* body) {
    // Si este nodo ya tiene hijos, intentamos insertar el cuerpo en uno de ellos.
    if (m_northWest) {
        int index = GetIndex(body);

        if (index != -1) {
            // El cuerpo cabe perfectamente en un hijo, se lo pasamos y terminamos.
            switch (index) {
                case 0: m_northWest->Insert(body); break;
                case 1: m_northEast->Insert(body); break;
                case 2: m_southWest->Insert(body); break;
                case 3: m_southEast->Insert(body); break;
            }
            return;
        }
    }

    // Si no tenemos hijos o el cuerpo no cabe en ninguno, se queda en este nodo.
    m_objects.push_back(body);

    // Si superamos la capacidad y no hemos alcanzado el límite de niveles...
    if (m_objects.size() > MAX_OBJECTS && m_level < MAX_LEVELS) {
        // ...y si aún no nos hemos subdividido...
        if (!m_northWest) {
            Subdivide();
        }

        // ...movemos todos nuestros objetos a los hijos correspondientes.
        int i = 0;
        while (i < m_objects.size()) {
            int index = GetIndex(m_objects[i]);
            if (index != -1) {
                // Obtenemos el objeto, lo insertamos en el hijo y lo eliminamos de aquí.
                Body* movingBody = m_objects[i];
                switch (index) {
                    case 0: m_northWest->Insert(movingBody); break;
                    case 1: m_northEast->Insert(movingBody); break;
                    case 2: m_southWest->Insert(movingBody); break;
                    case 3: m_southEast->Insert(movingBody); break;
                }
                m_objects.erase(m_objects.begin() + i);
            } else {
                // Si el objeto no se movió, avanzamos al siguiente.
                i++;
            }
        }
    }
}

void Quadtree::Query(const SDL_FRect& range, std::vector<Body*>& foundObjects) const {
    // 1. Si el rango de búsqueda no se cruza con los límites de este nodo, paramos.
    if (!SDL_HasRectIntersectionFloat(&m_bounds, &range)) {
        return;
    }

    // 2. Añadimos los objetos de este nodo a la lista de resultados.
    for (Body* body : m_objects) {
        // Podríamos añadir una comprobación extra aquí para ver si el cuerpo en sí
        // intersecta el rango, para mayor precisión.
        foundObjects.push_back(body);
    }

    // 3. Si tenemos hijos, continuamos la búsqueda recursivamente.
    if (m_northWest) {
        m_northWest->Query(range, foundObjects);
        m_northEast->Query(range, foundObjects);
        m_southWest->Query(range, foundObjects);
        m_southEast->Query(range, foundObjects);
    }
}