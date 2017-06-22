/* Copyright (c) 2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#ifndef COLEVENT_SRC_LHCO_LHCO_H_
#define COLEVENT_SRC_LHCO_LHCO_H_

#include <string>
#include "lhco/event.h"
#include "lhco/object.h"
#include "lhco/parser.h"
#include "lhco/particle.h"

namespace lhco {
std::string openingLine() {
    std::string line = "  #  ";
    line += "typ      eta    phi      pt    jmas  ntrk  btag   had/em";
    line += "  dum1  dum2";
    return line;
}

int numPhoton(const Event &ev);

int numPhoton(const colevent::Pt &pt, const colevent::Eta &eta,
              const Event &ev);

int numElectron(const Event &ev);

int numElectron(const colevent::Pt &pt, const colevent::Eta &eta,
                const Event &ev);

int numMuon(const Event &ev);

int numMuon(const colevent::Pt &pt, const colevent::Eta &eta, const Event &ev);

int numLeptonIso(const Event &ev);

int numTau(const Event &ev);

int numTau(const colevent::Pt &pt, const colevent::Eta &eta, const Event &ev);

int numNormalJet(const Event &ev);

int numNormalJet(const colevent::Pt &pt, const colevent::Eta &eta,
                 const Event &ev);

int numBjet(const Event &ev);

int numBjet(const colevent::Pt &pt, const colevent::Eta &eta, const Event &ev);

int numAllJet(const Event &ev);

int numAllJet(const colevent::Pt &pt, const colevent::Eta &eta,
              const Event &ev);

double missingET(const Event &ev);

double invariantMass(const Visibles &ps);
}  // namespace lhco

#endif  // COLEVENT_SRC_LHCO_LHCO_H_
