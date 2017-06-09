/* Copyright (c) 2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#include "lhco/particle.h"
#include <string>

using std::to_string;
using colevent::Energy;
using colevent::Px;
using colevent::Py;
using colevent::Pz;

namespace lhco {
std::string Met::show() const {
    return "Met {pt=" + to_string(pt()) + ",phi=" + to_string(phi()) + "}";
}

std::string show_pt_eta_phi(const Visible &p) {
    return "pt=" + to_string(p.pt()) + ",eta=" + to_string(p.eta()) +
           ",phi=" + to_string(p.phi());
}

std::string show_pt_eta_phi_m(const Visible &p) {
    return show_pt_eta_phi(p) + ",mass=" + to_string(p.mass());
}

std::string show_e_px_py_pz(const Visible &p) {
    return "e=" + to_string(p.energy()) + ",px=" + to_string(p.px()) +
           ",py=" + to_string(p.py()) + ",pz=" + to_string(p.pz());
}

std::string Visible::show() const {
    return "Visible {" + show_pt_eta_phi_m(*this) + "," +
           show_e_px_py_pz(*this) + "}";
}

Visible &Visible::operator+=(const Visible &rhs) {
    Energy e(this->energy() + rhs.energy());
    Px px(this->px() + rhs.px());
    Py py(this->py() + rhs.py());
    Pz pz(this->pz() + rhs.pz());
    *this = Visible(e, px, py, pz);
    return *this;
}

std::string Photon::show() const {
    return "Photon {" + show_pt_eta_phi(*this) + "}";
}

std::string Electron::show() const {
    return "Electron {" + show_pt_eta_phi(*this) +
           ",charge=" + to_string(charge()) + "}";
}

std::string Muon::show() const {
    return "Muon {" + show_pt_eta_phi_m(*this) +
           ",charge=" + to_string(charge()) + ",ptiso=" + to_string(ptiso()) +
           ",etrat=" + to_string(etrat()) + "}";
}

std::string Tau::show() const {
    return "Tau {" + show_pt_eta_phi_m(*this) +
           ",charge=" + to_string(charge()) + ",prong=" + to_string(prong()) +
           "}";
}

std::string Jet::show() const {
    return "Jet {" + show_pt_eta_phi_m(*this) +
           ",ntrk=" + to_string(num_track()) + "}";
}

std::string Bjet::show() const {
    return "Bjet {" + show_pt_eta_phi_m(*this) +
           ",ntrk=" + to_string(num_track()) + ",btag=" + to_string(btag()) +
           "}";
}
}  // namespace lhco
