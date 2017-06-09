/* Copyright (c) 2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#include "lhco/object.h"
#include <iomanip>
#include <iostream>
#include <string>

using std::to_string;

namespace lhco {
std::string Header::show() const {
    return "Header {event_number=" + to_string(event_number) +
           ",trigger_word=" + to_string(trigger_word) + "}";
}

std::istream &operator>>(std::istream &is, Header &h) {
    is >> h.trigger_word;
    return is;
}

std::ostream &operator<<(std::ostream &os, const Header &h) {
    os << std::setw(14) << h.event_number << std::setw(7) << h.trigger_word;
    return os;
}

std::string Object::show() const {
    std::string str = "Object {";
    str += "typ=" + to_string(typ);
    str += ",eta=" + to_string(eta);
    str += ",phi=" + to_string(phi);
    str += ",pt=" + to_string(pt);
    str += ",jmass=" + to_string(jmass);
    str += ",ntrk=" + to_string(ntrk);
    str += ",btag=" + to_string(btag);
    str += ",hadem=" + to_string(hadem) + "}";
    return str;
}

std::istream &operator>>(std::istream &is, Object &obj) {
    double ntrk, btag, dummy;
    is >> obj.eta >> obj.phi >> obj.pt >> obj.jmass >> ntrk >> btag >>
        obj.hadem >> dummy >> dummy;
    obj.ntrk = static_cast<int>(ntrk);
    obj.btag = static_cast<int>(btag);
    return is;
}

std::ostream &operator<<(std::ostream &os, const Object &obj) {
    auto ss = os.precision();
    os << std::setw(5) << obj.typ << std::fixed << std::setprecision(3)
       << std::setw(9) << obj.eta << std::setw(7) << obj.phi
       << std::setprecision(2) << std::setw(8) << obj.pt << std::setw(8)
       << obj.jmass << std::setprecision(1) << std::setw(6)
       << static_cast<double>(obj.ntrk) << std::setw(6)
       << static_cast<double>(obj.btag) << std::setprecision(2) << std::setw(9)
       << obj.hadem;
    os.precision(ss);
    return os;
}
}  // namespace lhco
