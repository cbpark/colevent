/* Copyright (c) 2014-2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#ifndef COLEVENT_SRC_COLEVENT_KINEMATICS_H_
#define COLEVENT_SRC_COLEVENT_KINEMATICS_H_

#include <array>
#include <cmath>
#include <string>
#include "colevent_constants.h"

namespace colevent {
struct Px {
    double value;
    Px() : value(0.0) {}
    explicit Px(double v) : value(v) {}
    Px operator-() const { return Px{-value}; }
};

struct Py {
    double value;
    Py() : value(0.0) {}
    explicit Py(double v) : value(v) {}
    Py operator-() const { return Py{-value}; }
};

struct Pz {
    double value;
    Pz() : value(0.0) {}
    explicit Pz(double v) : value(v) {}
    Pz operator-() const { return Pz{-value}; }
};

struct Energy {
    double value;
    Energy() : value(0.0) {}
    explicit Energy(double v) : value(v) {}
    Energy operator-() const { return Energy{-value}; }
};

double cosTheta(const Px &px, const Py &py, const Pz &pz);

double pseudoRapidity(const Px &px, const Py &py, const Pz &pz);

struct Pt {
    double value;
    Pt() : value(0.0) {}
    explicit Pt(double v) : value(v) {}
    Pt(const Px &px, const Py &py) { value = std::hypot(px.value, py.value); }
};

struct Eta {
    double value;
    Eta() : value(0.0) {}
    explicit Eta(double v) : value(v) {}
    Eta(const Px &px, const Py &py, const Pz &pz)
        : value(pseudoRapidity(px, py, pz)) {}
};

struct Phi {
    double value;
    Phi() : value(0.0) {}
    explicit Phi(double v) : value(v) {}
    Phi(const Px &px, const Py &py) : value(std::atan2(py.value, px.value)) {}
    Phi operator-() const { return Phi{-value}; }
};

struct Mass {
    double value;
    Mass() : value(0.0) {}
    explicit Mass(double v) : value(v) {}
};

class FourMomentum {
private:
    Energy e_;
    Px px_;
    Py py_;
    Pz pz_;

public:
    FourMomentum() : e_(0), px_(0), py_(0), pz_(0) {}
    FourMomentum(const Energy &e, const Px &px, const Py &py, const Pz &pz)
        : e_(e), px_(px), py_(py), pz_(pz) {}

    double energy() const { return e_.value; }
    double px() const { return px_.value; }
    double py() const { return py_.value; }
    double pz() const { return pz_.value; }

    double pt() const { return std::hypot(px(), py()); }
    double phi() const {
        return px_.value == 0 && py_.value == 0 ? 0 : std::atan2(py_.value,
                                                                 px_.value);
    }
    double eta() const { return pseudoRapidity(px_, py_, pz_); }
    double mass2() const {
        return e_.value * e_.value - px_.value * px_.value -
               py_.value * py_.value - pz_.value * pz_.value;
    }
    double mass() const {
        double m2 = mass2();
        return m2 < 0 ? -std::sqrt(-m2) : std::sqrt(m2);
    }

    FourMomentum operator-() const { return {-e_, -px_, -py_, -pz_}; }

    FourMomentum &operator+=(const FourMomentum &rhs) {
        Energy e(e_.value + rhs.energy());
        Px px(px_.value + rhs.px());
        Py py(py_.value + rhs.py());
        Pz pz(pz_.value + rhs.pz());
        *this = FourMomentum{e, px, py, pz};
        return *this;
    }

    FourMomentum &operator-=(const FourMomentum &rhs) {
        Energy e(e_.value - rhs.energy());
        Px px(px_.value - rhs.px());
        Py py(py_.value - rhs.py());
        Pz pz(pz_.value - rhs.pz());
        *this = FourMomentum{e, px, py, pz};
        return *this;
    }

    friend FourMomentum operator+(FourMomentum lhs, const FourMomentum &rhs) {
        lhs += rhs;
        return lhs;
    }

    friend FourMomentum operator-(FourMomentum lhs, const FourMomentum &rhs) {
        lhs -= rhs;
        return lhs;
    }
};

std::string show(const FourMomentum &p);

double deltaPhi(const FourMomentum &p1, const FourMomentum &p2);

double deltaR(const FourMomentum &p1, const FourMomentum &p2);

inline double lambda12(const double x, const double y, const double z) {
    double lambda = x * x + y * y + z * z - 2 * x * y - 2 * y * z - 2 * z * x;
    return lambda < 0 ? std::sqrt(-lambda) : std::sqrt(lambda);
}

class CM22 {
private:
    double m_in1_sq_ = 0, m_in2_sq_ = 0;
    double m_out1_sq_ = 0, m_out2_sq_ = 0;
    double s_ = 0;
    double costh_ = -2, sinth_ = -2;
    double phi_ = 0;
    double p_in_ = 0, p_out_ = 0;

public:
    CM22(const std::array<double, 4> &m, const double s, const double r1,
         const double r2)
        : m_in1_sq_(m[0] * m[0]),
          m_in2_sq_(m[1] * m[1]),
          m_out1_sq_(m[2] * m[2]),
          m_out2_sq_(m[3] * m[3]),
          s_(s),
          costh_(costh(r1)),
          sinth_(std::sqrt(1.0 - costh_ * costh_)),
          phi_(TWOPI * r2) {
        p_init();
    }

    double costh() const { return costh_; }
    double phi() const { return phi_; }

    double s() const { return s_; }
    double t() const { return (p1() - k1()).mass2(); }
    double u() const { return (p1() - k2()).mass2(); }

    FourMomentum p1() const;
    FourMomentum p2() const;
    FourMomentum k1() const;
    FourMomentum k2() const;

private:
    double costh(const double r) { return -1.0 + 2.0 * r; }
    void p_init();
};
}  // namespace colevent

#endif  // COLEVENT_SRC_COLEVENT_KINEMATICS_H_
