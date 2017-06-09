/* Copyright (c) 2014-2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#include "lhef/parser.h"
#include <istream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

using std::string;

namespace lhef {
void skipTillEventLine(std::istream *is) {
    std::string line;
    while (std::getline(*is, line) && line.find("<event") == string::npos) {
        continue;
    }
}

std::string eventStr(std::istream *is) {
    skipTillEventLine(is);

    std::string event_line(""), line;
    while (std::getline(*is, line)) {
        if (line.find("</event") == string::npos) {
            if (line.front() == '#') {  // comment line
                continue;
            } else {
                event_line += line + '\n';
            }
        } else {  // end of the event lines reached
            break;
        }
    }
    return event_line;
}

Event parseEvent(std::istream *is) {
    string evstr = eventStr(is);

    Event lhe;
    if (!evstr.empty()) {
        std::istringstream iss(evstr);
        EventInfo evinfo;
        iss >> evinfo;

        Particle p;
        EventEntry entry;
        for (int i = 0; i < evinfo.nup; ++i) {
            iss >> p;
            entry.insert({i + 1, p});
        }
        lhe.setEvent(evinfo, entry);
    } else {
        lhe(Event::EventStatus::Empty);
    }
    return lhe;
}

std::pair<bool, Event> parseOrFail(std::shared_ptr<std::istream> is) {
    const Event lhe{lhef::parseEvent(is.get())};
    if (lhe.empty()) { return std::make_pair(false, lhe); }
    return std::make_pair(true, lhe);
}
}  // namespace lhef
