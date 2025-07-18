# CMakeGen CLI

A minimal command‑line utility that generates a project directory pre‑populated with a basic `CMakeLists.txt`. Additional features are on the roadmap.

---

## Quick start

```bash
git clone https://github.com/Ronit9320/CMake-gen-CLI
cd CMake-gen-CLI
mkdir build && cd build
cmake ..
make
```

## To install it system wide

```
sudo make install
```

## Usage

```bash
./gencmake
```

This creates a new folder called `<project_name>` containing:

```
<project_name>/
└── CMakeLists.txt
```

## Roadmap

- Additional project templates (library, executable, tests)
- Customizable C++ standard flag
- Better error checking

---

## License

MIT
