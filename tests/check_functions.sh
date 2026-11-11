#!/bin/sh
# Fails if the binary imports a C library function the subject does not allow.
# Symbols starting with "_" are compiler/libc internals and are skipped.
# Usage: tests/check_functions.sh [path/to/minishell]
ALLOWED="readline rl_clear_history rl_on_new_line rl_replace_line \
rl_redisplay add_history printf malloc free write access open read close \
fork wait waitpid wait3 wait4 signal sigaction sigemptyset sigaddset kill \
exit getcwd chdir stat lstat fstat unlink execve dup dup2 pipe opendir \
readdir closedir strerror perror isatty ttyname ttyslot ioctl getenv \
tcsetattr tcgetattr tgetent tgetflag tgetnum tgetstr tgoto tputs"
BIN=${1:-./minishell}
bad=0
for f in $(nm -u "$BIN" | awk '{print $2}' | sed 's/@.*//' | grep -v '^_'); do
	case " $ALLOWED " in
		*" $f "*) ;;
		*) echo "FORBIDDEN: $f"; bad=1 ;;
	esac
done
[ "$bad" -eq 0 ] && echo "ok   only allowed functions"
exit "$bad"
