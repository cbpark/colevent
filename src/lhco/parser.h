/* Copyright (c) 2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#ifndef COLEVENT_SRC_LHCO_PARSER_H_
#define COLEVENT_SRC_LHCO_PARSER_H_

#include "lhco/event.h"

namespace lhco {
RawEvent parseRawEvent(std::istream *is);

Event parseEvent(std::istream *is);
}  // namespace lhco

#endif  // COLEVENT_SRC_LHCO_PARSER_H_
