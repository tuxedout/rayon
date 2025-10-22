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

#ifndef RAYON_CPP_CMAKE_TUX_H
#define RAYON_CPP_CMAKE_TUX_H

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
#include "../RayOn/plus-d/Primitives/RTEllipsoid.h"
#include "../RayOn/Textures/ImageTexture.h"

namespace RN {

    class Tux: public RTSceneCreator{
    public:
        void initScene(RN::RTScene &scene, RN::RTScene &lights, Camera &camera) override {

            RN::Materials::DiffusiveMaterial mat_light(RN::vec3d(1.8, 1.75, 1.482), RN::vec3d(0.3, 0.23, 0.3));
            RN::Materials::DiffusiveMaterial mat_light2(
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.0, 0.7, 0.4),
                                                           vec3d(1.0, 1.0, 0.1), 10)),
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.2, 0.4, 0.7),
                                                           vec3d(1.4, 0.6, 0.2), 10)));

            RN::Materials::DiffusiveMaterial mat_light22(
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.4, 0.1, 0.15),
                                                           vec3d(0.4, 0.1, 0.13), 20)),
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(1.0, 0.4, 0.4),
                                                           vec3d(1.2, 1.1, 1.3), 20)));

            RN::Materials::DiffusiveMaterial mat_light3(RN::vec3d(0.610, 0.75, 0.62), RN::vec3d(1.3, 0.5, 0.4));


            RN::Materials::SolidColorMaterial mat_orange(RN::vec3d(0.2,0.8,0.72));
            RN::Materials::SolidColorMaterial mat_white(RN::vec3d(0.94,0.94,0.94));
            RN::Materials::SolidColorMaterial mat_black(RN::vec3d(0.1,0.1,0.1));
            RN::Materials::SolidColorMaterial mat_red(RN::vec3d(0.92,0.18,0.172));
            RN::Materials::SolidColorMaterial mat_blue(RN::vec3d(0.1,0.8,0.92));
            RN::Materials::SolidColorMaterial mat_gray(RN::vec3d(0.5,0.5,0.5));
            RN::Materials::SolidColorMaterial mat_yellow(RN::vec3d(0.82,0.91,0.1));

            mat_yellow.roughness = 0.3;
            RN::Materials::SolidColorMaterial mat_check(std::make_shared<RN::Textures::CheckeredTexture>(RN::Textures::CheckeredTexture(vec3d(0), vec3d(1), 10)));
            mat_check.roughness = 0.1;

            RN::Materials::SolidColorMaterial mat_check2(std::make_shared<RN::Textures::CheckeredTexture>(RN::Textures::CheckeredTexture(vec3d(0), vec3d(1), 20)));
            mat_check.roughness = 0.1;

            RN::Materials::RefractingMaterial  mat_refr(RN::vec3d(1.0, 1.0, 1.0));
            mat_refr.reflectivity = 0.05;
            mat_refr.eta = 1.9;
            mat_refr.roughness = 0.0;

            RN::Materials::RefractingMaterial  mat_refr2(RN::vec3d(0.92, 0.72, 1.0));
            mat_refr2.reflectivity = 0.9;
            mat_refr2.eta = 2.1;
            mat_refr2.roughness = 0.0;

            RN::Materials::DiffusiveMaterial mat_light4(RN::vec3d(0.0, 0.5, 0.01), RN::vec3d(1.03, 0.8, 0.2));
            RN::Materials::DiffusiveMaterial mat_light_ec(RN::vec3d(0.985, 0.8, 0.589), RN::vec3d(0.15, 0.15, 0.14));

            std::string assets_path = ASSETS_PATH;

            std::shared_ptr<RN::Textures::ImageTexture> image_shared_ptr;

            auto opt_image = RN::Textures::ImageTexture::Load(assets_path + "home.png");
            if (opt_image) {
                image_shared_ptr = std::make_shared<RN::Textures::ImageTexture>(std::move(*opt_image));
            } else {
                // Handle the error: opt_image is empty
                std::cerr << "image is empty" << "\n";
            }

            RN::Materials::SolidColorMaterial texture_material(image_shared_ptr);

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

            RN::Materials::ReflectingMaterial  mat_pink_reflecting(RN::vec3d(0.92, 0.2, 0.31));
            mat_blue_reflecting.reflectivity = 0.95;
            mat_blue_reflecting.roughness = 0.1;

            /*
             * M * A * T * S
             */
            std::vector<std::shared_ptr<RN::Materials::Material>> mats;

            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_white));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_blue));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_red));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_yellow));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_check));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_check2));
//
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light));
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light2));
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light22));
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light3));
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light4));

            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_blue_reflecting));
            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_gold_reflecting));
            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_silver_reflecting));
            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_pink_reflecting));
//            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(texture_material));

            RN::DiscreteInterval materials(mats);

            auto start = std::chrono::high_resolution_clock::now();  // start timer
            auto end = std::chrono::high_resolution_clock::now();  // end timer

            scene.background = RN::vec3d(0.0,0.0,0.0);

            RN::Interval<double> intrvl_m50p50(-50.0,50.0);

            RN::Materials::RefractingMaterial  mat_refl(RN::vec3d(1.0, 1.0, 1.0));
            mat_refl.reflectivity = 0.715;
            mat_refl.roughness = 0.0;

            scene.background = RN::vec3d(0.0,0.0,0.0);

            RN::vec3d box_origin = RN::vec3d(0,0,0);


            RN::Grid3D<double> grid(64, 64, 32, 10, box_origin + vec3d(-100, 0,-25));

            grid.generateGrid([&scene, &materials, &image_shared_ptr](int i, int k, int j, RN::vec3d c, RN::vec3d rc) {
                auto tmp_clr = image_shared_ptr->color({rc.x, 1.0 - rc.y}, {});
                if (tmp_clr.x < 0.5) {
                    RN::RTBox box(c, 7, materials.getRandom());
                    scene.add(std::make_shared<RN::RTBox>(box));
                }
            });

            RN::RTSphere sphere2(box_origin + RN::vec3d(0, 2550, 1500.0), 1300.0,std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light));
            scene.add(std::make_shared<RN::RTSphere>(sphere2));

            RN::vec3d cam_center = box_origin + grid.start + grid.getCenterOffset() + vec3d(-30, 20,680);//RN::vec3d(box_origin.x + 280,box_origin.y + 330, box_origin.z + 710);

            camera.setCenter(cam_center);
            camera.lookAt(box_origin + grid.start + grid.getCenterOffset());
        };
    };

} // RN

#endif // RAYON_CPP_CMAKE_TUX_H