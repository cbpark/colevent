/* Copyright (c) 2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#ifndef COLEVENT_SRC_LHCO_PARTICLE_H_
#define COLEVENT_SRC_LHCO_PARTICLE_H_

#include <cmath>
#include <string>
#include <vector>
#include "kinematics.h"

namespace lhco {
class Particle {
private:
    double pt_ = 0.0;
    double phi_ = 0.0;
    double px_ = 0.0;
    double py_ = 0.0;

public:
    Particle() {}
    Particle(const colevent::Pt &pt, const colevent::Phi &phi)
        : pt_{pt.value},
          phi_{phi.value},
          px_{pt.value * std::cos(phi.value)},
          py_{pt.value * std::sin(phi.value)} {}
    virtual ~Particle() {}

    double pt() const { return pt_; }
    double phi() const { return phi_; }
    double px() const { return px_; }
    double py() const { return py_; }

    virtual std::string show() const = 0;
};

struct Met : public Particle {
    Met() {}
    Met(const colevent::Pt &pt, const colevent::Phi &phi) : Particle(pt, phi) {}
    ~Met() {}

    std::string show() const;
};

class Visible : public Particle {
public:
    enum class Charge { Positive, Negative, Neutral };

private:
    double eta_ = 0.0;
    double m_ = 0.0;
    double pz_ = 0.0;
    double e_ = 0.0;
    Charge q_ = Charge::Neutral;

protected:
    void set_energy(double m) {
        if (m > 0.0) {
            e_ = std::sqrt(pt() * pt() + pz_ * pz_ + m * m);
        } else {
            e_ = std::sqrt(pt() * pt() + pz_ * pz_);
        }
    }
    void set_charge(int q) {
        if (q > 0) {
            q_ = Charge::Positive;
        } else if (q < 0) {
            q_ = Charge::Negative;
        } else {
            q_ = Charge::Neutral;
        }
    }

public:
    Visible() {}
    Visible(const colevent::Pt &pt, const colevent::Eta &eta,
            const colevent::Phi &phi, const colevent::Mass &m)
        : Particle{pt, phi},
          eta_{eta.value},
          m_{m.value},
          pz_{pt.value * std::sinh(eta.value)} {
        set_energy(m.value);
    }
    Visible(const colevent::Energy &e, const colevent::Px &px,
            const colevent::Py &py, const colevent::Pz &pz)
        : Particle{colevent::Pt{px, py}, colevent::Phi{px, py}},
          pz_{pz.value},
          e_{e.value} {
        colevent::Eta eta(px, py, pz);
        eta_ = eta.value;
        colevent::FourMomentum p{e, px, py, pz};
        m_ = p.mass();
    }
    Visible(const colevent::Pt &pt, const colevent::Eta &eta,
            const colevent::Phi &phi, const colevent::Mass &m, const int &ntrk)
        : Particle{pt, phi},
          eta_{eta.value},
          m_{m.value},
          pz_{pt.value * std::sinh(eta.value)} {
        set_energy(m.value);
        set_charge(ntrk);
    }
    virtual ~Visible() {}

    double eta() const { return eta_; }
    double mass() const { return m_; }
    double pz() const { return pz_; }
    double energy() const { return e_; }
    int charge() const {
        switch (q_) {
        case Charge::Positive:
            return 1;
        case Charge::Negative:
            return -1;
        default:
            return 0;
        }
    }

    virtual std::string show() const;
    friend bool operator<(const Visible &lhs, const Visible &rhs) {
        return lhs.pt() < rhs.pt();
    }
    friend bool operator>(const Visible &lhs, const Visible &rhs) {
        return rhs < lhs;
    }
    Visible &operator+=(const Visible &rhs);
    friend Visible operator+(Visible lhs, const Visible &rhs) {
        return lhs += rhs;
    }
};

using Visibles = std::vector<Visible>;

class Photon : public Visible {
public:
    Photon() {}
    Photon(const colevent::Pt &pt, const colevent::Eta &eta,
           const colevent::Phi &phi, const colevent::Mass &m)
        : Visible(pt, eta, phi, m) {}
    ~Photon() {}

    std::string show() const;
};

struct Electron : public Visible {
    Electron() {}
    Electron(const colevent::Pt &pt, const colevent::Eta &eta,
             const colevent::Phi &phi, const colevent::Mass &m, const int ntrk)
        : Visible(pt, eta, phi, m, ntrk) {}
    ~Electron() {}

    std::string show() const;
};

class Muon : public Visible {
private:
    double ptiso_;
    double etrat_;

protected:
    void set_ptiso_etrat(double hadem) {
        ptiso_ = std::trunc(hadem);
        etrat_ = hadem - ptiso_;
    }

public:
    Muon() {}
    Muon(const colevent::Pt &pt, const colevent::Eta &eta,
         const colevent::Phi &phi, const colevent::Mass &m, const int ntrk,
         const double hadem)
        : Visible(pt, eta, phi, m, ntrk) {
        set_ptiso_etrat(hadem);
    }
    ~Muon() {}

    double ptiso() const { return ptiso_; }
    double etrat() const { return etrat_; }

    std::string show() const;
};

class Tau : public Visible {
public:
    enum class TauProng { OneProng, ThreeProng };

private:
    TauProng prong_;

protected:
    void set_prong(int ntrk) {
        if (ntrk < 2) {
            prong_ = TauProng::OneProng;
        } else {
            prong_ = TauProng::ThreeProng;
        }
    }

public:
    Tau() {}
    Tau(const colevent::Pt &pt, const colevent::Eta &eta,
        const colevent::Phi &phi, const colevent::Mass &m, const int ntrk)
        : Visible(pt, eta, phi, m) {
        ntrk > 0 ? set_charge(1) : set_charge(-1);
        set_prong(ntrk);
    }
    ~Tau() {}

    int prong() const { return prong_ == TauProng::OneProng ? 1 : 3; }

    std::string show() const;
};

class Jet : public Visible {
private:
    int num_track_;

public:
    Jet() {}
    Jet(const colevent::Pt &pt, const colevent::Eta &eta,
        const colevent::Phi &phi, const colevent::Mass &m, const int ntrk)
        : Visible(pt, eta, phi, m), num_track_(ntrk) {}
    virtual ~Jet() {}

    int num_track() const { return num_track_; }

    virtual std::string show() const;
};

class Bjet : public Jet {
public:
    enum class BTag { Loose, Tight };

private:
    BTag btag_ = BTag::Loose;

public:
    Bjet() {}
    Bjet(const colevent::Pt &pt, const colevent::Eta &eta,
         const colevent::Phi &phi, const colevent::Mass &m, const int ntrk,
         const int btag)
        : Jet(pt, eta, phi, m, ntrk) {
        btag < 1.5 ? btag_ = BTag::Loose : btag_ = BTag::Tight;
    }
    ~Bjet() {}

    int btag() const { return btag_ == BTag::Loose ? 1 : 2; }

    std::string show() const;
};
}  // namespace lhco

#endif  // COLEVENT_SRC_LHCO_PARTICLE_H_
