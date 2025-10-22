/*
 *
 *       Created by tux on 03.06.2024.
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
 *      main-pdf-test.cpp
 *
 */
#include <algorithm>
#include <thread>
#include <map>

#include "RayOn/Vec3.h"
#include "RayOn/CMS/PDF.h"


void test_combined_pdf() {
    std::shared_ptr<RN::SpherePDF> p0 = std::make_shared<RN::SpherePDF>();
    std::shared_ptr<RN::CosinePDF> p1 = std::make_shared<RN::CosinePDF>(RN::vec3d(0, 0, 1));

    RN::CombinedPDF combined_pdf(p0, p1);

    const int sample_count = 100;
    std::map<RN::vec3d, int> direction_count;
    double pdf_value_sum = 0.0;

    for (int i = 0; i < sample_count; ++i) {
        RN::vec3d direction = p1->generate();
        pdf_value_sum += p1->value(direction);

        // Simple discretization for counting purposes
        RN::vec3d discrete_dir = RN::vec3d(
                round(direction.x * 10) / 10,
                round(direction.y * 10) / 10,
                round(direction.z * 10) / 10
        );

        direction_count[discrete_dir]++;
    }

    // Output results
    std::cout << "Direction distribution:" << std::endl;
    for (const auto& entry : direction_count) {
        std::cout << "Direction: (" << entry.first.x << ", " << entry.first.y << ", " << entry.first.z << ") Count: " << entry.second << std::endl;
    }

    std::cout << "Average PDF value: " << (pdf_value_sum / sample_count) << std::endl;
    //std::cout << "First/Second : " << combined_pdf.first << "/" << combined_pdf.second << std::endl;
}

int main() {
    test_combined_pdf();
    return 0;
}
