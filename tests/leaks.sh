#!/bin/sh
# Run minishell under valgrind with the commands given on stdin, in an empty
# temp dir and a minimal environment (no PWD/OLDPWD).
# Fails on: definite leaks, invalid memory accesses or segfaults (in the shell
# and in every process it forks, except /usr and /bin programs), and on file
# descriptors other than 0-2 still open at exit.
#
# Usage: tests/leaks.sh [path/to/minishell] < commands.txt
BIN=$(realpath "${1:-./minishell}")
SUPP=$(realpath "$(dirname "$0")/../readline.supp")
DIR=$(mktemp -d)
cat > "$DIR/commands"
cd "$DIR" || exit 1
touch a.txt b.txt c.log && mkdir sub && touch sub/x.c
env -i PATH=/usr/bin:/bin HOME="$DIR" USER=tester \
	valgrind --leak-check=full --errors-for-leak-kinds=definite \
	--track-fds=yes --trace-children=yes \
	--trace-children-skip='/usr/*,/bin/*' --suppressions="$SUPP" \
	--log-file="$DIR/vg.%p.log" "$BIN" < "$DIR/commands" > /dev/null 2>&1
bad=0
if grep -qE "ERROR SUMMARY: [1-9]|SIGSEGV" "$DIR"/vg.*.log; then
	grep -hE "ERROR SUMMARY: [1-9]|SIGSEGV|definitely lost:|Invalid" \
		"$DIR"/vg.*.log
	bad=1
fi
# fds 0-2 are fine; valgrind's own log file is inherited by children, ignore it
if grep -hE "Open file descriptor ([3-9]|[1-9][0-9]+):" "$DIR"/vg.*.log \
	| grep -v '/vg\.[0-9]*\.log$'; then
	bad=1
fi
if [ "$bad" -eq 0 ]; then
	echo "ok   no leaks, no invalid accesses, no leaked fds"
else
	echo "FAIL valgrind logs: $DIR/vg.*.log"
fi
exit "$bad"
