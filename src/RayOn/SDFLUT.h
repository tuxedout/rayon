/*
 *
 *       Created by tux on 16.03.2024.
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
 *      SDF lookup table, contains screen-wide SDF data
 *
 */

#ifndef RAYON_SDFLUT_H
#define RAYON_SDFLUT_H

#include <vector>
#include "Vec2.h"
#include "SFScene.h"

namespace RN {

    static int permutation[] = {
            151, 160, 137, 91, 90, 15,
            131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
            8, 99, 37, 240, 21, 10, 23, 190,  6, 148, 247, 120, 234, 75, 0, 26,
            197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149,
            56, 87, 174, 20, 125, 136, 171, 168,  68, 175, 74, 165, 71, 134, 139, 48,
            27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105,
            92, 41, 55, 46, 245, 40, 244, 102, 143, 54,  65, 25, 63, 161, 1, 216,
            80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188,
            159, 86, 164, 100, 109, 198, 173, 186,  3, 64, 52, 217, 226, 250, 124, 123,
            5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16,
            58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152,  2, 44, 154,
            163,  70, 221, 153, 101, 155, 167,  43, 172, 9, 129, 22, 39, 253,  19, 98,
            108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34,
            242, 193, 238, 210, 144, 12, 191, 179, 162, 241,  81, 51, 145, 235, 249, 14,
            239, 107, 49, 192, 214,  31, 181, 199, 106, 157, 184,  84, 204, 176, 115,
            121, 50, 45, 127,  4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24,
            72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
    };


    class SDFLUT {
    public:
        SDFLUT(const SFScene &scene): scene(scene) {
            setSize(scene.pxSize());
            generate_SDF();
            generate_noise();
        }

        void setNoiseValue(const vec2d &val, vec2i p){
            perlin_table[p.y * width + p.x] = val;
        }

        // get distance & pointer to nearest object
        [[nodiscard]] const vec2<double> getNoiseValue(vec2d p) const {
            vec2i px = scene.pxCoords(p);

            return perlin_table[px.y * width + px.x];
        }

        void setSDFValue(const SFPoinInfo &val, vec2i p){
            sdf_table[p.y * width + p.x] = val;
        }

        // get distance & pointer to nearest object
        [[nodiscard]] const SFPoinInfo* getSDFValue(vec2d p) const {
            vec2i px = scene.pxCoords(p);

            return &sdf_table[px.y * width + px.x];
        }


        // get only distance
        [[nodiscard]] double getDistanceValue(vec2d p) const {
            vec2i px = scene.pxCoords(p);

            return sdf_table[px.y * width + px.x].distance;
        }

        // calculate and store SDF data
        void generate_SDF() {
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    vec2d scene_coords = scene.sceneCoords({x, y});
                    SFPoinInfo hitInfo = scene.hit(scene_coords);
                    setSDFValue(hitInfo, {x, y});
                }
            }
        }

        void generate_noise() {
            initPermutation();

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    vec2d scene_coords = scene.sceneCoords({x, y})*591;
                    double noise_x = (perlin(scene_coords.x, scene_coords.y));
                    double noise_y = (perlin(scene_coords.x, scene_coords.y + 3133));
                    setNoiseValue({noise_x, noise_y}, {x, y});
                }
            }
        }

        void setSize(vec2i s){
            width = s.x;
            height = s.y;
            sdf_table.resize(width * height);
            perlin_table.resize(width * height);
        }

        // normal is calculated from SDF gradient
        [[nodiscard]] vec2d normal(const vec2d &p) const {
            const double delta = 0.0001; // Малое смещение для вычисления градиента
            const double twoDelta = delta * 2.0;

            // Вычисляем компоненты градиента по X и Y, используя центральную разность
            double gradX = (getSDFValue(vec2d{p.x + delta, p.y})->distance - getSDFValue(vec2d{p.x - delta, p.y})->distance) / twoDelta;
            double gradY = (getSDFValue(vec2d{p.x, p.y + delta})->distance - getSDFValue(vec2d{p.x, p.y - delta})->distance) / twoDelta;

            vec2d grad = {gradX, gradY};
            grad.normalize();
            return grad;
        }

        int double_permutations[512];

        void initPermutation() {
            for(int i = 0; i < 256 ; i++) double_permutations[256+i] = double_permutations[i] = permutation[i];
        }

        static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
        static double lerp(double t, double a, double b) { return a + t * (b - a); }
        static double grad(int hash, double x, double y) {
            int h = hash & 7;
            double u = h<4 ? x : y;
            double v = h<4 ? y : x;
            return ((h&1) ? -u : u) + ((h&2) ? -2.0*v : 2.0*v);
        }

        double perlin(double x, double y) {
            int X = (int)floor(x) & 255,
                    Y = (int)floor(y) & 255;
            x -= floor(x);
            y -= floor(y);
            double u = fade(x),
                    v = fade(y);
            int A = double_permutations[X]+Y, AA = double_permutations[A], AB = double_permutations[A+1],
                    B = double_permutations[X+1]+Y, BA = double_permutations[B], BB = double_permutations[B+1];

            return lerp(v, lerp(u, grad(double_permutations[AA], x, y),  grad(double_permutations[BA], x-1, y)),
                        lerp(u, grad(double_permutations[AB], x, y-1), grad(double_permutations[BB], x-1, y-1)));
        }
    private:
        const SFScene &scene;

        // SDF data
        std::vector<SFPoinInfo> sdf_table;
        std::vector<vec2d>  perlin_table;

        int width = 0;
        int height = 0;
    };

} // namespace RN

#endif //RAYON_SDFLUT_H