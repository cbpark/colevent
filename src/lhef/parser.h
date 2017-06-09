/* Copyright (c) 2014-2015, 2017, Chan Beom Park <cbpark@gmail.com> */

#ifndef COLEVENT_SRC_LHEF_PARSER_H_
#define COLEVENT_SRC_LHEF_PARSER_H_

#include <istream>
#include <memory>
#include <utility>
#include "lhef/event.h"

namespace lhef {
Event parseEvent(std::istream *is);

std::pair<bool, Event> parseOrFail(std::shared_ptr<std::istream> is);
}  // namespace lhef

#endif  // COLEVENT_SRC_LHEF_PARSER_H_
