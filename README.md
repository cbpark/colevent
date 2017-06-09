# colevent

Tools for the [Les Houches Event File](http://home.thep.lu.se/~leif/LHEF/) (LHEF) and LHC Olympics (LHCO) data analyses using C++. If you want to use Haskell, see [lhef-tools](https://github.com/cbpark/lhef-tools) and [lhco-tools](https://github.com/cbpark/lhco-tools).

### Prerequisite

- C++ compiler supporting C++11 features ([Clang](http://clang.llvm.org/cxx_status.html) >= 3.3, [GCC](https://gcc.gnu.org/projects/cxx-status.html) >= 4.8.1),
- (optional) [ROOT](https://root.cern.ch/).

## Installation

```
./configure
make
make install
```

If ROOT is not located in the system path, it can be set by `./configure --with-rootsys=path/of/root`.

## Usage

``` c++
#include <fstream>
#include <iostream>
#include "lhef/lhef.h"

int main(int, char *argv[]) {
    std::ifstream fin(argv[1]);
    auto event = lhef::parseEvent(&fin);
    for (int ieve = 1; !event.done(); event = lhef::parseEvent(&fin), ++ieve) {
        std::cout << "-- event : " << ieve << '\n';
        const auto final_states{lhef::finalStates(event)};
        std::cout << lhef::show(final_states) << '\n';
    }
}
```

Setting flags might be necessary for building your programs. For example,

``` makefile
COLEVENT := /path/colevent
CXXFLAGS += -I$(COLEVENT)/include/colevent
LDFLAGS  += -L$(COLEVENT)/lib -lcolevent
```

## References

- [A standard format for Les Houches Event Files](http://arxiv.org/abs/hep-ph/0609017).
- [Les Houches Event File](http://home.thep.lu.se/~leif/LHEF/).
- [How to Read LHC Olympics Data Files](http://madgraph.phys.ucl.ac.be/Manual/lhco.html).
- [LHC Olympics Wiki](http://www.jthaler.net/olympicswiki/doku.php).
- [PGS 4 and the LHC Olympics](http://online.kitp.ucsb.edu/online/lhco_c06/conway/).