//
// Created by tux on 11.03.2024.
//

#ifndef RAYLIB_TEMPLATE_MATERIAL_H
#define RAYLIB_TEMPLATE_MATERIAL_H


#include "Vec3.h"

namespace RN {

    class Material {
    public:
        Material(vec3 e, vec3 a): emission(e), absorption(a){};
        Material(): emission(vec3 ()), absorption(vec3()) {};
        vec3 emission;
        vec3 absorption;

        float reflectivity = 0.0; //0 - no reflections
        float eta = 1.0; // 1.0 - air
    };
}


#endif //RAYLIB_TEMPLATE_MATERIAL_H
