*This project has been created as part of the 42 curriculum by raisufaj, itaharbo.*

# minishell

## Description

minishell is a small Unix shell written in C, with bash as the reference for
its behaviour. The goal of the project is to understand how a shell works
underneath: reading and parsing a command line, creating processes, wiring
file descriptors together and handling signals.

What it supports:

- an interactive prompt with history (GNU readline)
- running programs found through `PATH`, or given by a relative or absolute path
- single quotes, double quotes, `$VAR` and `$?` expansion
- redirections `<`, `>`, `>>` and heredocs `<<` (with `$VAR` expansion unless
  the delimiter is quoted)
- pipes `|`
- `Ctrl-C`, `Ctrl-D` and `Ctrl-\` behaving like in bash
- the builtins `echo -n`, `cd`, `pwd`, `export`, `unset`, `env` and `exit`
- bonus: `&&`, `||`, parentheses for priority, and `*` wildcards in the
  current directory

How it works, in order: the lexer splits the line into tokens, the parser
builds a tree (commands, pipes, `&&`/`||`, groups), every heredoc of the line
is read into a temporary file, and the executor walks the tree. Each command's
words are expanded (variables, `$?`, wildcards) right before it runs.

## Instructions

Requirements: Linux or macOS, `cc`, `make` and the GNU readline development
files (`sudo apt install libreadline-dev` on Debian/Ubuntu).

```sh
make            # build ./minishell
make bonus      # build ./minishell_bonus (same features)
make clean      # remove object files
make fclean     # remove object files and binaries
make re         # rebuild from scratch
./minishell
```

Tests (need `bash`, `python3` and `valgrind`):

```sh
python3 tests/compare_bash.py      # same stdout and exit code as bash
python3 tests/pty_check.py         # signals, Ctrl-D and heredocs in a terminal
tests/leaks.sh < tests/leak_baseline.txt  # valgrind: leaks, bad accesses, fds
tests/check_functions.sh           # only functions allowed by the subject
```

## Resources

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/readline.html)
- Linux man pages: [fork(2)](https://man7.org/linux/man-pages/man2/fork.2.html),
  [execve(2)](https://man7.org/linux/man-pages/man2/execve.2.html),
  [pipe(2)](https://man7.org/linux/man-pages/man2/pipe.2.html),
  [dup2(2)](https://man7.org/linux/man-pages/man2/dup2.2.html),
  [waitpid(2)](https://man7.org/linux/man-pages/man2/waitpid.2.html),
  [signal(7)](https://man7.org/linux/man-pages/man7/signal.7.html)

### How AI was used

Claude (Anthropic) was used to review the project against the subject and to
write the test scripts in `tests/` (comparison with bash, terminal and signal
checks, valgrind checks, allowed-function check). It also drafted fixes for
the bugs those tests found: heredocs inside pipelines, exit-status handling
and the global variable, signal handling while commands run, splitting of
whitespace-only variables, `cd` updating `PWD`/`OLDPWD`, starting with an
empty environment, and wildcard leaks. We reviewed, tested and can explain
every change.
