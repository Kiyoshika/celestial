# liblite
A lightweight library for C containing common data structures such as vectors, strings and maps.

# Building
From the root directory:
* `mkdir build && cd build`
* `cmake ..` (compiles in `RELEASE` mode by default)
    * Use `cmake -DDEBUG_MODE=ON ..` to compile in `DEBUG` mode
* `make` to build `liblite.a` in the `build/src/` directory
