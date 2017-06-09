/* Copyright (c) 2014-2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include "lhef/lhef.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: test_parse input\n"
                  << "    - input: Input file in "
                  << "Les Houches Event File format\n";
        return 1;
    }

    std::ifstream filename(argv[1]);
    if (!filename) {
        std::cerr << "-- Cannot open input file \"" << argv[1] << "\".\n";
        return 1;
    } else {
        std::cout << "-- Reading \"" << argv[1] << "\" ...\n";
    }

    lhef::Event lhe = lhef::parseEvent(&filename);
    lhef::Particles initstates;
    lhef::Particles finalstates;
    lhef::ParticleID is_lepton;
    std::merge(lhef::Electron.cbegin(), lhef::Electron.cend(),
               lhef::Muon.cbegin(), lhef::Muon.cend(),
               std::back_inserter(is_lepton));
    lhef::Particles leptons;
    lhef::Particle lep_anc;
    lhef::ParticleLines toplines;
    lhef::Particles daughters_of_top;
    int num_eve = 0;
    for (; !lhe.empty(); lhe = lhef::parseEvent(&filename)) {
        ++num_eve;
        std::cout << "-- Event number: " << num_eve << '\n'
                  << lhef::show(lhe) << '\n';
        initstates = lhef::initialStates(lhe);
        std::cout << "---- Initial-state particles:\n"
                  << lhef::show(initstates) << '\n';
        finalstates = lhef::finalStates(lhe);
        std::cout << "---- Final-state particles:\n"
                  << lhef::show(finalstates) << '\n';
        leptons = lhef::particlesOf(is_lepton, lhe);
        std::cout << "---- Leptons:\n" << lhef::show(leptons) << '\n';
        lep_anc = lhef::ancestor(leptons.front(), lhe);
        std::cout << "---- Ancestor of one lepton:\n"
                  << lhef::show(lep_anc) << '\n';
        toplines = lhef::particleLinesOf(lhef::Top, lhe);
        std::cout << "---- Lines of top quarks:\n";
        std::copy(toplines.cbegin(), toplines.cend(),
                  std::ostream_iterator<int>(std::cout, " "));
        std::cout << '\n';
        daughters_of_top = lhef::finalDaughters(toplines.front(), lhe);
        std::cout << "---- Daughters of one top quark:\n"
                  << lhef::show(daughters_of_top) << '\n';
    }

    std::cout << "-- " << num_eve << " events parsed.\n";
    filename.close();
}
