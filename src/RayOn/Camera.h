/*
 *
 *       Created by tux on 23.03.2024.
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
 *      Camera.h - simple perspective camera
 *
 */

#ifndef RAYON_CAMERA_H
#define RAYON_CAMERA_H

#include "Vec3.h"

namespace RN {

    class Camera {
    public:
        // focus length, image width(px), image height(px), camera position in scene coordinates
        Camera(double fl, int w_px, int h_px, vec3d c): _focal_length(fl), _image_width(w_px), _image_height(h_px), _center(c) {
            init();
        }

        Camera(double fl, int w_px, int h_px, vec3d c, double df): _focal_length(fl), _image_width(w_px), _image_height(h_px), _center(c), _defocus(df) {
            init();
        }

        void init() {
            _aspect = static_cast<double>(_image_width) / _image_height;
            _look_from = _center; // Assuming camera looks from the center unless specified otherwise

            // Calculate basis vectors for the camera
            _w = (_look_from - _look_at);
            //_focal_length = _w.length();
            _w.normalize();
            _u = vec3d::cross(_v_up, _w);
            _u.normalize();
            _v = vec3d::cross(_w, _u);
            _v.normalize();

            // Calculate viewport dimensions based on focal length and field of view
            double viewport_height = 2.0 * tan(v_fov / 2) * _focal_length;
            double viewport_width = viewport_height * _aspect;

            // Calculate the horizontal and vertical vectors for the viewport
            vec3d viewport_horizontal = _u * viewport_width;
            vec3d viewport_vertical = _v * viewport_height;

            // Calculate the upper left corner of the viewport
            vec3d viewport_upper_left = _center - _w * _focal_length - viewport_horizontal / 2 + viewport_vertical / 2;

            // Pixel size in world coordinates
            _pixel_size_u = viewport_horizontal / _image_width;
            _pixel_size_v = viewport_vertical / _image_height;

            // Starting pixel (center of the pixel)
            _start_pixel = viewport_upper_left + _pixel_size_u / 2 - _pixel_size_v / 2;

            if (_defocus > 0) {
                // If defocus is enabled, calculate defocus disk vectors
                double defocus_radius = tan(_defocus / 2) * _focal_length;
                _defocus_u = _u * defocus_radius;
                _defocus_v = _v * defocus_radius;
            }
        }

        vec3d defocus_disk_sample() const {
            // Generate a random point within the defocus disk
            vec3d p = vec3d::random_in_unit_disk();
            return _center + (_defocus_u * p.x) + (_defocus_v * p.y);
        }

        void lookAt(const vec3d &target) {
            _look_at = target;
            init(); // Reinitialize camera setup on lookAt change
        }

        [[nodiscard]] double focal_length() const { return _focal_length; }
        [[nodiscard]] double viewport_height() const { return _viewport_height; }
        [[nodiscard]] double viewport_width() const { return _viewport_width; }
        [[nodiscard]] double image_height() const { return _image_height; }
        [[nodiscard]] double image_width() const { return _image_width; }
        [[nodiscard]] vec3d center() const { return _center; }
        [[nodiscard]] vec3d pixelCenter(const vec2i &p) const { return _start_pixel + (_pixel_size_u * p.x) - (_pixel_size_v * p.y); }
        void setCenter(const vec3d &center) {
            _center = center;
            init();
        }



        [[nodiscard]] const vec3d &pixelSizeU() const {
            return _pixel_size_u;
        }

        [[nodiscard]] const vec3d &pixelSizeV() const {
            return _pixel_size_v;
        }
    private:
        double _aspect = 1.0;

        double _focal_length = 10.0;
        double _defocus = 0; // de-focus angle in radians

        vec3d _defocus_u;
        vec3d _defocus_v;

        double _viewport_width = 1.0;
        double _viewport_height = 1.0;

        double v_fov = 3.1415926535897932384626433832795 / 3;

        int _image_width = 1;
        int _image_height = 1;

        vec3d _pixel_size_u = {};
        vec3d _pixel_size_v = {};

        vec3d _center = vec3d(0, 0, 0);

        vec3d _start_pixel = {};

        // camera up direction
        vec3d _v_up = {0, 1, 0};

        vec3d _look_at = {0, 0, -1};
        vec3d _look_from = {0, 0, 0};

        vec3d   _u, _v, _w;
    };

} // RN

#endif //RAYON_CAMERA_H