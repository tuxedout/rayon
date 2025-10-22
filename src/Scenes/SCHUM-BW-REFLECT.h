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

#ifndef RAYON_CPP_CMAKE_SCHUM_BW_REFLECT_H
#define RAYON_CPP_CMAKE_SCHUM_BW_REFLECT_H

#include "../RayOn/plus-d/RTSceneCreator.h"
#include "../RayOn/Materials/ReflectingMaterial.h"
#include "../RayOn/Materials/RefractingMaterial.h"
#include "../RayOn/Materials/DiffusiveMaterial.h"
#include "../RayOn/Materials/DebugMaterials.h"
#include "../RayOn/Materials/RasterColorMaterial.h"
#include "../RayOn/Textures/CheckeredTexture.h"
#include "../RayOn/Textures/CheckeredColorTexture.h"
#include "../RayOn/Textures/LinesTexture.h"
#include "../RayOn/Textures/WireframeTexture.h"
#include "../Misc/DiscreteInterval.h"
#include "../Misc/Grid.h"
#include "../RayOn/plus-d/Primitives/RTBox.h"
#include "../RayOn/plus-d/Primitives/RTSphere.h"
#include "../RayOn/plus-d/Primitives/RTQuad.h"
#include "../RayOn/plus-d/Primitives/RTEllipsoid.h"
#include "../RayOn/Textures/ImageTexture.h"
#include "../RayOn/plus-d/Containers/RTMesh.h"
#include "../RayOn/plus-d/Primitives/RTBBWireframe.h"

namespace RN {

    class SCHUMBWReflectScene: public RTSceneCreator{
    public:
        void initScene(RN::RTScene &scene, RN::RTScene &lights, Camera &camera) override {

            RN::Materials::ColorMaterial mat_debug(RN::Color::Red);
            RN::Materials::ColorMaterial mat_debug2(std::make_shared<RN::Textures::WireframeTexture>(RN::Color::Red));
            RN::Materials::NormalMaterial mat_normal;
            RN::Materials::DepthMaterial mat_depth;

            RN::Materials::DiffusiveMaterial mat_light(4*RN::vec3d(1.0,1.0,1.0), RN::vec3d(1.0, 1.0, 1.0));
            RN::Materials::DiffusiveMaterial mat_light_red(RN::Color::Red * 2, RN::Color::Red);
            RN::Materials::DiffusiveMaterial mat_light_orange(RN::Color::Orange*2, RN::Color::Orange);

            RN::Materials::DiffusiveMaterial mat_light2(
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.2, 0.92, 0.82),
                                                           vec3d(1.0, 1.0, 1.0), 10)),
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.4, 0.4, 0.4),
                                                           vec3d(1., 1., 1.), 10)));

            RN::Materials::DiffusiveMaterial mat_light22(
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.74, 0.4, 0.974),
                                                           vec3d(0.1, 0.1, 0.1), 20)),
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.9, 0.9, 0.9),
                                                           vec3d(0., 1., 1.), 20)));

            RN::Materials::DiffusiveMaterial mat_light3(RN::vec3d(0.610, 0.65, 0.2), RN::vec3d(1., 0., 1.));

            /*
             * SOLID COLORS
             */
            RN::Materials::SolidColorMaterial mat_orange(RN::Color::Orange);
            RN::Materials::SolidColorMaterial mat_white(RN::vec3d(0.94,0.94,0.94));
            RN::Materials::SolidColorMaterial mat_black(RN::vec3d(0.1,0.1,0.1));
            RN::Materials::SolidColorMaterial mat_red(RN::vec3d(0.92,0.18,0.172));
            RN::Materials::SolidColorMaterial mat_green(RN::vec3d(0.1,0.98,0.172));
            RN::Materials::SolidColorMaterial mat_blue(RN::vec3d(0.1,0.1,0.92));
            RN::Materials::SolidColorMaterial mat_yellow(RN::Color::Yellow);
            RN::Materials::SolidColorMaterial mat_gray(RN::vec3d(0.5));
            RN::Materials::SolidColorMaterial mat_pink(RN::Color::Pink);

            /*
             * LINES
             */
            RN::Materials::SolidColorMaterial mat_lines(std::make_shared<RN::Textures::LinesTexture>(RN::Textures::LinesTexture(vec3d(1.0), vec3d(0.0), 50, false)));
            RN::Materials::SolidColorMaterial mat_lines2(std::make_shared<RN::Textures::LinesTexture>(RN::Textures::LinesTexture(vec3d(0.0), vec3d(1.0), 100, true)));
            RN::Materials::SolidColorMaterial mat_lines3(std::make_shared<RN::Textures::LinesTexture>(RN::Textures::LinesTexture(RN::Color::Pink, RN::Color::Black, 20, false)));
            RN::Materials::SolidColorMaterial mat_lines4(std::make_shared<RN::Textures::LinesTexture>(RN::Textures::LinesTexture(vec3d(0), RN::Color::Orange, 20, true)));

            /*
             * CHECK
             */
            RN::Materials::SolidColorMaterial mat_check(std::make_shared<RN::Textures::CheckeredTexture>(RN::Textures::CheckeredTexture(vec3d(0), vec3d(1), 10)));
            RN::Materials::SolidColorMaterial mat_check2(std::make_shared<RN::Textures::CheckeredTexture>(RN::Textures::CheckeredTexture(vec3d(0), vec3d(1), 20)));

            /*
             * REFR
             */
            RN::Materials::RefractingMaterial  mat_refr(RN::vec3d(1.0, 1.0, 1.0));
            mat_refr.reflectivity = 0.20;
            mat_refr.eta = 1.3;
            mat_refr.roughness = 0.0;

            /*
             * REFLECT
             */
            RN::Materials::ReflectingMaterial  mat_black_reflecting(RN::vec3d(0.9, 0.11, 0.56));
            mat_black_reflecting.reflectivity = 0.7;
            mat_black_reflecting.roughness = 0.0;

            RN::Materials::ReflectingMaterial  mat_gold_reflecting(RN::vec3d(0.94, 0.65, 0.1));
            mat_gold_reflecting.reflectivity = 0.9;
            mat_gold_reflecting.roughness = 0.0;

            RN::Materials::ReflectingMaterial  mat_silver_reflecting(RN::vec3d(0.84, 0.85, 0.91));
            mat_silver_reflecting.reflectivity = 0.95;
            mat_silver_reflecting.roughness = 0.0;

            RN::Materials::ReflectingMaterial  mat_blue_reflecting(RN::vec3d(0.2, 0.85, 0.91));
            mat_blue_reflecting.reflectivity = 0.95;
            mat_blue_reflecting.roughness = 0.0;

            RN::Materials::ReflectingMaterial  mat_pink_reflecting(RN::vec3d(0.92, 0.2, 0.31));

            RN::Materials::ReflectingMaterial  mat_yellow_reflecting(RN::Color::Yellow);
            mat_yellow_reflecting.reflectivity = 0.95;
            mat_yellow_reflecting.roughness = 0.0;


            /*
             * M * A * T * S
             */
            std::vector<std::shared_ptr<RN::Materials::Material>> mats;

            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_red));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_orange));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_yellow));
//            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_gray07));

//            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_check));
//            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_check2));
//
//            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_lines3));
//            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(mat_lines4));

//            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light));
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light2));
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light22));
//            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light3));
//            mats.push_back(std::make_shared<RN::Materials::Diffusive>(mat_light4));
////
//            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_blue_reflecting));
//            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_silver_reflecting));
            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_yellow_reflecting));
            //mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_pink_reflecting));
//            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(texture_material));

            RN::DiscreteInterval materials(mats);

            scene.background = RN::vec3d(0.0,0.0,0.0);

            std::string assets_path = ASSETS_PATH;

            RN::Materials::RasterColorMaterial mat_raster(RN::Color::White);

            auto mesh = RN::RTMesh::loadFromOBJ(assets_path + "skullobj.obj", std::make_shared<RN::Materials::RefractingMaterial>(mat_refr));
            Transforms::RTRotate rotate_translate_scale(std::make_shared<RTMesh>(mesh), {0,1,0}, -0.25 * RN::PI_D);
            Transforms::RTScale scale(std::make_shared<Transforms::RTRotate>(rotate_translate_scale), {2.3});
            Transforms::RTTranslate translate_scale(std::make_shared<Transforms::RTScale>(scale), {0.75,0.0,-0.2});

            scene.add(std::make_shared<Transforms::RTTranslate>(translate_scale));

            //scene.add(std::make_shared<RTBox>(mesh.bbox().getCenter(), mesh.bbox().getSize(), std::make_shared<RN::Materials::ColorMaterial>(mat_debug2)));
            //scene.add(std::make_shared<RTLine>(vec3d(0,0,0), vec3d(100,60,70), 3.3, std::make_shared<RN::Materials::ColorMaterial>(mat_debug)));
            //scene.add(std::make_shared<RTBBWireframe>(mesh.bbox().min, mesh.bbox().max, std::make_shared<RN::Materials::ColorMaterial>(mat_debug2)));

            const int grid_items_size = 20;
            const double grid_spacing = 1.5;
            const double grid_width = (grid_items_size-1)*grid_spacing;

            RN::vec3d box_origin = mesh.bbox().getCenter() - vec3d(grid_width/2, grid_width/2, 25);

            RN::Grid3D<double> grid(grid_items_size, grid_items_size, 1, grid_spacing, box_origin);

            const double box_size = 1.1;

            RN::Interval<double> interval0_07(-box_size/2,box_size/2);

//            grid.generateGrid([&scene, &materials, box_size, &interval0_07, &lights](int i, int k, int j, RN::vec3d c, RN::vec3d rc) {
////                if (tmp_clr.x < 0.5) {
//                    std::shared_ptr<RN::Materials::Material> mat_p = materials.getRandom();
//                    RN::RTBox box(c + vec3d(0,0,interval0_07.getRandom()), box_size, mat_p);
//
//                    if (mat_p->type() == Materials::mt_diffusive){
//                        lights.add(std::make_shared<RN::RTBox>(box));
//                    }
//
//                    scene.add(std::make_shared<RN::RTBox>(box));
////                }
//            });

            RN::RTSphere sphere2(mesh.bbox().getCenter() + RN::vec3d(0, 0, 2500), 400.0,std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light));
            scene.add(std::make_shared<RN::RTSphere>(sphere2));
            lights.add(std::make_shared<RN::RTSphere>(sphere2));

            RN::RTSphere sphere3(mesh.bbox().getCenter() + RN::vec3d(250, 1300, 500), 400.0,std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light_red));
            scene.add(std::make_shared<RN::RTSphere>(sphere3));
            lights.add(std::make_shared<RN::RTSphere>(sphere3));

            RN::RTSphere sphere4(mesh.bbox().getCenter() + RN::vec3d(-1350, 300, 500), 400.0,std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light_orange));
            scene.add(std::make_shared<RN::RTSphere>(sphere4));
            lights.add(std::make_shared<RN::RTSphere>(sphere4));

            RN::RTSphere sphere5(mesh.bbox().getCenter() + RN::vec3d(-1050, -500, 500), 400.0,std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light));
            scene.add(std::make_shared<RN::RTSphere>(sphere5));
            lights.add(std::make_shared<RN::RTSphere>(sphere5));

            RN::RTSphere sphere6(mesh.bbox().getCenter() + RN::vec3d(1050, -400, 500), 400.0,std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light));
            scene.add(std::make_shared<RN::RTSphere>(sphere6));
            lights.add(std::make_shared<RN::RTSphere>(sphere6));

            RN::vec3d origin = {0,0,0};

            double margin = -5;

            RN::RTQuad quadYZ( origin + vec3d(margin), RN::vec3d(0,box_size - margin * 2, 0), RN::vec3d(0,0,box_size - margin * 2), std::make_shared<RN::Materials::SolidColorMaterial>(mat_red));
            scene.add(std::make_shared<RN::RTQuad>(quadYZ));

            RN::RTQuad quadYZ2( origin + vec3d(margin) + vec3d(box_size - margin * 2, 0, 0), RN::vec3d( 0, box_size - margin * 2, 0), RN::vec3d( 0, 0, box_size- margin * 2), std::make_shared<RN::Materials::SolidColorMaterial>(mat_green));
            scene.add(std::make_shared<RN::RTQuad>(quadYZ2));

            RN::RTQuad quadYX(  origin + vec3d(margin), RN::vec3d(0, box_size - margin * 2, 0), RN::vec3d( box_size - margin * 2, 0, 0), std::make_shared<RN::Materials::SolidColorMaterial>(mat_check));
            //RN::RTQuad quadYX( origin + vec3d(margin), RN::vec3d(0, box_size - margin * 2, 0), RN::vec3d( box_size - margin * 2, 0, 0), std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light2));
            scene.add(std::make_shared<RN::RTQuad>(quadYX));

            RN::RTQuad quadXZ( origin + vec3d(margin), RN::vec3d(box_size - margin * 2,0, 0), RN::vec3d(0, 0, box_size - margin * 2), std::make_shared<RN::Materials::ReflectingMaterial>(mat_gold_reflecting));
            scene.add(std::make_shared<RN::RTQuad>(quadXZ));

            RN::RTQuad quadXZ2( origin + vec3d(margin) + vec3d(0, box_size - margin * 2,0), RN::vec3d(box_size - margin * 2,0, 0), RN::vec3d(0, 0, box_size - margin * 2), std::make_shared<RN::Materials::ReflectingMaterial>(mat_blue_reflecting));
            scene.add(std::make_shared<RN::RTQuad>(quadXZ2));

            RN::RTQuad quadLight( origin + vec3d(box_size/4+margin/2, box_size - margin - 0.1, box_size/4+margin/2), RN::vec3d(box_size/2-margin,0, 0), RN::vec3d(0, 0, box_size/2-margin), std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light));
            RN::Transforms::RTFlipFace flip(std::make_shared<RTQuad>(quadLight));
            scene.add(std::make_shared<RN::RTQuad>(quadLight));
            lights.add(std::make_shared<RN::Transforms::RTFlipFace>(flip));

//            RN::RTQuad quadYX( box_origin + vec3d(-30,-25,-15), RN::vec3d(0,50, 0), RN::vec3d(60, 0, 0), std::make_shared<RN::Materials::Diffusive>(mat_light2));
//            scene.add(std::make_shared<RN::RTQuad>(quadYX));

            RN::vec3d cam_center =  quadYX.bbox().getCenter() + vec3d(0,0,25);//mesh.bbox().getCenter() + vec3d(0,0,20);//RN::vec3d(box_origin.x + 280,box_origin.y + 330, box_origin.z + 710);

            camera.setCenter(cam_center);
            camera.lookAt(quadYX.bbox().getCenter());
        };
    };

} // RN

#endif // RAYON_CPP_CMAKE_SCHUM_BW_REFLECT_H