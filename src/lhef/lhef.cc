/* Copyright (c) 2014-2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#include "lhef/lhef.h"
#include <algorithm>
#include <functional>
#include <string>
#include <utility>
#include "particle.h"

namespace lhef {
std::string openingLine() { return "<LesHouchesEvents version=\"1.0\">"; }

std::string closingLine() { return "</LesHouchesEvents>"; }

Particles selectParticlesBy(
    std::function<bool(const Particles::value_type &)> pred, const Event &lhe) {
    const Particles ps{lhe.particles()};
    return selectBy(pred, ps);  // definition: see `particle.h`.
}

Particles initialStates(const Event &lhe) {
    const Particles ps{lhe.particles()};
    return initialStates(ps);
}

Particles intermediateStates(const Event &lhe) {
    const Particles ps{lhe.particles()};
    return intermediateStates(ps);
}

Particles finalStates(const Event &lhe) {
    const Particles ps{lhe.particles()};
    return finalStates(ps);
}

Particles collisionProducts(const Event &lhe) {
    const Particles ps{lhe.particles()};
    return collisionProducts(ps);
}

Particles particlesOf(const ParticleID &pid, const Event &lhe) {
    auto pred = [&pid](const Particle &p) { return p.is(pid); };
    return selectParticlesBy(pred, lhe);
}

ParticleLines particleLinesOf(const ParticleID &pid, const Event &lhe) {
    ParticleLines line;
    const auto entry{lhe.particleEntries()};
    for (const auto &e : entry) {
        const auto particle{e.second};
        if (particle.is(pid)) { line.push_back(std::move(e.first)); }
    }
    return line;
}

Particle mother(const Particle &p, const Event &lhe) {
    const int mo_line = p.mother().first;
    const auto entry{lhe.particleEntries()};
    if (entry.find(mo_line) == entry.end()) {  // mother particle not found
        return entry.at(1);
    } else {
        return entry.at(mo_line);
    }
}

Particle ancestor(const Particle &p, const Event &lhe) {
    const auto m = mother(p, lhe);
    return m.mother().first == 1 ? m : ancestor(m, lhe);
}

Particles daughters(int pline, const Event &lhe) {
    auto pred = [pline](const Particle &p) {
        return p.mother().first == pline;
    };
    return selectParticlesBy(pred, lhe);
}

bool isInMotherLines(const int pline, const Particle &p, const Event &lhe) {
    const int mo_line = p.mother().first;
    if (mo_line == 1) {
        return false;
    } else if (mo_line == pline) {
        return true;
    } else {
        const auto direct_mother{mother(p, lhe)};
        return isInMotherLines(pline, direct_mother, lhe);
    }
}

Particles finalDaughters(int pline, const Event &lhe) {
    Particles finalstates = finalStates(lhe);
    auto pos = std::remove_if(finalstates.begin(), finalstates.end(),
                              [pline, &lhe](const Particle &p) {
                                  return !isInMotherLines(pline, p, lhe);
                              });
    finalstates.erase(pos, finalstates.end());
    return finalstates;
}
}  // namespace lhef
