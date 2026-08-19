# CPE333 PS02 – Debian setup

Problem Session 2 covers process creation, process lifetime, `fork()`, `wait()`, and pipes.

- `problem-session.md` – assignment requirements.
- `src/` – starter C programs.
- `Makefile` – repeatable build commands.
- `notes/` – experiment observations and report material.

## Publish this work

Run these commands on the development machine from the repository root. The path is targeted so unrelated untracked directories are not added.

```bash
git add Y3/CPE333/ps02
git commit -m "Add CPE333 process lab starter"
git push origin main
```

## Get it in Debian

First time:

```bash
sudo apt update
sudo apt install -y build-essential gdb git man-db manpages-dev procps psmisc strace
git clone https://github.com/beambeambeam/my-cpe-lab.git
cd my-cpe-lab/Y3/CPE333/ps02
```

Already cloned:

```bash
cd ~/my-cpe-lab
git pull --ff-only origin main
cd Y3/CPE333/ps02
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
