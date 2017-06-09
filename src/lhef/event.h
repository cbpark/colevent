/* Copyright (c) 2014-2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#ifndef COLEVENT_SRC_LHEF_EVENT_H_
#define COLEVENT_SRC_LHEF_EVENT_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "lhef/particle.h"

namespace lhef {
struct GlobalInfo {
    // PDG codes of the beams.
    std::pair<int, int> idbmup;
    // Energies of the beams.
    std::pair<double, double> ebmup;
    // Author groups of the PDF.
    std::pair<int, int> pdfgup;
    // ID of the PDF set.
    std::pair<int, int> pdfsup;
    // Weighting strategy.
    int idwtup;
    // Number of processes.
    int nprup;
    // Cross sections.
    std::vector<double> xsecup;
    // Cross section errors.
    std::vector<double> xerrup;
    // Maximum event weight.
    std::vector<double> xmaxup;
    // Process ID.
    std::vector<int> lprup;

    GlobalInfo() {}
    GlobalInfo(int _idbmup1, int _idbmup2, double _ebmup1, double _ebmup2,
               int _pdfgup1, int _pdfgup2, int _pdfsup1, int _pdfsup2,
               int _idwtup, int _nprup, std::vector<double> _xsecup,
               std::vector<double> _xerrup, std::vector<double> _xmaxup,
               std::vector<int> _lprup)
        : idbmup({_idbmup1, _idbmup2}),
          ebmup({_ebmup1, _ebmup2}),
          pdfgup({_pdfgup1, _pdfgup2}),
          pdfsup({_pdfsup1, _pdfsup2}),
          idwtup(_idwtup),
          nprup(_nprup),
          xsecup(_xsecup),
          xerrup(_xerrup),
          xmaxup(_xmaxup),
          lprup(_lprup) {}

    friend std::istream &operator>>(std::istream &is, GlobalInfo &info);
    friend std::ostream &operator<<(std::ostream &os, const GlobalInfo &info);
};

struct EventInfo {
    // Number of particle entries in the event.
    int nup = 0;
    // ID of the process for the event.
    int idprup = 0;
    // Event weight.
    double xwgtup = 0.0;
    // Scale of the event in GeV.
    double scalup = 0.0;
    // The QED coupling \alpha_{QED} used for the event.
    double aqedup = 0.0;
    // The QCD coupling \alpha_{QCD} used for the event.
    double aqcdup = 0.0;

    EventInfo() {}
    EventInfo(int _nup, int _idprup, double _xwgtup, double _scalup,
              double _aqedup, double _aqcdup)
        : nup(_nup),
          idprup(_idprup),
          xwgtup(_xwgtup),
          scalup(_scalup),
          aqedup(_aqedup),
          aqcdup(_aqcdup) {}

    friend std::istream &operator>>(std::istream &is, EventInfo &evinfo);
    friend std::ostream &operator<<(std::ostream &os, const EventInfo &evinfo);
};

std::string show(const EventInfo &evinfo);

using EventEntry = std::unordered_map<int, Particle>;
std::string show(const EventEntry &entry);

class Event {
public:
    enum class EventStatus { Empty, Fill };

private:
    EventStatus status_;
    std::pair<EventInfo, EventEntry> event_;

public:
    explicit Event(EventStatus s = EventStatus::Empty) : status_(s) {}
    Event(const EventInfo &evinfo, const EventEntry &ev)
        : status_(EventStatus::Fill), event_({evinfo, ev}) {}

    void setEvent(const EventInfo &evinfo, const EventEntry &entry) {
        status_ = EventStatus::Fill;
        event_ = std::make_pair(evinfo, entry);
    }

    EventInfo eventInfo() const { return event_.first; }
    EventEntry particleEntries() const { return event_.second; }
    Particles particles() const;

    bool empty() const { return status_ == EventStatus::Empty; }
    bool done() const { return empty(); }

    void operator()(const EventStatus &s) { status_ = s; }

    friend std::string show(const Event &ev);
    friend std::ostream &operator<<(std::ostream &os, const Event &ev);
};

std::string show(const Event &ev);
}  // namespace lhef

#endif  // COLEVENT_SRC_LHEF_EVENT_H_
