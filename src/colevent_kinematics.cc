/* Copyright (c) 2014-2015, 2017, 2019 Chan Beom Park <cbpark@gmail.com> */

#include "colevent_kinematics.h"
#include <cmath>
#include <string>
#include "colevent_constants.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif  // HAVE_CONFIG_H
#ifdef HAVE_ROOT
#include "TLorentzVector.h"
#include "TVector3.h"
#endif  // HAVE_ROOT

using std::to_string;

namespace colevent {
std::string show(const FourMomentum &p) {
    std::string str = "FourMomentum {energy=" + to_string(p.energy());
    str += ",px=" + to_string(p.px());
    str += ",py=" + to_string(p.py());
    str += ",pz=" + to_string(p.pz());
    str += "}";
    return str;
}

#ifdef HAVE_ROOT

TVector3 mkTVector3(const Px &px, const Py &py, const Pz &pz) {
    return {px.value, py.value, pz.value};
}

double cosTheta(const Px &px, const Py &py, const Pz &pz) {
    TVector3 v{mkTVector3(px, py, pz)};
    return v.CosTheta();
}

double pseudoRapidity(const Px &px, const Py &py, const Pz &pz) {
    TVector3 v{mkTVector3(px, py, pz)};
    return v.PseudoRapidity();
}

TLorentzVector mkLorentzVector(const FourMomentum &p) {
    return {p.px(), p.py(), p.pz(), p.energy()};
}

double deltaPhi(const FourMomentum &p1, const FourMomentum &p2) {
    TLorentzVector v1 = mkLorentzVector(p1), v2 = mkLorentzVector(p2);
    return v1.DeltaPhi(v2);
}

double deltaR(const FourMomentum &p1, const FourMomentum &p2) {
    TLorentzVector v1 = mkLorentzVector(p1), v2 = mkLorentzVector(p2);
    return v1.DeltaR(v2);
}

#else  // HAVE_ROOT

double cosTheta(const Px &px, const Py &py, const Pz &pz) {
    double ptot = std::sqrt(px.value * px.value + py.value * py.value +
                            pz.value * pz.value);
    return ptot == 0 ? 1 : pz.value / ptot;
}

double pseudoRapidity(const Px &px, const Py &py, const Pz &pz) {
    double costh = cosTheta(px, py, pz);
    if (costh * costh < 1) {
        return -0.5 * std::log((1.0 - costh) / (1.0 + costh));
    }
    if (pz.value == 0) {
        return 10e10;
    } else {
        return -10e10;
    }
}

double mPi2Pi(double x) {
    const double TWOPI = 2.0 * PI;
    while (x >= PI) { x -= TWOPI; }
    while (x < -PI) { x += TWOPI; }
    return x;
}

double deltaPhi(const FourMomentum &p1, const FourMomentum &p2) {
    return mPi2Pi(p1.phi() - p2.phi());
}

double deltaR(const FourMomentum &p1, const FourMomentum &p2) {
    double deta = p1.eta() - p2.eta();
    double dphi = deltaPhi(p1, p2);
    return std::hypot(deta, dphi);
}

void CM22::p_init() {
    const double d = 2.0 * std::sqrt(s_);
    p_in_ = lambda12(s_, m_in1_sq_, m_in2_sq_) / d;
    p_out_ = lambda12(s_, m_out1_sq_, m_out2_sq_) / d;
}

FourMomentum CM22::p1() const {
    double e = (s_ + m_in1_sq_ - m_in2_sq_) / (2.0 * std::sqrt(s_));
    return {Energy(e), Px(0.0), Py(0.0), Pz(p_in_)};
}

FourMomentum CM22::p2() const {
    double e = (s_ - m_in1_sq_ + m_in2_sq_) / (2.0 * std::sqrt(s_));
    return {Energy(e), Px(0.0), Py(0.0), Pz(-p_in_)};
}

FourMomentum CM22::k1() const {
    double e = (s_ + m_out1_sq_ - m_out2_sq_) / (2.0 * std::sqrt(s_));
    return {Energy(e), Px(p_out_ * sinth_ * std::cos(phi_)),
            Py(p_out_ * sinth_ * std::sin(phi_)), Pz(p_out_ * costh_)};
}

FourMomentum CM22::k2() const {
    double e = (s_ - m_out1_sq_ + m_out2_sq_) / (2.0 * std::sqrt(s_));
    return {Energy(e), Px(-p_out_ * sinth_ * std::cos(phi_)),
            Py(-p_out_ * sinth_ * std::sin(phi_)), Pz(-p_out_ * costh_)};
}

#endif  // HAVE_ROOT
}  // namespace colevent
