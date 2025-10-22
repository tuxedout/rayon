/*
 *
 *       Created by tux on 09.06.2024.
 *       ________   _______  ____ ____  _______  ____ ____
 *      │----R---\ /---A---\ ----Y---- /---O---\│----N----\
 *      │         │         │    │    │         │         │
 *      │    ^    │    ^    │    │    │    ^    │    ^    │
 *      │    │    │    │    │    │    │    │    │    │    │
 *      │    │    │    │    │    │    │    │    │    │    │
 *      │    ┼    │    ┼    │    ┼    │    ┼    │    │    │
 *      │        (          \         │    │    │    │    │
 *      │    ^    │    ^    │)        │    │    │    │    │
 *      │    │    │    │    /         │    v    │    │    │
 *      │    │    │    │    │        /│         │    │    │
 *      │────│────│────│────│───────/  \_______/│____│____│
 *
 *      RayOn - simple rig to play with rays
 *
 *      MTLLoader.h
 *
 */

#ifndef RAYON_CPP_CMAKE_MTLLOADER_H
#define RAYON_CPP_CMAKE_MTLLOADER_H

#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <iostream>
#include "Material.h"

namespace RN {
    namespace Materials {
        /*
         * MTL illumination model:
         *
         * Illumination Model 0: Color on and Ambient off
         * ColorMaterial
         * Illumination Model 1: Color on and Ambient on
         * SolidcolorMaterial
         * Illumination Model 2: Highlight on
         * ReflectingMaterial
         * Illumination Model 3: Reflection on and Ray trace on
         * ReflectingMaterial
         * Illumination Model 4: Transparency: Glass on, Reflection: Ray trace on
         * RefractingMaterial
         * Illumination Model 5: Reflection: Fresnel on and Ray trace on
         * ReflectingMaterial
         * Illumination Model 6: Transparency: Refraction on, Reflection: Fresnel off and Ray trace on
         * RefractingMaterial
         * Illumination Model 7: Transparency: Refraction on, Reflection: Fresnel on and Ray trace on
         * RefractingMaterial
         * Illumination Model 8: Reflection on and Ray trace off
         * ReflectingMaterial
         * Illumination Model 9: Transparency: Glass on, Reflection: Ray trace off
         * RefractingMaterial
         * Illumination Model 10: Casts shadows onto invisible surfaces
         * n/a
         */

        struct MTLEntryInfo {
            vec3d ambient;
            vec3d diffuse;
            vec3d specular;
            double shininess;
            double transparency;
            int illuminationModel;

            void reset(){
                ambient = vec3d::ZERO;
                diffuse = vec3d::ZERO;
                specular = vec3d::ZERO;

                shininess = 0;
                transparency = 0;

            }
        };

        class MTLLoader {
        public:
            MTLLoader() = default;

            bool loadMTL(const std::string& filename) {
                std::ifstream file(filename);
                if (!file.is_open()) {
                    std::cerr << "Could not open MTL file: " << filename << std::endl;
                    return false;
                }

                std::string line;
                MTLEntryInfo currentMaterial;
                while (std::getline(file, line)) {
                    std::istringstream iss(line);
                    std::string prefix;
                    iss >> prefix;

                    if (prefix == "newmtl") {
                        std::string materialName;
                        iss >> materialName;
                        currentMaterial.reset();
                        //materials[materialName] = currentMaterial;
                    } else if (prefix == "Ka") {
                        iss >> currentMaterial.ambient.x >> currentMaterial.ambient.y >> currentMaterial.ambient.z;
                    } else if (prefix == "Kd") {
                        iss >> currentMaterial.diffuse.x >> currentMaterial.diffuse.y >> currentMaterial.diffuse.z;
                    } else if (prefix == "Ks") {
                        iss >> currentMaterial.specular.x >> currentMaterial.specular.y >> currentMaterial.specular.z;
                    } else if (prefix == "Ns") {
                        iss >> currentMaterial.shininess;
                    } else if (prefix == "d") {
                        iss >> currentMaterial.transparency;
                    } else if (prefix == "illum") {
                        iss >> currentMaterial.illuminationModel;
                    }
                    // You can add more attributes here as needed
                }

                return true;
            }

            std::shared_ptr<Material> createMaterial(const std::string &name, const MTLEntryInfo &info) {

            };

            std::shared_ptr<Material> getMaterial(const std::string& name) const {
                auto it = materials.find(name);
                if (it != materials.end()) {
                    return it->second;
                }
                return nullptr;
            }

        private:
            std::unordered_map<std::string, std::shared_ptr<Material>> materials;
        };

    } // Materials
} // RN

#endif //RAYON_CPP_CMAKE_MTLLOADER_H
