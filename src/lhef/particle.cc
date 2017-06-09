/* Copyright (c) 2014-2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#include "lhef/particle.h"
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include "kinematics.h"

using std::string;
using std::to_string;
using colevent::FourMomentum;
using colevent::Energy;
using colevent::Px;
using colevent::Py;
using colevent::Pz;

namespace lhef {
Particle &Particle::operator+=(const Particle &rhs) {
    Energy e(pup_[3] + rhs.pup_[3]);
    Px px(pup_[0] + rhs.pup_[0]);
    Py py(pup_[1] + rhs.pup_[1]);
    Pz pz(pup_[2] + rhs.pup_[2]);
    *this = Particle{e, px, py, pz};
    return *this;
}

Particle &Particle::operator-=(const Particle &rhs) {
    Energy e(pup_[3] - rhs.pup_[3]);
    Px px(pup_[0] - rhs.pup_[0]);
    Py py(pup_[1] - rhs.pup_[1]);
    Pz pz(pup_[2] - rhs.pup_[2]);
    *this = Particle{e, px, py, pz};
    return *this;
}

std::istream &operator>>(std::istream &is, Particle &p) {
    is >> p.idup_ >> p.istup_ >> p.mothup_.first >> p.mothup_.second >>
        p.icolup_.first >> p.icolup_.second >> p.pup_[0] >> p.pup_[1] >>
        p.pup_[2] >> p.pup_[3] >> p.pup_[4] >> p.vtimup_ >> p.spinup_;
    return is;
}

std::ostream &operator<<(std::ostream &os, const Particle &p) {
    os << std::setw(9) << p.idup_ << std::setw(5) << p.istup_ << std::setw(5)
       << p.mothup_.first << std::setw(5) << p.mothup_.second << std::setw(5)
       << p.icolup_.first << std::setw(5) << p.icolup_.second;

    auto ss = os.precision();

    os << std::setprecision(11) << std::scientific << std::uppercase;
    for (const auto &momentum : p.pup_) { os << std::setw(19) << momentum; }

    os << std::fixed << std::setprecision(0) << std::setw(2) << p.vtimup_ << '.'
       << std::setw(3) << p.spinup_ << '.';
    os.precision(ss);

    return os;
}

string show(const Particle &p) {
    string p_str = "Particle {idup=" + to_string(p.idup_) +
                   ",istup=" + to_string(p.istup_) + ",mothup=(" +
                   to_string(p.mothup_.first) + "," +
                   to_string(p.mothup_.second) + ")" + ",icolup=(" +
                   to_string(p.icolup_.first) + "," +
                   to_string(p.icolup_.second) + ")" + ",pup=(";

    for (const auto &momentum : p.pup_) { p_str += to_string(momentum) + ","; }
    p_str.pop_back();

    p_str = p_str + ")" + ",vtimup=" + to_string(p.vtimup_) +
            ",spinup=" + to_string(p.spinup_) + "}";

    return p_str;
}

double deltaPhi(const Particle &p1, const Particle &p2) {
    FourMomentum v1 = momentum(p1), v2 = momentum(p2);
    return deltaPhi(v1, v2);
}

double deltaR(const Particle &p1, const Particle &p2) {
    FourMomentum v1 = momentum(p1), v2 = momentum(p2);
    return deltaR(v1, v2);
}

string show(const Particles &ps) {
    string ps_str = "[";
    for (const auto &p : ps) { ps_str += show(p) + ","; }
    ps_str.pop_back();
    ps_str += "]";
    return ps_str;
}

Particle sum(const Particles &ps) {
    Particle sum = std::accumulate(
        ps.cbegin(), ps.cend(), Particle{Energy(0), Px(0), Py(0), Pz(0)},
        [](const Particles::value_type &p1, const Particles::value_type &p2) {
            return p1 + p2;
        });
    return sum;
}

Particles selectByID(const ParticleID &pid, const Particles &ps) {
    auto pred = [&pid](const Particles::value_type &p) { return p.is(pid); };
    return selectBy(pred, ps);
}

Particles excludeByID(const ParticleID &pid, const Particles &ps) {
    auto pred = [&pid](const Particles::value_type &p) { return !p.is(pid); };
    return selectBy(pred, ps);
}

Particles initialStates(const Particles &ps) {
    auto pred = [](const Particles::value_type &p) { return p.status() == -1; };
    return selectBy(pred, ps);
}

Particles intermediateStates(const Particles &ps) {
    auto pred = [](const Particles::value_type &p) { return p.status() == 2; };
    return selectBy(pred, ps);
}

Particles finalStates(const Particles &ps) {
    auto pred = [](const Particles::value_type &p) { return p.status() == 1; };
    return selectBy(pred, ps);
}

Particles collisionProducts(const Particles &ps) {
    auto pred = [](const Particles::value_type &p) {
        int mother_line = p.mother().first;
        return mother_line == 1 || mother_line == 2;
    };
    return selectBy(pred, ps);
}

FourMomentum pSum(const Particles &ps) {
    Particle p{sum(ps)};
    return momentum(p);
}

double invariantMass(const Particles &ps) {
    FourMomentum v{pSum(ps)};
    return v.mass();
}

double sqrtSOfInits(const Particles &ps) {
    const Particles init{initialStates(ps)};
    return invariantMass(init);
}

double transverseMomentum(const Particles &ps) {
    FourMomentum v{pSum(ps)};
    return v.pt();
}
}  // namespace lhef
