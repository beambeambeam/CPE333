# CPE333

Standalone repository for CPE333 coursework.

## Platform support

The programs use C17 and POSIX process APIs: `fork()`, `wait()`, `pipe()`, `read()`, `write()`, `close()`, and `sleep()`.

Supported environments:

- Linux distributions – Debian, Ubuntu, Fedora, Arch, and others.
- WSL2 with a Linux distribution.
- macOS with Xcode Command Line Tools.
- BSD systems with a C compiler and POSIX userland.

Native Windows is not supported because Windows does not provide `fork()` with these POSIX semantics. Use WSL2, a Linux VM, or another Unix-like environment.

## Clone

```bash
git clone https://github.com/beambeambeam/CPE333.git
cd CPE333
```

## Contents

- `ps02/` – Problem Session 2: process creation and pipes.

Build PS02 programs:

```bash
cd ps02
make
```

`CPE333` is also available inside `my-cpe-lab` as a Git submodule. It remains an independent repository.
