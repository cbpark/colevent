# colevent

Tools for the [Les Houches Event File](http://home.thep.lu.se/~leif/LHEF/) (LHEF) and LHC Olympics (LHCO) data analyses using C++. If you want to use Haskell, see [lhevent](https://github.com/cbpark/lhevent).

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

* [Nix](https://nixos.org/nix/) users can build this package by running

```
nix-build
```

See [default.nix](./default.nix).

## Usage

* LHEF

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

* LHCO

``` c++
#include <fstream>
#include <iostream>
#include "lhco/lhco.h"

using std::cout;

int main(int, char *argv[]) {
    std::ifstream fin(argv[1]);
    auto event = lhco::parseEvent(&fin);
    for (int ieve = 1; !event.done(); event = lhco::parseEvent(&fin), ++ieve) {
        cout << "-- event : " << ieve << '\n';
        for (const auto &j : event.jets()) { cout << lhco::show(j) << '\n'; }
        cout << "missing energy = " << lhco::missingET(event) << '\n';
    }
}
```

See also `test_parser_@(lhco|lhef).cc` and `test_render_@(lhco|lhef).cc` in [`src`](src).


Setting flags might be necessary for building your programs. For example, you can add

``` makefile
COLEVENT := /path/to/colevent
CXXFLAGS += -I$(COLEVENT)/include/colevent
LDFLAGS  += -L$(COLEVENT)/lib -lcolevent
```

to the Makefile.

If it has been installed to the system path, `pkg-config` will provide the proper flags:

```
pkg-config --cflags --libs colevent
```

## References

- [A standard format for Les Houches Event Files](http://arxiv.org/abs/hep-ph/0609017).
- [Les Houches Event File](http://home.thep.lu.se/~leif/LHEF/).
- [How to Read LHC Olympics Data Files](http://madgraph.phys.ucl.ac.be/Manual/lhco.html).
- [LHC Olympics Wiki](http://www.jthaler.net/olympicswiki/doku.php).
- [PGS 4 and the LHC Olympics](http://online.kitp.ucsb.edu/online/lhco_c06/conway/).
