#!/usr/bin/env python3
"""Drive ./minishell through a real terminal (pty): signals, Ctrl-D, history,
heredoc prompts. Each case sends keys with a short pause after each one, then
checks the terminal text.

  expect: substrings that must appear, in this order
  reject: substrings that must not appear
  once:   substrings that must appear exactly once
  rc:     if set, the shell must have exited with this code; if absent,
          the shell must still be running at the end

Usage: python3 tests/pty_check.py [-k name_substring] [path/to/minishell]
"""
import os
import pty
import re
import select
import shutil
import sys
import tempfile
import time

CASES = [
    dict(name="signal: ctrl-C at prompt sets $? to 130",
         keys=["abc", "\x03", "echo st=$?\n"], expect=["st=130"]),
    dict(name="signal: ctrl-\\ at prompt does nothing",
         keys=["\x1c", "echo still\n"], expect=["still"], reject=["Quit"]),
    dict(name="signal: ctrl-D at empty prompt exits",
         keys=["\x04"], expect=["exit"], rc=0),
    dict(name="signal: ctrl-C in cat",
         keys=["cat\n", "\x03", "echo st=$?\n"], expect=["st=130"]),
    dict(name="signal: ctrl-\\ in cat",
         keys=["cat\n", "\x1c", "echo st=$?\n"],
         expect=["Quit (core dumped)", "st=131"]),
    dict(name="history: up arrow recalls previous line",
         keys=["echo hist1\n", "\x1b[A", "\n"],
         expect=["echo hist1", "hist1", "echo hist1", "hist1"]),
    # --- task 5: one newline + one prompt after a signal
    dict(name="signal: ctrl-C in pipeline gives one prompt",
         keys=["sleep 5 | sleep 5\n", "\x03", "echo st=$?\n"],
         expect=["st=130"], reject=["minishell> minishell>", "\n\nminishell>"]),
    dict(name="signal: ctrl-C in cat | cat gives one prompt",
         keys=["cat | cat\n", "\x03", "echo st=$?\n"],
         expect=["st=130"], reject=["minishell> minishell>", "\n\nminishell>"]),
    dict(name="signal: ctrl-C in group gives one prompt",
         keys=["(sleep 5)\n", "\x03", "echo st=$?\n"],
         expect=["st=130"], reject=["minishell> minishell>", "\n\nminishell>"]),
    dict(name="signal: ctrl-\\ in cat | cat prints Quit once",
         keys=["cat | cat\n", "\x1c", "echo st=$?\n"],
         once=["Quit (core dumped)"], expect=["st=131"]),
    # --- task 6: heredoc in a terminal
    dict(name="heredoc: ctrl-C aborts the line, $?=130",
         keys=["cat << EOF\n", "line\n", "\x03", "echo st=$?\n"],
         expect=["st=130"], reject=["\nline\n"]),
    dict(name="heredoc: ctrl-C keeps a file named like the delimiter",
         keys=["touch keep\n", "cat << A << keep\n", "\x03", "ls keep\n"],
         expect=["ls keep\nkeep"]),
    dict(name="heredoc: ctrl-D warns and keeps what was typed",
         keys=["cat << EOF\n", "line\n", "\x04", "echo after\n"],
         expect=["here-document", "line", "after"]),
]

ANSI = re.compile(r"\x1b\[[0-9;?]*[A-Za-z]|\x1b[=>]")
PAUSE = 0.4


def session(ms, keys):
    d = tempfile.mkdtemp()
    pid, fd = pty.fork()
    if pid == 0:
        os.chdir(d)
        os.execve(ms, [ms], {"PATH": "/usr/bin:/bin", "HOME": d,
                             "USER": "tester", "TERM": "xterm", "PWD": d})
    out = b""

    def drain():
        nonlocal out
        end = time.time() + PAUSE
        while time.time() < end:
            if select.select([fd], [], [], 0.05)[0]:
                try:
                    chunk = os.read(fd, 4096)
                except OSError:
                    return
                if not chunk:
                    return
                out += chunk

    drain()
    for k in keys:
        os.write(fd, k.encode())
        drain()
    wpid, status = os.waitpid(pid, os.WNOHANG)
    if wpid == 0:
        os.kill(pid, 9)
        os.waitpid(pid, 0)
        rc = None
    else:
        rc = os.waitstatus_to_exitcode(status)
    os.close(fd)
    shutil.rmtree(d, ignore_errors=True)
    return ANSI.sub("", out.decode(errors="replace")).replace("\r", ""), rc


def check(case, text, rc):
    errors = []
    pos = 0
    for s in case.get("expect", []):
        i = text.find(s, pos)
        if i < 0:
            errors.append(f"missing (in order): {s!r}")
        else:
            pos = i + len(s)
    for s in case.get("reject", []):
        if s in text:
            errors.append(f"unexpected: {s!r}")
    for s in case.get("once", []):
        if text.count(s) != 1:
            errors.append(f"want exactly once, got {text.count(s)}: {s!r}")
    want = case.get("rc")
    if want is None and rc is not None:
        errors.append(f"shell exited with {rc} but should still run")
    if want is not None and rc != want:
        errors.append(f"shell should exit with {want}, got {rc}")
    return errors


def main():
    args = sys.argv[1:]
    key = None
    if args[:1] == ["-k"]:
        key, args = args[1], args[2:]
    ms = os.path.abspath(args[0] if args else "./minishell")
    ran = fails = 0
    for case in CASES:
        if key and key not in case["name"]:
            continue
        ran += 1
        text, rc = session(ms, case["keys"])
        errors = check(case, text, rc)
        if errors:
            fails += 1
            print(f"FAIL {case['name']}")
            for e in errors:
                print(f"     {e}")
            print("     terminal:\n       " + text.rstrip().replace("\n", "\n       "))
        else:
            print(f"ok   {case['name']}")
    print(f"\n{ran - fails}/{ran} passed")
    sys.exit(1 if fails or not ran else 0)


if __name__ == "__main__":
    main()
