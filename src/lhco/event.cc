/* Copyright (c) 2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#include "lhco/event.h"
#include <algorithm>
#include <functional>
#include <iomanip>

namespace lhco {
std::string RawEvent::show() const {
    std::string str = "RawEvent {" + header_.show() + ",[";
    for (const auto &o : objects_) { str += o.show() + ","; }
    str.pop_back();
    str += "]}";
    return str;
}

std::ostream &operator<<(std::ostream &os, const RawEvent &ev) {
    os << std::setw(3) << 0 << ev.header_ << '\n';
    auto ss = os.precision();
    int counter = 1;
    double dummy = 0.0;
    for (const auto &obj : ev.objects_) {
        os << std::setw(3) << counter << obj << std::setprecision(1)
           << std::setw(6) << dummy << std::setw(6) << dummy;
        if (obj.typ != 6) { os << '\n'; }
        os.precision(ss);
        ++counter;
    }
    return os;
}

template <typename T>
std::string showAll(const std::vector<T> &ps) {
    std::string str = "";
    if (!ps.empty()) {
        str += "[";
        for (const auto &p : ps) { str += p.show() + ","; }
        str.pop_back();
        str += "],";
    }
    return str;
}

std::string Event::show() const {
    std::string str = "Event {";
    str += showAll(photons_) + showAll(electrons_) + showAll(muons_);
    str += showAll(taus_) + showAll(jets_) + showAll(bjets_);
    str += met_.show() + "}";
    return str;
}

template <typename T>
void sortByPt(std::vector<T> *ps) {
    if (!ps->empty()) { std::sort(ps->begin(), ps->end(), std::greater<T>()); }
}

void Event::sort_particles() {
    sortByPt(&photons_);
    sortByPt(&electrons_);
    sortByPt(&muons_);
    sortByPt(&taus_);
    sortByPt(&jets_);
    sortByPt(&bjets_);
}
}  // namespace lhco
