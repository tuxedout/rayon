/*
 *
 *       Created by tux on 20.04.2024.
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
 *      RTMesh.h
 *
 */

#ifndef RAYON_CPP_CMAKE_RTMESH_H
#define RAYON_CPP_CMAKE_RTMESH_H

#include <vector>
#include "../Primitives/RTTriangle.h"
#include "../BVH.h"
#include <fstream>
#include <sstream>
#include <string>

namespace RN {

    class RTMesh : public BVH {
    public:
        explicit RTMesh(const std::vector<std::shared_ptr<RTItem>> &triangles): BVH(triangles, 0, triangles.size()){

        }

        static RTMesh loadFromOBJ(const std::string& filename, const std::shared_ptr<RN::Materials::Material> &mat) {

            auto start = std::chrono::high_resolution_clock::now();  // start timer
            auto end = std::chrono::high_resolution_clock::now();  // end timer

            long long int total_duration;

            std::ifstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file: " + filename);
            }

            std::vector<vec3d> vertices;
            std::vector<vec3d> normals;
            std::vector<std::shared_ptr<RTItem>> triangles;
            std::string line;

            while (getline(file, line)) {
                std::istringstream iss(line);
                std::string type;
                iss >> type;
                if (type == "v") {
                    double x, y, z;
                    iss >> x >> y >> z;
                    vertices.push_back(vec3d(x, y, z));
                } else if (type == "vn") {
                    double nx, ny, nz;
                    iss >> nx >> ny >> nz;
                    normals.push_back(vec3d(nx, ny, nz));
                } else if (type == "f") {
                    std::vector<int> v_indices;
                    std::vector<int> n_indices;
                    std::string token;
                    while (iss >> token) {
                        auto slash_pos1 = token.find('/');
                        auto slash_pos2 = token.rfind('/');
                        int v_idx = std::stoi(token.substr(0, slash_pos1)) - 1;
                        int n_idx = std::stoi(token.substr(slash_pos2 + 1)) - 1;
                        v_indices.push_back(v_idx);
                        n_indices.push_back(n_idx);
                    }
                    // Вычисление средней нормали
//                    vec3d avg_normal = (normals[n_indices[0]] + normals[n_indices[1]] + normals[n_indices[2]]) / 3;
//                    avg_normal.normalize(); // Нормализация средней нормали

                    auto triangle = std::make_shared<RTTriangle>(vertices[v_indices[0]], vertices[v_indices[1]], vertices[v_indices[2]], normals[n_indices[0]], normals[n_indices[1]], normals[n_indices[2]], mat);
                    triangles.push_back(triangle);
                }
            }

            auto mesh = RTMesh(triangles);

            end = std::chrono::high_resolution_clock::now();  // end timer

            std::cout << "file loaded: " << filename << " triangles: " << triangles.size() << "\n";

            total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            printf("Time taken: %lld milliseconds\n", total_duration);

            return mesh;
        }
    };

} // namespace RN

#endif // RAYON_CPP_CMAKE_RTMESH_H
