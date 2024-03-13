//
// Created by root on 11.03.2024.
//

#ifndef RAYLIB_TEMPLATE_MATERIAL_H
#define RAYLIB_TEMPLATE_MATERIAL_H


#include <glm/vec3.hpp>

namespace RN {

    class Material {
    public:
        Material(glm::vec3 e, glm::vec3 a): emission(e), absorption(a){};
        Material(): emission(glm::vec3 (0,0,0)), absorption(0,0,0) {};
        glm::vec3 emission;
        glm::vec3 absorption;

        float reflectivity = 0.0; //0 - no reflections
        float eta = 1.0; // 1.0 - air
    };
}


#endif //RAYLIB_TEMPLATE_MATERIAL_H
