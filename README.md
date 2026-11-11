*This project has been created as part of the 42 curriculum by raisufaj, itaharbo.*

# minishell

A Unix command-line shell written from scratch in C, modelled on bash.
It reads commands, parses them into a syntax tree, and runs them using the
same building blocks a real shell uses: processes, pipes, file descriptors
and signals.

| | |
|---|---|
| **Language** | C, compiled with `-Wall -Wextra -Werror` |
| **Size** | ~4,300 lines in 51 source files, plus ~2,000 lines of our own C utility library (libft) |
| **Dependencies** | GNU readline only (for line editing and history) |
| **Team** | 2 developers: Rasiol Isufaj and Idriss Taharbouche |
| **Built** | May–July 2025 |
| **Tested with** | 113 automated comparisons against bash, 13 interactive terminal tests, valgrind |

## Description

minishell is an interactive shell that behaves like bash for everyday use.
You can run programs, chain them with pipes, redirect their input and
output, use variables, and combine commands with `&&`, `||` and
parentheses.

**What it supports**

- **Running programs** found through `PATH`, or given by a relative or absolute path
- **Pipes**: `ls | grep .c | wc -l`
- **Redirections**: `<`, `>`, `>>`, and heredocs `<<` (multi-line input, with variable expansion)
- **Quotes**: `'single'` (everything literal) and `"double"` (variables still expand)
- **Variables**: `$NAME`, plus `$?` for the exit status of the last command
- **Logic and grouping**: `make && ./app || echo failed`, `(cd build && make)`
- **Wildcards**: `*.c`, `src*`, `*/main.c`
- **Built-in commands**: `echo` (with `-n`), `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- **Keyboard signals like bash**: Ctrl-C gives a fresh prompt, Ctrl-D exits, Ctrl-\ is ignored at the prompt
- **History**: arrow keys recall earlier commands

## Demo

A real session, copied from the terminal:

```console
minishell> echo "Hello, $USER" | tr a-z A-Z
HELLO, RASIOL
minishell> export GREETING="hi there"
minishell> echo $GREETING > out.txt && cat out.txt
hi there
minishell> ls nope || echo "ls failed with status $?"
ls: cannot access 'nope': No such file or directory
ls failed with status 2
minishell> cat << EOF | wc -l
> one
> two
> EOF
2
minishell> (export INNER=1 && echo "inside: $INNER") && echo "outside: [$INNER]"
inside: 1
outside: []
minishell> echo *.txt
notes.txt out.txt todo.txt
minishell> echo '$HOME stays literal in single quotes'
$HOME stays literal in single quotes
minishell> unknown_cmd
minishell: unknown_cmd: command not found
minishell> echo $?
127
minishell> exit 3
exit
```

## Engineering highlights

The project came with strict rules, which made several ordinary problems
harder:

- Only 53 C library functions are allowed. There is no `strdup` or
  `strtok`, so we wrote our own string, list and formatting utilities.
- Each function is at most 25 lines, with at most 5 functions per file and
  no `for` loops.
- The program may use exactly one global variable, and it may only record
  which signal arrived.
- The program must not leak memory or crash, in the shell itself and in
  every process it starts.

A few of the problems we solved along the way:

**Heredocs inside pipelines.** In `cat << A | grep x | wc -l`, the user
types the heredoc text before anything runs. Our first version fed it
straight to the command. With several heredocs in one line, they read each
other's input, and a body larger than 64 KB froze the shell because nobody
was reading the pipe yet.
- The final design reads every heredoc of the line first, in order. Each
  body goes into its own private temporary file.
- The file is created with `O_EXCL`, so two shells can never share one.
- From then on, `<<` is just "read from this file", exactly like `<`.
- After the line finishes, the shell deletes only the files it created
  itself.

**Telling "exit" apart from an exit status.** `exit 3` has to stop the
shell, but a program that simply finished with status 3, or was killed by
a signal (status 137), must not.
- The shell keeps the two apart with a value outside the 0–255 range of
  real statuses.
- Inside a pipeline or a subshell, `exit` only ends that child process.
  This matches bash: `echo hi | exit` leaves your shell running.

**Signals with one global variable.** When you press Ctrl-C, the terminal
sends the signal to every process of the running command.
- Child processes get the default behaviour, so they stop.
- Only the main shell reacts: it prints one newline and one new prompt,
  even for long pipelines or nested subshells.
- The signal handler only stores the signal number. Everything else is
  decided in normal code, where it is safe to do so.

**Expanding at the right moment.** In `export X=1 && echo $X`, the value
of `$X` only exists once the first command has run. Variables and
wildcards are therefore expanded for each command right before it starts,
not when the line is parsed. That way `$?`, `export` and `cd` from earlier
in the same line are visible, as in bash.

**Cleaning up in every process.** Every forked child frees the entire
syntax tree and environment before it exits. Our valgrind harness checks
for leaks and open file descriptors in the shell and in each child it
starts.

## How it works

```text
 input line ── readline (prompt, line editing, history)
     │
     ▼
 Lexer ─────── splits the line into tokens: words, | && || < > << >> ( )
     │
     ▼
 Parser ────── recursive descent, builds a syntax tree (AST)
     │
     ▼
 Heredocs ──── every << body is read now and saved to a private temp file
     │
     ▼
 Executor ──── walks the tree
     ├── command   expand words → apply redirections → builtin or fork + execve
     ├── pipe      pipe() + fork one child per side, wire stdout → stdin
     ├── && / ||   run the right side depending on the left side's status
     └── ( … )     fork a subshell so cd/export inside don't leak out
```

The grammar the parser implements:

```text
line      := and_or
and_or    := pipeline ( ("&&" | "||") pipeline )*
pipeline  := command ( "|" command )*
command   := "(" and_or ")" redirect*
           | ( word | redirect )+
redirect  := ( "<" | ">" | ">>" | "<<" ) word
```

Builtins like `cd` and `export` run inside the shell process so they can
change its state. Inside a pipeline they run in a child, as in bash.

## Testing

Correctness was checked against bash itself, not just by hand:

- **113 comparison tests** (`tests/compare_bash.py`). Each test is a short
  script that runs in both minishell and bash; the output and exit code
  must match exactly. They cover quoting, expansion, redirections, pipes,
  builtins, exit codes, heredocs, `&&`/`||`, subshells and wildcards.
- **13 interactive tests** (`tests/pty_check.py`). These drive the shell
  through a real pseudo-terminal and press the keys a user would: Ctrl-C,
  Ctrl-D and Ctrl-\ at the prompt and inside running commands, Ctrl-C in
  the middle of a heredoc, and arrow-key history.
- **Memory and resource checks** (`tests/leaks.sh`). These run the shell
  under valgrind, following every child process. They fail on any
  definite leak, invalid memory access, crash or file descriptor left
  open.
- **Allowed-functions check** (`tests/check_functions.sh`). This inspects
  the compiled binary with `nm` and fails if it uses any C library
  function outside the allowed list.
- **Style**: the 42 style checker (`norminette`) reports 0 errors.

## Instructions

**Requirements:** Linux or macOS with `cc`, `make` and the GNU readline
development files (on Debian/Ubuntu: `sudo apt install libreadline-dev`).

```sh
git clone https://github.com/rasiolisufaj/minishell.git
cd minishell
make            # builds ./minishell
./minishell
```

Other build targets: `make bonus` (same features, binary `minishell_bonus`),
`make clean`, `make fclean`, `make re`.

**Running the tests** (also needs `bash`, `python3` 3.9 or newer and
`valgrind`):

```sh
python3 tests/compare_bash.py              # 113/113 passed
python3 tests/pty_check.py                 # 13/13 passed
tests/leaks.sh < tests/leak_baseline.txt   # ok   no leaks, ...
tests/check_functions.sh                   # ok   only allowed functions
python3 tests/compare_bash.py -k heredoc:  # run only matching tests
```

## Project structure

```text
minishell/
├── srcs/
│   ├── main/        prompt loop, startup, line processing
│   ├── lexer/       tokenizer: words, quotes, operators
│   ├── parser/      recursive-descent parser building the syntax tree
│   ├── expands/     $VAR, $?, quote removal, word splitting
│   ├── wildcards/   * pattern matching against directory entries
│   ├── executor/    pipes, redirections, heredocs, fork/execve, PATH lookup
│   ├── builtins/    echo, cd, pwd, export, unset, env, exit
│   └── utils/       signals and shared helpers
├── includes/        headers and data structures (tokens, tree nodes, shell state)
├── libft/           our own C library: strings, memory, lists, printf, get_next_line
├── tests/           bash comparison, terminal, valgrind and allowed-function checks
└── Makefile
```

## Scope and limitations

minishell covers the core of interactive bash, not all of it. It does not
support:

- `;` to separate commands
- command substitution: `$(...)` and backticks
- `${VAR}`
- arithmetic
- functions and aliases
- job control: Ctrl-Z, `fg`, `bg`
- numbered redirections like `2>`
- `?` and `[...]` in wildcards

## Team

minishell was built by two developers:

- **Rasiol Isufaj** worked mainly on the lexer, the parser and syntax
  tree, the main loop, and shared utilities. Rasiol also contributed to
  the builtins and the executor.
- **Idriss Taharbouche** worked mainly on variable expansion, wildcards,
  and most of the executor and builtins.

We worked on separate git branches and merged them, which meant agreeing
early on the data structures (tokens, tree nodes, environment) that connect
our parts.

## Resources

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html):
  the behaviour we compared against
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html):
  grammar, quoting and expansion rules
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/readline.html)
- Linux man pages: [fork(2)](https://man7.org/linux/man-pages/man2/fork.2.html),
  [execve(2)](https://man7.org/linux/man-pages/man2/execve.2.html),
  [pipe(2)](https://man7.org/linux/man-pages/man2/pipe.2.html),
  [dup2(2)](https://man7.org/linux/man-pages/man2/dup2.2.html),
  [waitpid(2)](https://man7.org/linux/man-pages/man2/waitpid.2.html),
  [signal(7)](https://man7.org/linux/man-pages/man7/signal.7.html)

### How AI was used

After the shell was written, we used Claude (Anthropic) for a review pass.
It checked the project against its requirements and wrote the test tools
in `tests/`: the bash comparison, the terminal and signal tests, the
valgrind checks and the allowed-function check. It also drafted fixes for
the bugs those tests found:

- heredocs inside pipelines
- exit-status handling and the global variable
- signal handling while commands run
- splitting of whitespace-only variables
- `cd` updating `PWD`/`OLDPWD`
- starting with an empty environment
- wildcard leaks

We reviewed and tested every change and can explain all of it.
