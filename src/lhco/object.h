/* Copyright (c) 2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#ifndef COLEVENT_SRC_LHCO_OBJECT_H_
#define COLEVENT_SRC_LHCO_OBJECT_H_

#include <string>
#include <vector>

namespace lhco {
struct Header {
    int event_number = 0;
    int trigger_word = 0;

    Header() {}
    Header(int _evnum, int _trig) : event_number(_evnum), trigger_word(_trig) {}

    std::string show() const;
    friend std::istream &operator>>(std::istream &is, Header &h);
    friend std::ostream &operator<<(std::ostream &os, const Header &h);
};

struct Object {
    int typ = 0;
    double eta = 0.0;
    double phi = 0.0;
    double pt = 0.0;
    double jmass = 0.0;
    int ntrk = 0;
    int btag = 0;
    double hadem = 0.0;

    Object() {}
    Object(int _typ, double _eta, double _phi, double _pt, double _jmass,
           int _ntrk, int _btag, double _hadem)
        : typ(_typ),
          eta(_eta),
          phi(_phi),
          pt(_pt),
          jmass(_jmass),
          ntrk(_ntrk),
          btag(_btag),
          hadem(_hadem) {}

    std::string show() const;
    friend std::istream &operator>>(std::istream &is, Object &obj);
    friend std::ostream &operator<<(std::ostream &os, const Object &obj);
};

using Objects = std::vector<Object>;
}  // namespace lhco

#endif  // COLEVENT_SRC_LHCO_OBJECT_H_
