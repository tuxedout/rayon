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

#ifndef RAYON_CPP_CMAKE_SCHUM_BW_H
#define RAYON_CPP_CMAKE_SCHUM_BW_H

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
#include "../RayOn/plus-d/Containers/RTMesh.h"

namespace RN {

    class SCHUMBWScene: public RTSceneCreator{
    public:
        void initScene(RN::RTScene &scene, RN::RTScene &lights, Camera &camera) override {

            RN::Materials::DiffusiveMaterial mat_light(RN::vec3d(3.0, 3.0, 3.0), RN::vec3d(1., 1., 1.));
            RN::Materials::DiffusiveMaterial mat_light2(
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.2, 0.92, 0.82),
                                                           vec3d(1.0, 1.0, 1.0), 10)),
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.4, 0.4, 0.4),
                                                           vec3d(1., 1., 1.), 10)));

            RN::Materials::DiffusiveMaterial mat_light22(
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.974, 0.974, 0.74),
                                                           vec3d(0.1, 0.1, 0.1), 20)),
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.9, 0.9, 0.9),
                                                           vec3d(0., 1., 1.), 20)));

            RN::Materials::DiffusiveMaterial mat_light3(RN::vec3d(0.610, 0.65, 0.2), RN::vec3d(1., 0., 1.));


            RN::Materials::SolidColorMaterial mat_gray02(RN::vec3d(0.2));
            RN::Materials::SolidColorMaterial mat_white(RN::vec3d(0.94));
            RN::Materials::SolidColorMaterial mat_black(RN::vec3d(0.1,0.1,0.1));
            RN::Materials::SolidColorMaterial mat_gray03(RN::vec3d(0.3));
            RN::Materials::SolidColorMaterial mat_gray04(RN::vec3d(0.4));
            RN::Materials::SolidColorMaterial mat_gray(RN::vec3d(0.5));
            RN::Materials::SolidColorMaterial mat_gray06(RN::vec3d(0.6));
            RN::Materials::SolidColorMaterial mat_gray07(RN::vec3d(0.6));

            RN::Materials::SolidColorMaterial lines(std::make_shared<RN::Textures::LinesTexture>(RN::Textures::LinesTexture(vec3d(1.0), vec3d(0.0), 50, false)));
            RN::Materials::SolidColorMaterial mat_check(std::make_shared<RN::Textures::CheckeredTexture>(RN::Textures::CheckeredTexture(vec3d(0), vec3d(1), 10)));

            RN::Materials::SolidColorMaterial mat_check2(std::make_shared<RN::Textures::CheckeredTexture>(RN::Textures::CheckeredTexture(vec3d(0), vec3d(1), 20)));

            RN::Materials::SolidColorMaterial mat_lines(std::make_shared<RN::Textures::LinesTexture>(RN::Textures::LinesTexture(vec3d(0), vec3d(1), 20,
                                                                                                                                false)));

            RN::Materials::SolidColorMaterial mat_lines2(std::make_shared<RN::Textures::LinesTexture>(RN::Textures::LinesTexture(vec3d(0), vec3d(1), 20,
                                                                                                                                 true)));

            RN::Materials::RefractingMaterial  mat_refr(RN::vec3d(1.0, 0.3, 0.730));
            mat_refr.reflectivity = 0.5;
            mat_refr.eta = 1.8;
            mat_refr.roughness = 0.0;

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

            RN::Materials::ReflectingMaterial  mat_blue_reflecting(RN::vec3d(0.41, 0.41, 0.91));
            mat_blue_reflecting.reflectivity = 0.7;
            mat_blue_reflecting.roughness = 0.0;

            RN::Materials::ReflectingMaterial  mat_silver_reflecting(RN::vec3d(0.84, 0.85, 0.85));
            mat_silver_reflecting.reflectivity = 0.25;
            mat_silver_reflecting.roughness = 0.1;

            /*
             * M * A * T * S
             */
            std::vector<std::shared_ptr<RN::Materials::Material>> mats;

            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_white));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_gray));
//            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_gray02));
//            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_gray03));
//            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_gray04));
//            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_gray06));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_gray07));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_check));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_check2));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_lines));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(lines));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_lines2));
//
//            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light));
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light2));
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light22));
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light3));
//            mats.push_back(std::make_shared<RN::Materials::Diffusive>(mat_light4));
//
//            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_blue_reflecting));
//            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_silver_reflecting));
//            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_silver_reflecting));
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

            std::string asspth = ASSETS_PATH;

            auto mesh = RN::RTMesh::loadFromOBJ(asspth + "schum.obj", std::make_shared<RN::Materials::RefractingMaterial>(mat_refr));
            scene.add(std::make_shared<RTMesh>(mesh));


            const int grid_items_size = 10;
            const double grid_spacing = 3.5;
            const double grid_width = (grid_items_size-1)*grid_spacing;

            RN::vec3d box_origin = mesh.bbox().getCenter() - vec3d(grid_width/2, grid_width/2, 25);

            RN::Grid3D<double> grid(grid_items_size, grid_items_size, 1, grid_spacing, box_origin);

            const double box_size = 3.1;

            RN::Interval<double> interval0_07(-box_size/2,box_size/2);

            grid.generateGrid([&scene, &materials, &image_shared_ptr, box_size, &interval0_07, &lights](int i, int k, int j, RN::vec3d c, RN::vec3d rc) {
                auto tmp_clr = image_shared_ptr->color({rc.x, 1.0 - rc.y}, {});
//                if (tmp_clr.x < 0.5) {
                    std::shared_ptr<RN::Materials::Material> mat_p = materials.getRandom();
                    RN::RTBox box(c + vec3d(0,0,interval0_07.getRandom()), box_size, mat_p);

                    if (mat_p->type() == Materials::mt_diffusive){
                        lights.add(std::make_shared<RN::RTBox>(box));
                    }

                    scene.add(std::make_shared<RN::RTBox>(box));
//                }
            });
//
            RN::RTSphere sphere2(box_origin + RN::vec3d(0, 1350, 300.0), 400.0,std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light));
            scene.add(std::make_shared<RN::RTSphere>(sphere2));
            lights.add(std::make_shared<RN::RTSphere>(sphere2));

            RN::vec3d cam_center =  mesh.bbox().getCenter() + vec3d(0,0,10);//RN::vec3d(box_origin.x + 280,box_origin.y + 330, box_origin.z + 710);

//            RN::RTQuad quadYX( box_origin + vec3d(-30,-25,-15), RN::vec3d(0,50, 0), RN::vec3d(60, 0, 0), std::make_shared<RN::Materials::Diffusive>(mat_light2));
//            scene.add(std::make_shared<RN::RTQuad>(quadYX));

            camera.setCenter(cam_center);
            camera.lookAt(mesh.bbox().getCenter());
        };
    };

} // RN

#endif // RAYON_CPP_CMAKE_SCHUM_BW_H