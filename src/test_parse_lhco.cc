/* Copyright (c) 2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#include <fstream>
#include <iostream>
#include "kinematics.h"
#include "lhco/lhco.h"

double invMass(const lhco::Event &lhco) {
    double invmass = 0.0;
    if (numMuon(lhco) == 1 && numBjet(lhco) == 1) {
        lhco::Visibles ps;
        ps.push_back(lhco.muon().front());
        ps.push_back(lhco.bjet().front());
        invmass = invariantMass(ps);
    }
    return invmass;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: test_parse input\n"
                  << "    - input: Input file in "
                  << "LHC Olympics format\n";
        return 1;
    }

    std::ifstream filename(argv[1]);
    if (!filename) {
        std::cerr << "-- Cannot open input file \"" << argv[1] << "\".\n";
        return 1;
    } else {
        std::cout << "-- Reading \"" << argv[1] << "\" ...\n";
    }

    lhco::Event lhco = lhco::parseEvent(&filename);
    colevent::Pt ptcut(30.0);
    colevent::Eta etacut(2.0);
    int num_eve = 0;
    for (; !lhco.empty(); lhco = lhco::parseEvent(&filename)) {
        ++num_eve;
        std::cout << "-- Event number: " << num_eve << '\n'
                  << lhco.show() << '\n';
        std::cout << "---- Missing ET = " << missingET(lhco) << '\n';
        std::cout << "---- Before cut:\n------ ";
        std::cout << "# of photons: " << numPhoton(lhco) << ", "
                  << "electrons: " << numElectron(lhco) << ", "
                  << "muons: " << numMuon(lhco) << ", "
                  << "taus: " << numTau(lhco) << ", "
                  << "all jets: " << numAllJet(lhco) << ", "
                  << "bjets: " << numBjet(lhco) << '\n'
                  << "------ Invariant mass of muon and b-jet = "
                  << invMass(lhco) << '\n';
        std::cout << "---- After cut:\n------ ";
        std::cout << "# of photons: " << numPhoton(ptcut, etacut, lhco) << ", "
                  << "electrons: " << numElectron(ptcut, etacut, lhco) << ", "
                  << "muons: " << numMuon(ptcut, etacut, lhco) << ", "
                  << "taus: " << numTau(ptcut, etacut, lhco) << ", "
                  << "all jets: " << numAllJet(ptcut, etacut, lhco) << ", "
                  << "bjets: " << numBjet(ptcut, etacut, lhco) << '\n';
    }

    std::cout << "-- " << num_eve << " events parsed.\n";
    filename.close();
}
