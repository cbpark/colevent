/* Copyright (c) 2014-2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#ifndef COLEVENT_SRC_LHEF_LHEF_H_
#define COLEVENT_SRC_LHEF_LHEF_H_

#include <string>
#include "lhef/event.h"
#include "lhef/parser.h"
#include "lhef/particle.h"
#include "lhef/pid.h"

namespace lhef {
using ParticleLines = std::vector<int>;

std::string openingLine();

std::string closingLine();

Particles initialStates(const Event &lhe);

Particles intermediateStates(const Event &lhe);

Particles finalStates(const Event &lhe);

Particles collisionProducts(const Event &lhe);

Particles particlesOf(const ParticleID &pid, const Event &lhe);

ParticleLines particleLinesOf(const ParticleID &pid, const Event &lhe);

Particle mother(const Particle &p, const Event &lhe);

Particle ancestor(const Particle &p, const Event &lhe);

Particles daughters(int pline, const Event &lhe);

Particles finalDaughters(int pline, const Event &lhe);
}  // namespace lhef

#endif  // COLEVENT_SRC_LHEF_LHEF_H_
