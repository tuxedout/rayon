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
 *      SCHUM.h - Scene
 *
 */

#ifndef RAYON_CPP_CMAKE_CORNEIL_BENCHY_H
#define RAYON_CPP_CMAKE_CORNEIL_BENCHY_H

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
#include "../RayOn/plus-d/Transforms/RTTranslate.h"
#include "../RayOn/plus-d/Transforms/RTScale.h"
#include "../RayOn/plus-d/Transforms/RTRotate.h"
#include "../RayOn/Materials/IsotropicMaterial.h"
#include "../RayOn/plus-d/Transforms/RTFlipFace.h"
#include "../RayOn/Materials/DebugMaterials.h"

namespace RN {

    class CorneilBenchyScene: public RTSceneCreator{
    public:
        void initScene(RN::RTScene &scene, RN::RTScene &lights, Camera &camera) override {

            /*
             * MATERIALS CREATION
             */

            /*
             * LIGHTS
             */
            RN::Materials::DiffusiveMaterial mat_light(RN::vec3d(2.0,2.0,2.0), RN::vec3d(1.0, 1.0, 1.0));
            RN::Materials::DiffusiveMaterial mat_light_orange(RN::vec3d(2.2,0.7,1.12), RN::vec3d(1.0, 0.50, 0.6));
            RN::Materials::DiffusiveMaterial mat_light_lines(std::make_shared<RN::Textures::LinesTexture>(RN::Textures::LinesTexture(vec3d(1.9), vec3d(0.0), 9, false)), std::make_shared<RN::Textures::LinesTexture>(RN::Textures::LinesTexture(vec3d(1.2, 0.6, 1.1), vec3d(0.0), 9,
                                                                                                                                                                                                                                                 true)));
            RN::Materials::DiffusiveMaterial mat_light2(
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(2.2, 1.2, 0.1),
                                                           vec3d(0.52, 0.52, 0.52), 10)),
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.0, 0.0, 0.0),
                                                           vec3d(0.0, 0.0, 0.0), 10)));

//            RN::Materials::DiffusiveMaterial mat_light2(
//                    std::make_shared<RN::Textures::CheckeredTexture>(
//                            RN::Textures::CheckeredTexture(vec3d(1.7, 0.83, 0.2),
//                                                           vec3d(1.2, 1.2, 1.2), 10)),
//                    std::make_shared<RN::Textures::CheckeredTexture>(
//                            RN::Textures::CheckeredTexture(vec3d(0.0, 0.0, 0.0),
//                                                           vec3d(0.0, 0.0, 0.0), 10)));

            RN::Materials::DiffusiveMaterial mat_light22(
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.4, 0.1, 0.15),
                                                           vec3d(0.4, 0.1, 0.13), 20)),
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(1.0, 0.4, 0.4),
                                                           vec3d(1.0, 1.0, 1.0), 20)));

            RN::Materials::DiffusiveMaterial mat_light3(RN::vec3d(3.610, 0.75, 2.62), RN::vec3d(1.0, 0.75, 0.74));

            /*
             * SOLID COLORS
             */
            RN::Materials::SolidColorMaterial mat_orange(RN::vec3d(0.2,0.8,0.72));
            RN::Materials::SolidColorMaterial mat_white(RN::vec3d(0.94,0.94,0.94));
            RN::Materials::SolidColorMaterial mat_black(RN::vec3d(0.1,0.1,0.1));
            RN::Materials::SolidColorMaterial mat_red(RN::vec3d(0.92,0.18,0.172));
            RN::Materials::SolidColorMaterial mat_green(RN::vec3d(0.1,0.98,0.172));
            RN::Materials::SolidColorMaterial mat_blue(RN::vec3d(0.1,0.8,0.92));
            RN::Materials::SolidColorMaterial mat_gray(RN::vec3d(0.5,0.5,0.5));
            RN::Materials::SolidColorMaterial mat_yellow(RN::vec3d(0.82,0.91,0.1));

            /*
             * LINES
             */
            RN::Materials::SolidColorMaterial lines(std::make_shared<RN::Textures::LinesTexture>(RN::Textures::LinesTexture(vec3d(1.0), vec3d(0.0), 50, false)));
            RN::Materials::SolidColorMaterial lines2(std::make_shared<RN::Textures::LinesTexture>(RN::Textures::LinesTexture(vec3d(0.0), vec3d(1.0), 100, true)));

            /*
             * CHECK
             */
            RN::Materials::SolidColorMaterial mat_check(std::make_shared<RN::Textures::CheckeredTexture>(RN::Textures::CheckeredTexture(vec3d(0), vec3d(1), 10)));
            //mat_check.roughness = 0.1;

            RN::Materials::SolidColorMaterial mat_check2(std::make_shared<RN::Textures::CheckeredTexture>(RN::Textures::CheckeredTexture(vec3d(0), vec3d(1), 20)));

            /*
             * REFR
             */
            RN::Materials::RefractingMaterial  mat_refr(RN::vec3d(0.410, 0.49, 0.92));
            mat_refr.reflectivity = 0.0;
            mat_refr.eta = 1.789;
            mat_refr.roughness = 0.0;

            RN::Materials::RefractingMaterial  mat_refr2(RN::vec3d(0.810, 0.79, 0.82));
            mat_refr2.reflectivity = 0.11;
            mat_refr2.eta = 1.1;
            mat_refr2.roughness = 0.0;

            RN::Materials::IridescentMaterial mat_irr;

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
            mat_silver_reflecting.reflectivity = 0.6;
            mat_silver_reflecting.roughness = 0.0;

            RN::Materials::ReflectingMaterial  mat_blue_reflecting(RN::vec3d(0.2, 0.85, 0.91));
            mat_blue_reflecting.reflectivity = 0.5;
            mat_blue_reflecting.roughness = 0.0;

            RN::Materials::ReflectingMaterial  mat_pink_reflecting(RN::vec3d(0.92, 0.2, 0.31));
            mat_pink_reflecting.reflectivity = 0.65;

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
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(lines));
            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(lines2));
//
//            mats.push_back(std::make_shared<RN::Materials::Diffusive>(mat_light));
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light2));
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light22));
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light3));
            mats.push_back(std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light_lines));
//            mats.push_back(std::make_shared<RN::Materials::Diffusive>(mat_light4));

            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_blue_reflecting));
            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_gold_reflecting));
            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_silver_reflecting));
            mats.push_back(std::make_shared<RN::Materials::ReflectingMaterial>(mat_pink_reflecting));
//            mats.push_back(std::make_shared<RN::Materials::SolidColorMaterial>(texture_material));

            std::string asspth = ASSETS_PATH;

            auto mesh = RN::RTMesh::loadFromOBJ(asspth + "goat-skull.obj", std::make_shared<RN::Materials::RefractingMaterial>(mat_refr2));

            Transforms::RTScale scale(std::make_shared<RTMesh>(mesh), {28});
            Transforms::RTRotate rotate_scale(std::make_shared<Transforms::RTScale>(scale), {1,0,0}, 0.25 * RN::PI_D);
            Transforms::RTTranslate translate_scale(std::make_shared<Transforms::RTRotate>(rotate_scale), {51,45,90});

            scene.add(std::make_shared<Transforms::RTTranslate>(translate_scale));

            RN::DiscreteInterval materials(mats);

            scene.background = RN::vec3d(0.0,0.0,0.0);

            RN::vec3d origin = {0,0,0};

            double box_size = 100;

            double margin = -5;

            RN::RTQuad quadLeft(origin + vec3d(margin), RN::vec3d(0, box_size - margin * 2, 0), RN::vec3d(0, 0, (box_size - margin * 2)),std::make_shared<RN::Materials::ReflectingMaterial>(mat_blue_reflecting));
            //scene.add(std::make_shared<RN::Transforms::RTFlipFace>(std::make_shared<RTQuad>(quadLeft)));
            //lights.add(std::make_shared<RN::RTQuad>(quadLeft));
            scene.add(std::make_shared<RN::RTQuad>(quadLeft));

            RN::RTQuad quadRight(origin + vec3d(margin) + vec3d(box_size - margin * 2, 0, 0), RN::vec3d(0, box_size - margin * 2, 0), RN::vec3d(0, 0, box_size - margin * 2),std::make_shared<RN::Materials::ReflectingMaterial>(mat_gold_reflecting));
            //scene.add(std::make_shared<RN::RTQuad>(quadRight));
            //lights.add(std::make_shared<RN::Transforms::RTFlipFace>(std::make_shared<RTQuad>(quadRight)));
            scene.add(std::make_shared<RN::Transforms::RTFlipFace>(std::make_shared<RTQuad>(quadRight)));

            RN::RTQuad quadBackground(origin + vec3d(margin) + vec3d (0,0,0), RN::vec3d(0, box_size - margin * 2, 0), RN::vec3d(box_size- margin * 2, 0, 0), std::make_shared<RN::Materials::ReflectingMaterial>(mat_silver_reflecting));
            //RN::RTQuad quadBackground( origin + vec3d(margin), RN::vec3d(0, box_size - margin * 2, 0), RN::vec3d( box_size - margin * 2, 0, 0), std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light2));
            //scene.add(std::make_shared<RN::RTQuad>(quadBackground));
//            lights.add(std::make_shared<RN::Transforms::RTFlipFace>(std::make_shared<RTQuad>(quadBackground)));
            scene.add(std::make_shared<RN::Transforms::RTFlipFace>(std::make_shared<RTQuad>(quadBackground)));

            RN::RTQuad quadBottom(origin + vec3d(margin), RN::vec3d(box_size - margin * 2, 0, 0), RN::vec3d(0, 0, box_size - margin * 2), std::make_shared<RN::Materials::SolidColorMaterial>(mat_check2));
            //scene.add(std::make_shared<RN::RTQuad>(quadBottom));
            //lights.add(std::make_shared<RN::Transforms::RTFlipFace>(std::make_shared<RTQuad>(quadBottom)));
            scene.add(std::make_shared<RN::Transforms::RTFlipFace>(std::make_shared<RTQuad>(quadBottom)));

            RN::RTQuad quadTop(origin + vec3d(margin) + vec3d(0, box_size - margin * 2, 0), RN::vec3d(box_size - margin * 2, 0, 0), RN::vec3d(0, 0, box_size - margin * 2), std::make_shared<RN::Materials::SolidColorMaterial>(RN::Color::White));
            RN::Transforms::RTFlipFace flip(std::make_shared<RTQuad>(quadTop));
            scene.add(std::make_shared<RN::RTQuad>(quadTop));
            //lights.add(std::make_shared<RN::RTQuad>(quadTop));
            //lights.add(std::make_shared<RN::Transforms::RTFlipFace>(flip));

            RN::RTQuad quadLight( origin + vec3d(box_size/4 + margin/2, 0, box_size/4 + margin/2)  + vec3d(0,box_size + 4.9999,0), RN::vec3d(box_size/2 - margin,0, 0), RN::vec3d(0, 0, box_size/2 - margin), std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light));
            scene.add(std::make_shared<RN::RTQuad>(quadLight));
            //RN::Transforms::RTFlipFace flipLight(std::make_shared<RTQuad>(quadLight));
            //scene.add(std::make_shared<RN::Transforms::RTFlipFace>(flipLight));
            //lights.add(std::make_shared<RN::Transforms::RTFlipFace>(flipLight));
            lights.add(std::make_shared<RTQuad>(quadLight));

            RN::vec3d cam_center = quadBackground.bbox().getCenter() + vec3d(0,0,box_size*2.7);

            RN::vec3d poi =  quadBackground.bbox().getCenter() + vec3d(0,0,box_size/2);

            int grid_size = 5;

            double grid_spacing = 20;


            RN::RTSphere sphere2(vec3d(32,30,35), 25, std::make_shared<RN::Materials::ReflectingMaterial>(mat_silver_reflecting));
            scene.add(std::make_shared<RN::RTSphere>(sphere2));

            RN::RTSphere sphere5(vec3d(80,16,69), 15, std::make_shared<RN::Materials::RefractingMaterial>(mat_refr));
            scene.add(std::make_shared<RN::RTSphere>(sphere5));

            RN::RTSphere box({0,0,0}, 10.0, std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light_orange));
            RN::Transforms::RTRotate rotate(std::make_shared<RN::RTSphere>(box), {0,1,0}, 0.25 * RN::PI_D);
            RN::Transforms::RTTranslate translate(std::make_shared<RN::Transforms::RTRotate>(rotate),origin + vec3d(30,12,80) + margin);

            //scene.add(std::make_shared<RN::Transforms::RTRotate>(rotate));
            scene.add(std::make_shared<RN::Transforms::RTTranslate>(translate));
            lights.add(std::make_shared<RN::Transforms::RTTranslate>(translate));

            camera.setCenter(cam_center);
            camera.lookAt(poi);
        };
    };

} // RN

#endif // RAYON_CPP_CMAKE_CORNEIL_BENCHY_H