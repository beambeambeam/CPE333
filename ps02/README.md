# CPE333 PS02 – Debian setup

Problem Session 2 covers process creation, process lifetime, `fork()`, `wait()`, and pipes.

- `problem-session.md` – assignment requirements.
- `src/` – starter C programs.
- `Makefile` – repeatable build commands.
- `notes/` – experiment observations and report material.

## Get it in Debian

Install the required tools:

```bash
sudo apt update
sudo apt install -y build-essential gdb git man-db manpages-dev procps psmisc strace
```

Standalone clone – recommended:

```bash
git clone https://github.com/beambeambeam/CPE333.git
cd CPE333/ps02
```

Already cloned:

```bash
cd ~/CPE333
git pull --ff-only origin main
cd ps02
```

Parent-repository clone:

```bash
git clone --recurse-submodules https://github.com/beambeambeam/my-cpe-lab.git
cd my-cpe-lab/Y3/CPE333/ps02
```

If the parent repository was cloned without submodules:

```bash
cd ~/my-cpe-lab
git pull --ff-only origin main
git submodule update --init --recursive
cd Y3/CPE333/ps02
```

## Publish changes

CPE333 changes are committed and pushed from the standalone repository:

```bash
cd ~/CPE333
git add ps02
git commit -m "Update PS02 lab"
git push origin main
```

The parent repository stores a fixed CPE333 commit. Update its pointer only when needed:

```bash
cd ~/my-cpe-lab/Y3/CPE333
git pull --ff-only origin main
cd ../..
git add Y3/CPE333
git commit -m "Update CPE333 submodule"
git push origin main
```

## Build

```bash
make
```

Compiler flags enable C17, POSIX declarations, warnings, and debugging symbols. Binaries are written to `bin/`, which is ignored by Git.

Clean build output:

```bash
make clean
make
```

## Run starter programs

```bash
./bin/01_hello
./bin/02_hello_wait
./bin/03_lifetime child-first
./bin/03_lifetime parent-first
./bin/05_pipe
```

`03_lifetime child-first` keeps the parent alive while the child exits. Inspect the child during the parent sleep:

```bash
ps -u "$USER" -o pid,ppid,stat,cmd
ps -ef --forest
pstree -p "$$"
```

The child can appear as `<defunct>` until the parent calls `waitpid()`.

`03_lifetime parent-first` exits the parent while the child continues. The child prints its parent PID before and after sleeping. The second PPID normally becomes PID 1 or another system subreaper.

## Fork-count safety

Take a VM snapshot before running this experiment. Run it as a normal user, never with `sudo`.

The program creates one child at a time and each parent waits, but it still must run under a process limit:

```bash
( ulimit -u 128; ./bin/04_fork_count )
```

The program reports successful `fork()` calls when `fork()` fails. Do not run an uncontrolled program where every process forks again.

To stop an experiment, inspect exact user process IDs first:

```bash
pgrep -a -u "$USER"
kill <pid>
```

## Manual pages

```bash
man 2 fork
man 2 wait
man 2 pipe
man 2 read
man 2 write
man 2 close
man 3 sleep
man 1 ps
```

Optional syscall trace:

```bash
strace -f ./bin/05_pipe
```

## Pipe observations to record

The base pipe program demonstrates child-to-parent communication. Create or modify variants for the required experiments:

1. A sender reading from the same pipe consumes bytes from the shared byte stream; it does not read a private copy.
2. A receiver reading before the sender writes blocks while a write end remains open.
3. Several writes can be returned by one read, or one message can be split across reads. A pipe has no message boundaries.
4. Closing every write end lets the reader receive EOF (`read()` returns `0`).

Save commands, output, PIDs, PPIDs, process states, and explanations in `notes/` for the PDF report.
