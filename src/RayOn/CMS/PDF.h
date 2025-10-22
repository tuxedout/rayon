/*
 *
 *       Created by tux on 31.05.2024.
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
 *      PDF.h
 *
 */

#ifndef RAYON_CPP_CMAKE_PDF_H
#define RAYON_CPP_CMAKE_PDF_H

#include <utility>

#include "../Vec3.h"
#include "ONB.h"
#include "../plus-d/RTDrawable.h"

namespace RN {

    class PDF {
        public:
            virtual ~PDF() = default;

            [[nodiscard]] virtual double value(const vec3d& direction) const = 0;
            [[nodiscard]] virtual vec3d generate(const vec3d &normal) const = 0;
    };

    class SpherePDF: public PDF {
        [[nodiscard]] double value(const vec3d& direction) const override {
            return 1/ (4 * RN::PI_D);
        }

        [[nodiscard]] vec3d generate(const vec3d &normal) const override {
            return vec3d::random_unit_vector();
        }
    };

    class HemiSpherePDF: public PDF {
        [[nodiscard]] double value(const vec3d& direction) const override {
            return 1/ (2*RN::PI_D);
        }

        [[nodiscard]] vec3d generate(const vec3d &normal) const override {
            return vec3d::random_on_hemisphere(normal);
        }
    };

    class CosinePDF: public PDF {
    public:
        explicit CosinePDF(const vec3d& w): uvw(w){

        }

        CosinePDF(const CosinePDF &other): uvw(other.uvw) {

        }

        [[nodiscard]] double value(const vec3d& direction) const override {
            auto cosine_theta = vec3d::dot(vec3d::normalize(direction), uvw.w());

            //return fmax(0, cosine_theta / (RN::PI_D));
            return cosine_theta / (RN::PI_D);
        }

        [[nodiscard]] vec3d generate(const vec3d &normal) const override {
            return uvw.local(vec3d::random_on_hemisphere(normal));
        }

        void set(const vec3d &normal) {
            uvw.build_from_w(normal);
        }

    private:
        ONB uvw;
    };

    class ItemPDF: public PDF {
    public:
        ItemPDF(const std::shared_ptr<RTItem> &objects, const vec3d &o)
                : objects(objects), _origin(o)
        {}

        ItemPDF()
                : objects(nullptr), _origin({0,0,0})
        {}

        [[nodiscard]] double value(const vec3d& direction) const override {
            return objects->pdfValue(_origin, direction); // Используем pdf объекта RTLightsList
        }

        [[nodiscard]] vec3d generate(const vec3d &normal) const override {
            return objects->random(_origin); // Используем pdf объекта RTLightsList
        }

        void setOrigin(const vec3d &o){
            _origin = o;
        }

    private:
        std::shared_ptr<RTItem> objects;
        vec3d _origin;
    };

    class ItemsPDF: public PDF {
    public:
        explicit ItemsPDF(const std::vector<std::shared_ptr<RTItem>> &objects)
                :_children(objects), _origin({0,0,0})
        {}

        ItemsPDF(const ItemsPDF &other): _children(other._children), _origin(other._origin) {

        }

        ItemsPDF(): _origin({0,0,0})
        {}

        [[nodiscard]] double value(const vec3d& direction) const override {
            auto weight = 1.0 / _children.size();
            auto sum = 0.0;

            for (const auto& object : _children)
                sum += object->pdfValue(_origin, direction);

            return sum * weight;
        }

        [[nodiscard]] vec3d generate(const vec3d &normal) const override {
            return _children[random_int(0, (int)_children.size()-1)]->random(_origin);
        }

        void setOrigin(const vec3d &o){
            _origin = o;
        }

        void setItems(const std::vector<std::shared_ptr<RTItem>> &items) {
            _children = items;
        }

    private:
        std::vector<std::shared_ptr<RTItem>> _children;
        vec3d _origin;
    };

    class CombinedPDF : public PDF {
    public:
        CombinedPDF(const std::shared_ptr<PDF> &p0, const std::shared_ptr<PDF> &p1) {
            p[0] = p0;
            p[1] = p1;
        }

        [[nodiscard]] double value(const vec3d& direction) const override {
            return 0.5 * p[0]->value(direction) + 0.5 *p[1]->value(direction);
        }

        [[nodiscard]] vec3d generate(const vec3d &normal) const override {
            if (RN::random() < 0.5) {
                return p[0]->generate(normal);
            }
            else {
                return p[1]->generate(normal);
            }
        }

        std::shared_ptr<PDF> p[2];
    };

    class CombinedItemsCosinePDF : public PDF {
    public:
        CombinedItemsCosinePDF(const ItemsPDF i_pdf, const CosinePDF c_pdf): other_PDF(c_pdf), items_PDF(i_pdf) {

        }

        [[nodiscard]] double value(const vec3d& direction) const override {
            return 0.5 * other_PDF.value(direction) + 0.5 * items_PDF.value(direction);
        }

        [[nodiscard]] vec3d generate(const vec3d &normal) const override {
            if (RN::random() < 0.5) {
                return other_PDF.generate(normal);
            } else {
                return items_PDF.generate(normal);
            }
        }

        CosinePDF other_PDF;
        ItemsPDF items_PDF;
    };

} // RN

#endif //RAYON_CPP_CMAKE_PDF_H
