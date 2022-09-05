# Test Exchange

## Build
The first step is to update submodules:
```
git submodule update --init --recursive
```

The second step is to build all use `Makefile` in the root project directory.
```
make debug - builds debug version
make release - builds release version
make config-debug - run CMake configuration for debug
make config-release - run CMake configuration for release
```