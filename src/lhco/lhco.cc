/* Copyright (c) 2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#include "lhco/lhco.h"
#include <cmath>
#include <vector>
#include "kinematics.h"

using colevent::Energy;
using colevent::Px;
using colevent::Py;
using colevent::Pz;
using colevent::Pt;
using colevent::Eta;

namespace lhco {
template <typename T>
int numOfParticles(const Pt &pt, const Eta &eta, const std::vector<T> &ps) {
    int count = 0;
    for (const auto &p : ps) {
        if (p.pt() > pt.value && std::abs(p.eta()) < eta.value) { ++count; }
    }
    return count;
}

int numPhoton(const Event &ev) { return ev.photon().size(); }

int numPhoton(const Pt &pt, const Eta &eta, const Event &ev) {
    return numOfParticles(pt, eta, ev.photon());
}

int numElectron(const Event &ev) { return ev.electron().size(); }

int numElectron(const Pt &pt, const Eta &eta, const Event &ev) {
    return numOfParticles(pt, eta, ev.electron());
}

int numMuon(const Event &ev) { return ev.muon().size(); }

int numMuon(const Pt &pt, const Eta &eta, const Event &ev) {
    return numOfParticles(pt, eta, ev.muon());
}

int numTau(const Event &ev) { return ev.tau().size(); }

int numTau(const Pt &pt, const Eta &eta, const Event &ev) {
    return numOfParticles(pt, eta, ev.tau());
}

int numNormalJet(const Event &ev) { return ev.jet().size(); }

int numNormalJet(const Pt &pt, const Eta &eta, const Event &ev) {
    return numOfParticles(pt, eta, ev.jet());
}

int numBjet(const Event &ev) { return ev.bjet().size(); }

int numBjet(const Pt &pt, const Eta &eta, const Event &ev) {
    return numOfParticles(pt, eta, ev.bjet());
}

int numAllJet(const Event &ev) { return numNormalJet(ev) + numBjet(ev); }

int numAllJet(const Pt &pt, const Eta &eta, const Event &ev) {
    return numNormalJet(pt, eta, ev) + numBjet(pt, eta, ev);
}

double missingET(const Event &ev) { return ev.met().pt(); }

double invariantMass(const Visibles &ps) {
    Visible vis{Energy(0.0), Px(0.0), Py(0.0), Pz(0.0)};
    for (const auto &p : ps) { vis += p; }
    return vis.mass();
}
}  // namespace lhco
