/* Copyright (c) 2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#include "lhco/parser.h"
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include "object.h"

using std::istringstream;

namespace lhco {
Object getObj(std::unique_ptr<istringstream> iss, const int &typ) {
    Object obj;
    obj.typ = typ;
    *iss >> obj;
    return obj;
}

RawEvent parseRawEvent(std::istream *is) {
    std::string line;
    Header header;
    Objects objs;
    RawEvent lhco;

    while (std::getline(*is, line)) {
        if (line.find("#") == std::string::npos) {
            std::unique_ptr<istringstream> iss(new istringstream(line));
            int first_digit = 0, second_digit = 0;
            *iss >> first_digit >> second_digit;
            if (first_digit == 0) {  // line for event header
                header.event_number = second_digit;
                *iss >> header;
            } else if (second_digit < 6) {
                Object obj = getObj(std::move(iss), second_digit);
                objs.push_back(obj);
            } else if (second_digit == 6) {  // line for missing energy
                Object obj = getObj(std::move(iss), second_digit);
                objs.push_back(obj);
                lhco.set_event(header, objs);
                break;
            } else {  // undefined line
                lhco(EventStatus::Empty);
                break;
            }
        } else {  // comment line
            continue;
        }
    }

    return lhco;
}

Event parseEvent(std::istream *is) {
    RawEvent raw_ev = parseRawEvent(is);
    Event ev;
    if (raw_ev.empty()) {
        ev(EventStatus::Empty);
    } else {
        for (const auto &obj : raw_ev.objects()) {
            switch (obj.typ) {
            case 0:  // photon
                ev.add_photon(obj);
                break;
            case 1:  // electron
                ev.add_electron(obj);
                break;
            case 2:  // muon
                ev.add_muon(obj);
                break;
            case 3:  // tau
                ev.add_tau(obj);
                break;
            case 4:
                if (obj.btag > 0.5) {  // b-jet
                    ev.add_bjet(obj);
                } else {  // normal jet
                    ev.add_jet(obj);
                }
                break;
            default:  // missing energy
                ev.set_met(obj);
                break;
            }
        }
        ev.sort_particles();
    }
    return ev;
}
}  // namespace lhco
