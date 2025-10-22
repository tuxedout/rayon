/*
 *
 *       Created by tux on 13.04.2024.
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
 *      Grid.h
 *
 */

#ifndef RAYON_CPP_CMAKE_GRID_H
#define RAYON_CPP_CMAKE_GRID_H

#include <functional>
#include <iostream>
#include <tuple>
#include "../RayOn/Vec2.h"
#include "../RayOn/Vec3.h"

namespace RN {

    template<typename T>
    class GridBase {
    protected:
        T spacing; // Расстояние между элементами
        T start;
    public:
        GridBase(T spacing = static_cast<T>(1), T start = static_cast<T>(0)) : spacing(spacing) {}
        virtual ~GridBase() = default;
    };

    template<typename T>
    class Grid1D : public GridBase<T> {
    private:
        int length;
    public:
        Grid1D(int length, T spacing, T start = 0)
                : GridBase<T>(spacing, start), length(length) {}

        void generateGrid(std::function<void(int, T, T)> func) {
            for (int i = 0; i < length; ++i) {
                T x = i * this->spacing + this->start;
                T rx = static_cast<T>(i) / (length - 1);
                func(i, x, rx);
            }
        }

        T getCoordinates(int i) const {
            return i * this->spacing + this->start;
        }

        T getRelativeCoordinates(int i) const {
            return static_cast<T>(i) / (length - 1);
        }

        T getCenterOffset() const {
            return this->spacing * (length - 1) / 2;
        }
    };

    template<typename T>
    class Grid2D : public GridBase<T> {
    private:
        int width, height;
    public:
        vec2<T> start;

        Grid2D(int width, int height, T spacing, vec2<T> start={0, 0}): GridBase<T>(spacing), width(width), height(height), start(start) {}

        void generateGrid(std::function<void(int, int, vec2<T>, vec2<T>)> func) {
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    vec2<T> coords(j * this->spacing + start.x, i * this->spacing + start.y);
                    vec2<T> relCoords(static_cast<T>(j) / (width - 1), 1.0 - static_cast<T>(i) / (height - 1));
                    func(i, j, coords, relCoords);
                }
            }
        }

        vec2<T> getCoordinates(int i, int j) const {
            return {j * this->spacing + start.x, i * this->spacing + start.y};
        }

        vec2<T> getRelativeCoordinates(int i, int j) const {
            return {static_cast<T>(j) / (width - 1), static_cast<T>(i) / (height - 1)};
        }

        vec2<T> getCenterOffset() const {
            return {this->spacing * (width - 1) / 2, this->spacing * (height - 1) / 2};
        }
    };

    template<typename T>
    class Grid3D : public GridBase<T> {
    private:
        int width, height, depth;
    public:

        vec3<T> start;

        Grid3D(int width, int height, int depth, T spacing, vec3<T> start = {0, 0, 0})
                : GridBase<T>(spacing), width(width), height(height), depth(depth), start(start) {}

        void generateGrid(std::function<void(int, int, int, vec3<T>, vec3<T>)> func) {
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    for (int k = 0; k < depth; ++k) {
                        // TODO: add relative and use vec3d
                        T x = j * this->spacing + start.x;
                        T y = i * this->spacing + start.y;
                        T z = k * this->spacing + start.z;



                        func(i, j, k, {x, y, z}, {static_cast<T>(j) / (width - 1), static_cast<T>(i) / (height - 1), static_cast<T>(k) / (depth - 1)});
                    }
                }
            }
        }

        vec3<T> getCoordinates(int i, int j, int k) const {
            return {j * this->spacing + start.x, i * this->spacing + start.y, k * this->spacing + start.z};
        }

        vec3<T> getRelativeCoordinates(int i, int j, int k) const {
            return {static_cast<T>(j) / (width - 1), static_cast<T>(i) / (height - 1), static_cast<T>(k) / (depth - 1)};
        }

        vec3<T> getCenterOffset() const {
            return {this->spacing * (width - 1) / 2, this->spacing * (height - 1) / 2, this->spacing * (depth - 1) / 2};
        }
    };

    template<typename T>
    class GridCylinder : public GridBase<T> {
    private:
        int heightSegments, radialSegments;
        T radius, height;

    public:
        GridCylinder(int radialSegments, int heightSegments, T radius, T height, T spacing)
                : GridBase<T>(spacing), radialSegments(radialSegments), heightSegments(heightSegments), radius(radius), height(height) {}

        void generateGrid(std::function<void(vec3<T>, vec3<T>)> func) {
            for (int i = 0; i <= heightSegments; ++i) {
                T y = i * (height / heightSegments);
                for (int j = 0; j < radialSegments; ++j) {
                    T theta = j * (2 * RN::PI_D / radialSegments);
                    T x = radius * cos(theta);
                    T z = radius * sin(theta);
                    func({x, y, z}, {static_cast<T>(j) / radialSegments, static_cast<T>(i) / heightSegments, 0});
                }
            }
        }
    };

    template<typename T>
    class GridPyramid : public GridBase<T> {
    private:
        int baseSegments;
        T height, baseWidth;

    public:
        GridPyramid(int baseSegments, T baseWidth, T height, T spacing, T start = 0)
                : GridBase<T>(spacing, start), baseSegments(baseSegments), height(height), baseWidth(baseWidth) {}

        void generateGrid(std::function<void(int, vec3<T>)> func) {
            // Генерация базы
            for (int i = 0; i < baseSegments; ++i) {
                T angle = i * (2 * RN::PI_D / baseSegments);
                T x = baseWidth * cos(angle);
                T z = baseWidth * sin(angle);
                func(i, {x, 0, z});
            }
            // Вершина пирамиды
            func(baseSegments, {0, height, 0});
        }
    };

} // namespace RN

#endif // RAYON_CPP_CMAKE_GRID_H
