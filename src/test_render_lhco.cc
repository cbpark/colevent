/* Copyright (c) 2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#include <iostream>
#include "lhco/lhco.h"

int main() {
    lhco::Header header{4, 3587};

    lhco::Objects objs;
    objs.push_back(
        lhco::Object(2, 0.271, 3.641, 270.63, 0.11, -1.0, 3.0, 5.01));
    objs.push_back(
        lhco::Object(4, -0.136, 0.290, 440.15, 79.66, 19.0, 0.0, 1.23));
    objs.push_back(
        lhco::Object(4, 0.671, 3.309, 91.96, 23.46, 14.0, 2.0, 2.36));
    objs.push_back(lhco::Object(4, 0.422, 3.019, 99.76, 3.97, 12.0, 2.0, 1.87));
    objs.push_back(lhco::Object(4, -1.419, 1.018, 23.75, 1.79, 5.0, 0.0, 4.17));
    objs.push_back(lhco::Object(6, 0.000, 3.688, 20.20, 0.00, 0.0, 0.0, 0.00));

    lhco::RawEvent ev{header, objs};
    std::cout << "-- Rendering LHCO Event ...\n";
    std::cout << ev << '\n';
    std::cout << "-- Rendering done.\n";
}
