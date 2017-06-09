/* Copyright (c) 2014-2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#include "lhef/event.h"
#include <algorithm>
#include <iomanip>
#include <ios>
#include <iostream>
#include <map>

using std::setw;
using std::to_string;

namespace lhef {
Particles Event::particles() const {
    Particles ps;
    const auto entries = particleEntries();
    ps.reserve(entries.size());
    std::for_each(
        entries.cbegin(), entries.cend(),
        [&ps](const EventEntry::value_type &e) { ps.push_back(e.second); });
    return ps;
}

std::istream &operator>>(std::istream &is, GlobalInfo &info) {
    is >> info.idbmup.first >> info.idbmup.second >> info.ebmup.first >>
        info.ebmup.second >> info.pdfgup.first >> info.pdfgup.second >>
        info.pdfsup.first >> info.pdfsup.second >> info.idwtup >> info.nprup;

    double _xsecup, _xerrup, _xmaxup;
    int _lprup;
    for (int i = 0; i < info.nprup; ++i) {
        is >> _xsecup >> _xerrup >> _xmaxup >> _lprup;
        info.xsecup.push_back(_xsecup);
        info.xerrup.push_back(_xerrup);
        info.xmaxup.push_back(_xmaxup);
        info.lprup.push_back(_lprup);
    }

    return is;
}

std::ostream &operator<<(std::ostream &os, const GlobalInfo &info) {
    auto ss = os.precision();

    os << "<init>\n";
    os << setw(9) << info.idbmup.first << setw(9) << info.idbmup.second
       << std::setprecision(11) << std::scientific << std::uppercase << setw(19)
       << info.ebmup.first << setw(19) << info.ebmup.second;
    os.precision(ss);
    os << setw(2) << info.pdfgup.first << setw(2) << info.pdfgup.second
       << setw(6) << info.pdfsup.first << setw(6) << info.pdfsup.second
       << setw(2) << info.idwtup << setw(3) << info.nprup << '\n';

    auto xsecup_it = info.xsecup.begin();
    auto xerrup_it = info.xerrup.begin();
    auto xmaxup_it = info.xmaxup.begin();
    auto lprup_it = info.lprup.begin();
    for (; xsecup_it != info.xsecup.end() || xerrup_it != info.xerrup.end() ||
           xmaxup_it != info.xmaxup.end() || lprup_it != info.lprup.end();
         ++xsecup_it, ++xerrup_it, ++xmaxup_it, ++lprup_it) {
        os << std::setprecision(11) << std::scientific << std::uppercase
           << setw(19) << *xsecup_it << setw(19) << *xerrup_it << setw(19)
           << *xmaxup_it;
        os << setw(4) << *lprup_it << '\n';
    }

    os << "</init>";

    os.precision(ss);
    return os;
}

std::istream &operator>>(std::istream &is, EventInfo &evinfo) {
    is >> evinfo.nup >> evinfo.idprup >> evinfo.xwgtup >> evinfo.scalup >>
        evinfo.aqedup >> evinfo.aqcdup;
    return is;
}

std::ostream &operator<<(std::ostream &os, const EventInfo &evinfo) {
    auto ss = os.precision();
    os << setw(2) << evinfo.nup << setw(4) << evinfo.idprup
       << std::setprecision(7) << std::scientific << std::uppercase << setw(15)
       << evinfo.xwgtup << setw(15) << evinfo.scalup << setw(15)
       << evinfo.aqedup << setw(15) << evinfo.aqcdup;
    os.precision(ss);
    return os;
}

std::string show(const EventInfo &evinfo) {
    std::string evinfo_str = "EventInfo {nup=" + to_string(evinfo.nup) +
                             ",idprup=" + to_string(evinfo.idprup) +
                             ",xwgtup=" + to_string(evinfo.xwgtup) +
                             ",scalup=" + to_string(evinfo.scalup) +
                             ",aqedup=" + to_string(evinfo.aqedup) +
                             ",aqcdup=" + to_string(evinfo.aqcdup) + "}";
    return evinfo_str;
}

std::string show(const EventEntry &entry) {
    std::string entry_str = "[";
    for (const auto &e : entry) {
        entry_str += "(" + to_string(e.first) + "," + show(e.second) + "),";
    }
    entry_str.pop_back();
    entry_str += "]";
    return entry_str;
}

std::ostream &operator<<(std::ostream &os, const Event &ev) {
    os << "<event>\n" << ev.event_.first << '\n';

    // EventEntry is unordered_map. It has to be ordered.
    const std::map<int, Particle> entry_ordered(ev.event_.second.cbegin(),
                                                ev.event_.second.cend());
    for (const auto &entry : entry_ordered) { os << entry.second << '\n'; }
    os << "</event>";
    return os;
}

std::string show(const Event &ev) {
    std::string ev_str = "Event (";
    ev_str += show(ev.event_.first) + "," + show(ev.event_.second) + ")";
    return ev_str;
}
}  // namespace lhef
