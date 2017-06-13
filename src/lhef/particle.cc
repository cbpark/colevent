/* Copyright (c) 2014-2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#include "lhef/particle.h"
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include "colevent_kinematics.h"
#include "lester_mt2_bisect.h"
#if DEBUG
#include <iostream>
#endif

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

double mT2(const Particle &p1, const Particle &p2, const double kx,
           const double ky, const double m_inv1, const double m_inv2,
           bool print_copyright) {
    FourMomentum v1 = momentum(p1), v2 = momentum(p2);
    if (!print_copyright) {
        asymm_mt2_lester_bisect::disableCopyrightMessage();
    }
    return asymm_mt2_lester_bisect::get_mT2(v1.mass(), v1.px(), v1.py(),
                                            v2.mass(), v2.px(), v2.py(), kx, ky,
                                            m_inv1, m_inv2);
}

string show(const Particles &ps) {
    if (ps.empty()) { return "[]"; }

    string ps_str = "[";
    for (const auto &p : ps) { ps_str += show(p) + ","; }
    ps_str.pop_back();
    ps_str += "]";
    return ps_str;
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

double invariantMass(const std::initializer_list<Particles> &pss) {
    Particles ps;
    ps.reserve(pss.size());
    std::transform(pss.begin(), pss.end(), std::back_inserter(ps),
                   [](const Particles &ps_) { return sum(ps_); });
#if DEBUG
    std::cout << "invariantMass: " << show(ps) << '\n';
#endif
    return invariantMass(ps);
}
}  // namespace lhef
