/* Copyright (c) 2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#ifndef COLEVENT_SRC_LHCO_EVENT_H_
#define COLEVENT_SRC_LHCO_EVENT_H_

#include <ostream>
#include <string>
#include <vector>
#include "lhco/object.h"
#include "lhco/particle.h"

namespace lhco {
enum class EventStatus { Empty, Fill };

class RawEvent {
private:
    EventStatus status_;
    Header header_;
    Objects objects_;

public:
    explicit RawEvent(EventStatus s = EventStatus::Empty) : status_(s) {}
    RawEvent(const Header &header, const Objects &objects)
        : status_(EventStatus::Fill), header_(header), objects_(objects) {}

    void set_event(const Header &header, const Objects &objects) {
        status_ = EventStatus::Fill;
        header_ = header;
        objects_ = objects;
    }
    Header header() const { return header_; }
    Objects objects() const { return objects_; }
    bool empty() const { return status_ == EventStatus::Empty; }
    void operator()(const EventStatus &s) { status_ = s; }

    std::string show() const;
    friend std::ostream &operator<<(std::ostream &os, const RawEvent &ev);
};

class Event {
private:
    EventStatus status_;

    std::vector<Photon> photons_;
    std::vector<Electron> electrons_;
    std::vector<Muon> muons_;
    std::vector<Tau> taus_;
    std::vector<Jet> jets_;
    std::vector<Bjet> bjets_;
    Met met_;

public:
    explicit Event(EventStatus s = EventStatus::Empty) : status_(s) {}

    std::vector<Photon> photon() const { return photons_; }
    void add_photon(const Object &obj) {
        status_ = EventStatus::Fill;
        Photon photon{colevent::Pt{obj.pt}, colevent::Eta{obj.eta},
                      colevent::Phi{obj.phi}, colevent::Mass{obj.jmass}};
        photons_.push_back(photon);
    }
    std::vector<Electron> electron() const { return electrons_; }
    void add_electron(const Object &obj) {
        status_ = EventStatus::Fill;
        Electron elec{colevent::Pt{obj.pt}, colevent::Eta{obj.eta},
                      colevent::Phi{obj.phi}, colevent::Mass{obj.jmass},
                      obj.ntrk};
        electrons_.push_back(elec);
    }
    std::vector<Muon> muon() const { return muons_; }
    void add_muon(const Object &obj) {
        status_ = EventStatus::Fill;
        Muon muon{colevent::Pt{obj.pt},
                  colevent::Eta{obj.eta},
                  colevent::Phi{obj.phi},
                  colevent::Mass{obj.jmass},
                  obj.ntrk,
                  obj.hadem};
        muons_.push_back(muon);
    }
    std::vector<Tau> tau() const { return taus_; }
    void add_tau(const Object &obj) {
        status_ = EventStatus::Fill;
        Tau tau{colevent::Pt{obj.pt}, colevent::Eta{obj.eta},
                colevent::Phi{obj.phi}, colevent::Mass{obj.jmass}, obj.ntrk};
        taus_.push_back(tau);
    }
    std::vector<Jet> jet() const { return jets_; }
    void add_jet(const Object &obj) {
        status_ = EventStatus::Fill;
        Jet jet{colevent::Pt{obj.pt}, colevent::Eta{obj.eta},
                colevent::Phi{obj.phi}, colevent::Mass{obj.jmass}, obj.ntrk};
        jets_.push_back(jet);
    }
    std::vector<Bjet> bjet() const { return bjets_; }
    void add_bjet(const Object &obj) {
        status_ = EventStatus::Fill;
        Bjet bjet{colevent::Pt{obj.pt},
                  colevent::Eta{obj.eta},
                  colevent::Phi{obj.phi},
                  colevent::Mass{obj.jmass},
                  obj.ntrk,
                  obj.btag};
        bjets_.push_back(bjet);
    }
    Met met() const { return met_; }
    void set_met(const Object &obj) {
        status_ = EventStatus::Fill;
        met_ = Met{colevent::Pt{obj.pt}, colevent::Phi{obj.phi}};
    }
    bool empty() const { return status_ == EventStatus::Empty; }
    bool done() const { return empty(); }
    void operator()(const EventStatus &s) { status_ = s; }
    void sort_particles();

    std::string show() const;
};
}  // namespace lhco

#endif  // COLEVENT_SRC_LHCO_EVENT_H_
