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

#ifndef RAYON_CPP_CMAKE_OPTICAL_TEST_H
#define RAYON_CPP_CMAKE_OPTICAL_TEST_H

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
#include "../RayOn/Materials/DispersiveMaterial.h"

namespace RN {

    class OpticalTest: public RTSceneCreator{
    public:
        void initScene(RN::RTScene &scene, RN::RTScene &lights, Camera &camera) override {
            RN::Materials::RefractingMaterial  mat_refr(RN::vec3d(1.0, 1.0, 1.0));
            mat_refr.reflectivity = 0.50;
            mat_refr.eta = 1.769;
            mat_refr.roughness = 0.0;

            RN::Materials::RefractingMaterial  mat_refr2(RN::vec3d(0.92, 0.972, 1.0));
            mat_refr2.reflectivity = 0.85;
            mat_refr2.eta = 1.3;
            mat_refr2.roughness = 0.0;

            RN::Materials::DispersiveMaterial dispersive;

            RN::Materials::DiffusiveMaterial mat_light22(
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.1, 0.1, 0.8),
                                                           vec3d(0.9, 0.9, 0.2), 20)),
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(4.1, 4.0, 7.1),
                                                           vec3d(7.2, 7.1, 4.3), 20)));

            RN::Materials::DiffusiveMaterial mat_light222(
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.1, 0.1, 0.8),
                                                           vec3d(0.9, 0.9, 0.2), 20)),
                    std::make_shared<RN::Textures::CheckeredTexture>(
                            RN::Textures::CheckeredTexture(vec3d(0.1, 0.2, 0.1),
                                                           vec3d(2.2, 2.1, 0.3), 20)));


            RN::Materials::SolidColorMaterial mat_white(RN::vec3d(0.94,0.94,0.94));
            RN::Materials::SolidColorMaterial mat_black(RN::vec3d(0.1,0.1,0.1));

            RN::Materials::SolidColorMaterial mat_check(std::make_shared<RN::Textures::CheckeredTexture>(RN::Textures::CheckeredTexture(vec3d(0), vec3d(1), 60)));
            mat_check.roughness = 0.1;

            RN::Materials::SolidColorMaterial mat_check2(std::make_shared<RN::Textures::CheckeredTexture>(RN::Textures::CheckeredTexture(vec3d(0), vec3d(1), 30)));
            mat_check2.roughness = 0.0;


            scene.background = RN::vec3d(0.0,0.0,0.0);

            RN::vec3d box_origin = RN::vec3d(-45,-45,-45);



            RN::RTQuad quadYZ( box_origin, RN::vec3d(0,50, 0), RN::vec3d(0,0,50), std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light222));
            scene.add(std::make_shared<RN::RTQuad>(quadYZ));
            lights.add(std::make_shared<RN::RTQuad>(quadYZ));

            RN::RTQuad quadYX( box_origin, RN::vec3d(0,50, 0), RN::vec3d(50, 0, 0), std::make_shared<RN::Materials::SolidColorMaterial>(mat_check2));
            scene.add(std::make_shared<RN::RTQuad>(quadYX));

            RN::RTQuad quadXZ( box_origin, RN::vec3d(50,0, 0), RN::vec3d(0, 0, 50), std::make_shared<RN::Materials::SolidColorMaterial>(mat_white));
            scene.add(std::make_shared<RN::RTQuad>(quadXZ));

            RN::RTSphere sphere2(box_origin + RN::vec3d(25, 1070, 25), 700.0,std::make_shared<RN::Materials::DiffusiveMaterial>(mat_light22));
            scene.add(std::make_shared<RN::RTSphere>(sphere2));
            lights.add(std::make_shared<RN::RTSphere>(sphere2));
//
//            RN::RTSphere sphere(box_origin + vec3d(25, 16, 25), 10, std::make_shared<RN::Materials::Refracting>(mat_refr));
//            scene.add(std::make_shared<RN::RTSphere>(sphere));

//            RN::RTSphere sphere3(box_origin + vec3d(25, 50, 25), 15, std::make_shared<RN::Materials::Refracting>(mat_refr));
//            scene.add(std::make_shared<RN::RTSphere>(sphere3));

//            RN::RTSphere box(box_origin + vec3d(25, 35, 25), 10, std::make_shared<RN::Materials::Refracting>(mat_refr2));
//            scene.add(std::make_shared<RN::RTSphere>(box));
//
//            RN::RTBox box2(box_origin + vec3d(25, 18, 25), vec3d(12), std::make_shared<RN::Materials::Reflecting>(mat_refl));
//            scene.add(std::make_shared<RN::RTBox>(box2));

//            RN::RTEllipsoid elli(box_origin + vec3d(25, 24, 25), {12,5,12}, std::make_shared<RN::Materials::DispersiveMaterial>(dispersive));
//            scene.add(std::make_shared<RN::RTEllipsoid>(elli));

//            RN::RTSphere box(box_origin + vec3d(25, 35, 25), 10, std::make_shared<RN::Materials::Refracting>(mat_refr2));
//            scene.add(std::make_shared<RN::RTSphere>(box));

            RN::RTEllipsoid elli2(box_origin + vec3d(25, 60, 25), {10,1,10}, std::make_shared<RN::Materials::RefractingMaterial>(mat_refr));
            scene.add(std::make_shared<RN::RTEllipsoid>(elli2));

            RN::vec3d cam_center = RN::vec3d(box_origin.x + 100,box_origin.y + 100, box_origin.z + 100);

            camera.setCenter(cam_center);
            camera.lookAt(box_origin);
        };
    };

} // RN

#endif //RAYON_CPP_CMAKE_OPTICAL_TEST_H
