/*
 *
 *       Created by tux on 15.04.2024.
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
 *      MainScene.h
 *
 */

#ifndef RAYON_CPP_CMAKE_MAINSCENE_H
#define RAYON_CPP_CMAKE_MAINSCENE_H

#include "../RayOn/plus-d/RTSceneCreator.h"
#include "../RayOn/Materials/ReflectingMaterial.h"
#include "../RayOn/Materials/RefractingMaterial.h"
#include "../RayOn/Materials/DiffusiveMaterial.h"
#include "../RayOn/Textures/CheckeredTexture.h"
#include "../RayOn/Textures/CheckeredColorTexture.h"
#include "../RayOn/Textures/LinesTexture.h"
#include "../Misc/DiscreteInterval.h"
#include "../Misc/Grid.h"
#include "../RayOn/plus-d/Primitives/RTBox.h"
#include "../RayOn/plus-d/Primitives/RTSphere.h"
#include "../RayOn/plus-d/Primitives/RTQuad.h"
#include "../RayOn/plus-d/Primitives/RTCylinder.h"

namespace RN {

    class MainScene: public RTSceneCreator{
    public:
        void initScene(RN::RTScene &scene, RN::RTScene &lights, Camera &camera) override {
            std::vector<std::shared_ptr<RN::Materials::Material>> mats;
            std::vector<std::shared_ptr<RN::Materials::Material>> mats_light;
            std::vector<std::shared_ptr<RN::Materials::Material>> mats_refr;
            std::vector<std::shared_ptr<RN::Materials::Material>> mats_refl;

            // scene setup
            // materials
            RN::Interval<double> z0p1(0,1);
            RN::Interval<double> z05p1(0.5,0.9);

            RN::Materials::ReflectingMaterial  mat_black_reflecting(RN::vec3d(0.9, 0.11, 0.56));
            mat_black_reflecting.reflectivity = 0.7;
            mat_black_reflecting.roughness = 0.0;

            RN::Materials::ReflectingMaterial  mat_gold_reflecting(RN::vec3d(0.94, 0.65, 0.1));
            mat_gold_reflecting.reflectivity = 0.59;
            mat_gold_reflecting.roughness = 0.2;

            RN::Materials::ReflectingMaterial  mat_silver_reflecting(RN::vec3d(0.84, 0.85, 0.91));
            mat_silver_reflecting.reflectivity = 0.25;
            mat_silver_reflecting.roughness = 0.0;

            RN::Materials::ReflectingMaterial  mat_blue_reflecting(RN::vec3d(0.2, 0.85, 0.91));
            mat_blue_reflecting.reflectivity = 0.95;
            mat_blue_reflecting.roughness = 0.1;

            RN::Materials::RefractingMaterial  mat_refr(RN::vec3d(1.0, 1.0, 1.0));
            mat_refr.reflectivity = 0.05;
            mat_refr.eta = 1.9;
            mat_refr.roughness = 0.0;

            RN::Materials::RefractingMaterial  mat_refr2(RN::vec3d(0.92, 0.72, 1.0));
            mat_refr2.reflectivity = 0.9;
            mat_refr2.eta = 2.1;
            mat_refr2.roughness = 0.0;

            RN::Materials::DiffusiveMaterial mat_light(RN::vec3d(0.3, 0.23, 0.3), RN::vec3d(3.8, 3.75, 3.482));
            RN::Materials::DiffusiveMaterial mat_light2(RN::vec3d(0.2, 0.12, 0.72), RN::vec3d(1.5, 0.51, 0.127));
            RN::Materials::DiffusiveMaterial mat_light3(RN::vec3d(0.110, 0.75, 0.62), RN::vec3d(1.3, 0.5, 2.4));
            RN::Materials::DiffusiveMaterial mat_light4(RN::vec3d(0.0, 0.5, 0.01), RN::vec3d(1.43, 0.8, 2.2));
            RN::Materials::DiffusiveMaterial mat_light_ec(RN::vec3d(0.985, 0.8, 0.589), RN::vec3d(1.15, 0.85, 0.14));

            RN::Materials::DiffusiveMaterial mat_light21(
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.0, 0.7, 0.4),
                                                           vec3d(0.9, 0.9, 0.1), 10)),
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.2, 2.4, 0.7),
                                                           vec3d(2.4, 2.6, 2.2), 10)));

            RN::Materials::DiffusiveMaterial mat_light22(
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.74, 0.1, 0.15),
                                                           vec3d(0.4, 0.75, 0.93), 20)),
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.90, 0.4, 0.4),
                                                           vec3d(1.62, 0.71, 0.13), 20)));

            RN::Materials::SolidColorMaterial mat_check(std::make_shared<RN::Textures::CheckeredTexture>(RN::Textures::CheckeredTexture(RN::vec3d(1, 1, 1), RN::vec3d(0, 0, 0), 10.0)));
            mat_check.roughness = 0.0;
            RN::Materials::SolidColorMaterial mat_check2(std::make_shared<RN::Textures::CheckeredTexture>(RN::Textures::CheckeredTexture(RN::vec3d(0, 0, 0), RN::vec3d(1, 1, 1), 20.0)));
            mat_check2.roughness = 0.0;
            RN::Materials::SolidColorMaterial mat_check3(std::make_shared<RN::Textures::CheckeredColorTexture>(RN::Textures::CheckeredColorTexture()));
            mat_check3.roughness = 0.0;

            RN::Materials::SolidColorMaterial mat_lines(std::make_shared<RN::Textures::LinesTexture>(RN::vec3d(1, 1, 1), RN::vec3d(0, 0, 0), 100, false));
            mat_lines.roughness = 0.0;
            RN::Materials::SolidColorMaterial mat_lines2(std::make_shared<RN::Textures::LinesTexture>(RN::vec3d(1, 1, 1), RN::vec3d(0, 0, 0), 100, true));
            mat_lines2.roughness = 0.0;
            RN::Materials::SolidColorMaterial mat_lines3(std::make_shared<RN::Textures::LinesTexture>(RN::vec3d(1, 1, 1), RN::vec3d(0, 0, 0), 10, false));
            mat_lines3.roughness = 0.0;

            RN::Materials::SolidColorMaterial mat_orange(RN::vec3d(0.2,0.8,0.72));
            RN::Materials::SolidColorMaterial mat_white(RN::vec3d(0.94,0.94,0.94));
            RN::Materials::SolidColorMaterial mat_black(RN::vec3d(0.1,0.1,0.1));
            RN::Materials::SolidColorMaterial mat_red(RN::vec3d(0.92,0.18,0.172));

            RN::Materials::SolidColorMaterial mat_blue(RN::vec3d(0.1,0.8,0.92));

            RN::Materials::SolidColorMaterial mat_gray(RN::vec3d(0.5,0.5,0.5));

            RN::Materials::SolidColorMaterial mat_yellow(RN::vec3d(0.82,0.91,0.1));

            RN::Materials::ReflectingMaterial mat_big(RN::vec3d(1.0, 0.1, 1.0));
            mat_big.reflectivity = 0.0;

            RN::Materials::ReflectingMaterial mat_0(RN::vec3d(0.2, 0.92, 0.1));
            mat_0.reflectivity = 0.2;

            RN::Materials::ReflectingMaterial mat_1(RN::vec3d(0.0, 1.0, 0.2));
            mat_1.reflectivity = z05p1.getRandom();

            RN::Materials::ReflectingMaterial mat_2(RN::vec3d(z0p1.getRandom(), z0p1.getRandom(), z0p1.getRandom()));
            mat_2.reflectivity = z0p1.getRandom();

            RN::Materials::ReflectingMaterial mat_3(RN::vec3d(z05p1.getRandom(), z0p1.getRandom(), z0p1.getRandom()));
            mat_3.reflectivity = 1.0 - z0p1.getRandom();

            RN::Materials::ReflectingMaterial mat_4(RN::vec3d(z05p1.getRandom(), z0p1.getRandom(), z0p1.getRandom()));
            mat_4.reflectivity = 1.0 - z0p1.getRandom();

            /*
             * M * A * T * S
             */
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_check));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_check2));
//    mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_check3));
            mats_refr.push_back(std::make_shared<RN::Materials::RefractingMaterial>(mat_refr));
            mats_refr.push_back(std::make_shared<RN::Materials::RefractingMaterial>(mat_refr2));
            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_black_reflecting));
            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_blue_reflecting));
            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_silver_reflecting));
            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_gold_reflecting));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_red));
//    mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_lines));
//    mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_lines2));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_lines3));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_orange));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_blue));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_yellow));
//    mats.push_back(std::make_shared<RN::Materials::Diffusive>(mat_light_ec));

//    mats_light.push_back(std::make_shared<RN::Materials::Reflecting>(mat_black_reflecting));
//    mats_light.push_back(std::make_shared<RN::Materials::Reflecting>(mat_blue_reflecting));
//    mats_light.push_back(std::make_shared<RN::Materials::Reflecting>(mat_silver_reflecting));
//    mats_light.push_back(std::make_shared<RN::Materials::Reflecting>(mat_gold_reflecting));
//    mats_light.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_red));
//    mats_light.push_back(std::make_shared<RN::Materials::Diffusive>(mat_light));
            mats_light.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light21));
            mats_light.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light22));
            mats_light.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light3));
            mats_light.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light_ec));
            mats_light.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light4));

            mats_refl.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_black_reflecting));
            mats_refl.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_blue_reflecting));
            mats_refl.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_silver_reflecting));
            mats_refl.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_gold_reflecting));

            RN::DiscreteInterval materials(mats);
            RN::DiscreteInterval materials_light(mats_light);
            RN::DiscreteInterval materials_refr(mats_refr);
            RN::DiscreteInterval materials_refl(mats_refl);

            auto start = std::chrono::high_resolution_clock::now();  // start timer
            auto end = std::chrono::high_resolution_clock::now();  // end timer


            scene.background = RN::vec3d(0.0,0.0,0.0);

            RN::Interval<double> radius_interval(0.1, 5.4);
            RN::Interval<double> x_interval(-50.0,50.0);
            RN::Interval<double> y_interval(0.0,60.0);
            RN::Interval<double> z_interval(-60.0,-2.0);
            RN::Interval<double> z2_interval(30.0,80.0);

            //RN::RTSphere sphere(RN::vec3d(0,-6000.5, -1), 6000, std::make_shared<RN::Materials::SolidColorMaterial>(mat_lines));
            //scene.add(std::make_shared<RN::RTSphere>(sphere));

            const int grid_size = 10;

            RN::Grid3D<double> grid(2, 2, 2, 176);
            RN::Grid3D<double> grid22(8, 8, 8, 22);

            grid22.generateGrid([&z2_interval, &scene, &materials, &lights, &mat_light_ec, &mat_black, &mats, &materials_refr, &materials_refl, &materials_light](int i, int k, int j, RN::vec3d c, RN::vec3d rc){
                double z_val = z2_interval.getRandom();

                RN::vec3d position = c + RN::vec3d(10,10,10);

                const double radius = 3.5;

                if (RN::random() > 0.65) {

                    if (RN::random() > 0.5) {
                        if (RN::random() > 0.5) {
                            RN::RTCylinder _cyl(position, radius, radius,
                                           materials_refl.getRandom());
                            scene.add(std::make_shared<RN::RTCylinder>(_cyl));
                        }else{
                            RN::RTBox _box(position, RN::vec3d(radius * 2.0 - RN::EPSILON_100_D * 10,
                                                               radius * 2.0 - RN::EPSILON_100_D * 10,
                                                               radius * 2.0 - RN::EPSILON_100_D * 10), materials_light.getRandom());
                            scene.add(std::make_shared<RN::RTBox>(_box));
                            lights.add(std::make_shared<RN::RTBox>(_box));
                        }
                    }else{
                        if (RN::random() > 0.5) {
                            RN::RTBox _box(position, RN::vec3d(radius * 2.0 - RN::EPSILON_100_D * 10,
                                                               radius * 2.0 - RN::EPSILON_100_D * 10,
                                                               radius * 2.0 - RN::EPSILON_100_D * 10), materials_refl.getRandom());
                            scene.add(std::make_shared<RN::RTBox>(_box));
                        } else {

                            RN::RTBox _box(position, RN::vec3d(radius * 2.0 - RN::EPSILON_100_D * 10,
                                                               radius * 2.0 - RN::EPSILON_100_D * 10,
                                                               radius * 2.0 - RN::EPSILON_100_D * 10), materials.getRandom());
                            scene.add(std::make_shared<RN::RTBox>(_box));
                        }
                    }

//            RN::RTSphere sphere(RN::vec3d(c), radius, materials_light.getRandom());
//            scene.add(std::make_shared<RN::RTSphere>(sphere));
                }else{
                    RN::RTSphere sphere(position, radius, materials_refr.getRandom());
                    scene.add(std::make_shared<RN::RTSphere>(sphere));
                }
            });


//    RN::Grid2D<double> grid(210,70,20);
//
//    RN::Textures::Image tux = RN::Textures::Image::Load(std::string(ASSETS_PATH) + "tux_small.png");
//
//    grid.generateGrid([&z2_interval, &scene, &materials, &tux, &mat_light_ec, &mat_black, &mats, &materials_light](int i, int k, RN::vec2d c, RN::vec2d rc){
//                double z_val = z2_interval.getRandom();
//
//                RN::vec3d tmp_color = tux.color(rc, {});
//
//                if (tmp_color.length() > 0.5) {
//                    const double radius = 9.0;
//                    RN::RTBox _box(RN::vec3d( c.x, c.y, 0.0), RN::vec3d(radius*2.0 - RN::EPSILON_100_D*10,radius*2.0 - RN::EPSILON_100_D*10,z_val), materials_light.getRandom());
//                    scene.add(std::make_shared<RN::RTBox>(_box));
//                } else {
//                    const double radius = 10.0;
//                    RN::RTBox _box(RN::vec3d( c.x, c.y, 0.0), RN::vec3d(radius*2.0 - RN::EPSILON_100_D*10,radius*2.0 - RN::EPSILON_100_D*10,z_val), materials.getRandom());
//                    scene.add(std::make_shared<RN::RTBox>(_box));
//                }
//    });

//    const RN::vec2i grid_size_wh = {300, 100};
//
//    RN::vec2d grid_start = {-radius*(grid_size_wh.x)+radius, radius + 0.5};
//    RN::vec3d grid_center = {0, radius*(grid_size_wh.y)+radius, 0};
        RN::vec3d grid_center = grid.getCenterOffset();

        RN::RTSphere sphere2(RN::vec3d(grid_center.x + 500, grid_center.y + 1500, grid_center.z + 100), 900.0,std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light));
        scene.add(std::make_shared<RN::RTSphere>(sphere2));
        lights.add(std::make_shared<RN::RTSphere>(sphere2));

//    RN::RTSphere sphere4(RN::vec3d(grid_center.x+6.0, grid_center.y, -5.0), 3.0,std::make_shared<RN::Materials::Diffusive>(mat_light3));
//    scene.add(std::make_shared<RN::RTSphere>(sphere4));
//
//    RN::RTSphere sphere5(RN::vec3d(grid_center.x, grid_center.y-6.0, -5.0), 3.0,std::make_shared<RN::Materials::Diffusive>(mat_light4));
//    scene.add(std::make_shared<RN::RTSphere>(sphere5));
//    RN::RTSphere sphere6(RN::vec3d(grid_center.x, grid_center.y+6.0, -5.0), 3.0,std::make_shared<RN::Materials::Diffusive>(mat_light2));
//    scene.add(std::make_shared<RN::RTSphere>(sphere6));

//    RN::RTSphere sphere7(RN::vec3d(-15.5, 15.5, -25.0), 10.0,std::make_shared<RN::Materials::Refracting>(mat_refr2));
//    scene.add(std::make_shared<RN::RTSphere>(sphere7));
//    RN::RTSphere sphere8(RN::vec3d(0.0, 15.5, -20), 1.5, std::make_shared<RN::Materials::Refracting>(mat_refr2));
//    scene.add(std::make_shared<RN::RTSphere>(sphere8));
//    RN::RTSphere sphere9(RN::vec3d(0.0, 15.5, -25), 1.5, std::make_shared<RN::Materials::SolidColorMaterial>(mat_lines3));
//    scene.add(std::make_shared<RN::RTSphere>(sphere9));
//    RN::RTSphere sphere10(RN::vec3d(0.0, 15.5, -35), 8.0, std::make_shared<RN::Materials::SolidColorMaterial>(mat_lines2));
//    scene.add(std::make_shared<RN::RTSphere>(sphere10));
//    RN::RTBox box(RN::vec3d( -4.0, 4500.5, -55), RN::vec3d(1000.0,10.0,1000.0), std::make_shared<RN::Materials::Diffusive>(mat_light));
//    scene.add(std::make_shared<RN::RTBox>(box));



//    for (int y = 0; y < grid_size_wh.y; y++) {
//        for (int x = 0; x < grid_size_wh.x; x++) {
//            //RN::RTSphere _sphere(RN::vec3d(grid_start.x + x * radius, grid_start.y + y * radius, 0.0), radius,std::make_shared<RN::Materials::Refracting>(mat_refr2));
////            if (x*y % 2) {
//                double z_val = z2_interval.getRandom();
//
//                RN::RTBox _box(RN::vec3d(grid_start.x + x * radius*2.0, grid_start.y + y * radius*2.0, 0.0), RN::vec3d(radius*2.0 - RN::EPSILON_100_D*10,radius*2.0 - RN::EPSILON_100_D*10,z_val), materials.getRandom());
//                scene.add(std::make_shared<RN::RTBox>(_box));
////                RN::RTSphere _sphere(RN::vec3d(grid_start.x + x * radius*2.0, grid_start.y + y * radius*2.0, z_val + 3 * radius), radius, materials.getRandom());
////                scene.add(std::make_shared<RN::RTSphere>(_sphere));
////            }else{
////                RN::RTSphere _sphere(RN::vec3d(grid_start.x + x * radius*2.0, grid_start.y + y * radius*2.0, z2_interval.getRandom()), radius, materials.getRandom());
////                scene.add(std::make_shared<RN::RTSphere>(_sphere));
////            }
//        }
//    }

            //RN::DiscreteInterval<std::shared_ptr<RN::Materials::Material>> materials(mats);

            /*for (int i = 0; i < 10000; i++) {
                while (true) {
                    RN::vec3 rnd_vec = RN::vec3d(x_interval.getRandom(),y_interval.getRandom(), z_interval.getRandom());

                    RN::RTSphere l_sphere(rnd_vec, radius_interval.getRandom(), materials.getRandom());

                    //scene.add(std::make_shared<RN::RTSphere>(l_sphere));
                    if (!scene.intersects(l_sphere.bbox())) {
                        scene.add(std::make_shared<RN::RTSphere>(l_sphere));
                        break;
                    }
                }
            }*/

            /*
             *  C
             *  A
             *  M
             *  E
             *  R
             *  A
             */

            RN::vec3d cam_center = RN::vec3d(grid_center.x + 170,grid_center.y + 170, grid_center.z + 170);

            camera.setCenter(cam_center);

            RN::vec3d box_origin = RN::vec3d(-5,-5,-5);

            RN::Materials::DiffusiveMaterial mat_light_check(
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.7, 0.2, 0.4),
                                                           vec3d(0.92, 0.95, 0.1), 10)),
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(1.2, 0.1, 0.1),
                                                           vec3d(1.4, 1.6, 0.1), 10)));

            RN::RTQuad quadYZ( box_origin, RN::vec3d(0,grid_center.y * 2 + 10, 0), RN::vec3d(0,0,grid_center.z * 2 + 10), std::make_shared<RN::Materials::SolidColorMaterial>(mat_white));
            scene.add(std::make_shared<RN::RTQuad>(quadYZ));

            RN::RTQuad quadYX( box_origin, RN::vec3d(0,grid_center.y * 2 + 10, 0), RN::vec3d(grid_center.x * 2 + 10, 0, 0), std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light_check));
            scene.add(std::make_shared<RN::RTQuad>(quadYX));
            lights.add(std::make_shared<RN::RTQuad>(quadYX));

            RN::RTQuad quadXZ( box_origin, RN::vec3d(grid_center.x * 2 + 10,0, 0), RN::vec3d(0, 0, grid_center.z * 2 + 10), std::make_shared<RN::Materials::SolidColorMaterial>(mat_yellow));
            scene.add(std::make_shared<RN::RTQuad>(quadXZ));

//    camera.lookAt(sphere2.center());
            camera.lookAt({grid_center.x+50, grid_center.y, grid_center.z});
        };
    };

} // RN

#endif //RAYON_CPP_CMAKE_MAINSCENE_H
