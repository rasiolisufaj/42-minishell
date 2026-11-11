#!/usr/bin/env python3
"""Run every CASE through ./minishell and through bash, then compare stdout
and exit code. Each case is a script: one command per line, fed on stdin.

Usage: python3 tests/compare_bash.py [-k name_substring] [path/to/minishell]
"""
import os
import re
import shutil
import subprocess
import sys
import tempfile

CASES = [
    # --- echo, quotes, expansion
    ("echo: basic", "echo hello world"),
    ("echo: -n", "echo -n hi\necho"),
    ("echo: repeated -n", "echo -nnn -n hi\necho"),
    ("echo: -n then other option", "echo -n -x hi"),
    ("echo: empty args", "echo\necho ''\necho '' a"),
    ("quotes: single", "echo '$USER \"x\" | > <'"),
    ("quotes: double", "echo \"$USER '$USER' | >\""),
    ("quotes: concatenated", "echo \"a\"\"b\"'c'd"),
    ("quotes: spaces kept", "echo \"   spaced   out  \""),
    ("expand: adjacent vars", "echo $USER$HOME"),
    ("expand: lone dollar", "echo $\necho \"$\"\necho $+"),
    ("expand: unset var", "echo a$NOPE b\necho \"[$NOPE]\""),
    ("expand: $?", "ls /nonexist\necho $?\necho \"$?\""),
    ("expand: $? after not found", "nopecmd\necho $?"),
    ("split: var with spaces", "export A=\"a   b    c\"\necho $A\necho \"$A\""),
    ("split: var of spaces inside word", "export A=\"   \"\necho [$A]"),
    ("expand: var as command", "export L=\"echo hi\"\n$L"),
    ("expand: empty var as command", "$NOPE\necho $?"),
    ("expand: empty string as command", "\"\"\necho $?"),
    # --- redirections
    ("redir: out", "echo hi > f\ncat f"),
    ("redir: append", "echo a > f\necho b >> f\ncat f"),
    ("redir: in", "echo data > f\n< f cat\ncat < f"),
    ("redir: missing input", "cat < nofile\necho $?"),
    ("redir: two outputs", "echo hi > f1 > f2\ncat f1\necho --\ncat f2"),
    ("redir: before command", "> f echo x\ncat f"),
    ("redir: in and out", "echo q > f\ncat < f > g\ncat g"),
    ("redir: no permission", "touch ro\nchmod 000 ro\necho x > ro\necho $?"),
    ("redir: failure stops command", "cat < nofile > out\nls out\necho $?"),
    ("redir: target expands to nothing", "echo hi > $NOPE\necho $?"),
    # --- pipes
    ("pipe: simple", "echo hi | cat"),
    ("pipe: chain", "echo hi | cat | cat | cat | wc -c"),
    ("pipe: ls | wc", "ls | wc -l"),
    ("pipe: grep", "printf 'a\\nb\\na\\n' | grep a | wc -l"),
    ("pipe: status of last command",
     "ls /nonexist | cat\necho $?\ncat /dev/null | ls /nonexist\necho $?"),
    ("pipe: exit on the left", "exit 3 | echo ok\necho alive $?"),
    ("pipe: yes | head", "yes | head -3"),
    ("pipe: into redirect", "echo hi | cat > f\ncat f"),
    ("pipe: env builtin", "export Z=9\nenv | grep ^Z="),
    # --- builtins
    ("export: and unset", "export A=1\necho $A\nunset A\necho [$A]"),
    ("export: without value", "export A\nenv | grep -c ^A\nexport | grep 'A$'"),
    ("export: several", "export B=2 C=3\necho $B$C"),
    ("export: invalid names", "export 1A=x\necho $?\nexport =x\necho $?"),
    ("export: overwrite", "export A=1\nexport A=2\necho $A"),
    ("export: = inside value", "export A=x=y\necho $A"),
    ("unset: PATH", "unset PATH\nls\necho $?"),
    ("cd: absolute", "cd /tmp\npwd"),
    ("cd: missing dir", "cd /nonexist\necho $?"),
    ("cd: file", "touch f\ncd f\necho $?"),
    ("cd: no argument goes HOME", "cd\npwd"),
    ("cd: too many args", "cd /tmp /usr\necho $?"),
    ("pwd", "pwd"),
    ("exit: plain", "exit"),
    ("exit: 42", "exit 42"),
    ("exit: -1", "exit -1"),
    ("exit: 256", "exit 256"),
    ("exit: not numeric", "exit abc"),
    ("exit: too big", "exit 99999999999999999999"),
    ("exit: keeps last status", "ls /nonexist\nexit"),
    # --- command lookup
    ("path: absolute", "/bin/echo abs"),
    ("path: relative", "cp /bin/echo ./myecho\n./myecho rel"),
    ("path: directory", "/tmp\necho $?"),
    ("path: not executable", "touch noexec\n./noexec\necho $?"),
    ("path: missing relative", "./nope\necho $?"),
    ("path: external exit 1", "sh -c 'exit 1'\necho alive $?"),
    # --- heredoc
    ("heredoc: basic", "cat << EOF\nhello\nworld\nEOF"),
    ("heredoc: quoted delimiter", "cat << 'EOF'\n$USER\nEOF"),
    ("heredoc: several, last wins", "cat << A << B\nfirst\nA\nsecond\nB"),
    ("heredoc: with output redirect", "cat << EOF > f\nx\nEOF\ncat f"),
    # --- bonus
    ("bonus: &&", "true && echo ok\nfalse && echo no\necho $?"),
    ("bonus: ||", "false || echo ok\ntrue || echo no"),
    ("bonus: && ||", "false && echo no || echo yes"),
    ("bonus: group in pipe", "(echo a && echo b) | cat"),
    ("bonus: group is a subshell", "(cd /tmp)\npwd"),
    ("bonus: group redirect", "(echo hi) > f\ncat f"),
    ("bonus: wildcard all", "echo *"),
    ("bonus: wildcard suffix", "echo *.txt"),
    ("bonus: wildcard quoted", "echo \"*\" '*'"),
    ("bonus: wildcard no match", "echo *.zzz"),
    ("bonus: wildcard middle", "echo a*t"),
    # --- task 2: whitespace-only expansion
    ("split: var of only spaces before word",
     "export A=\"   \"\necho $A x\necho done"),
    ("split: var of only spaces as command",
     "export A=\"   \"\n$A\necho st=$?"),
    ("split: leading and trailing spaces", "export A=\"  a  b  \"\necho [$A]"),
    # --- task 3: echo and cd
    ("echo: lone dash is not an option", "echo -\necho - x\necho -n -"),
    ("cd: sets PWD and OLDPWD when missing",
     "cd /tmp\necho $PWD\ncd /usr\necho $OLDPWD"),
    # --- task 4: exit status handling
    ("exit: in pipe does not quit shell", "echo hi | exit 3\necho alive $?"),
    ("exit: in group does not quit shell", "(exit 3)\necho alive $?"),
    ("exit: external status 255 does not quit",
     "sh -c 'exit 255'\necho alive $?"),
    ("exit: killed child status", "sh -c 'kill -9 $$'\necho alive $?"),
    ("exit: killed child in group", "(sh -c 'kill -9 $$')\necho alive $?"),
    ("exit: after && quits", "true && exit 4\necho never"),
    ("exit: before || quits", "exit 0 || echo never"),
    ("exit: no argument uses status from same line", "ls /nonexist || exit"),
    # --- task 6: heredocs
    ("heredoc: expands variables", "cat << EOF\n$USER is $NOPE.\nEOF"),
    ("heredoc: expands $?", "ls /nonexist\ncat << EOF\nst=$?\nEOF"),
    ("heredoc: quotes in delimiter disable expansion",
     "cat << 'EOF'\n$USER\nEOF\ncat << E\"O\"F\n$USER\nEOF"),
    ("heredoc: delimiter is not expanded", "cat << $USER\nx\n$USER"),
    ("heredoc: quotes in body are literal", "cat << EOF\n'$USER' \"q\"\nEOF"),
    ("heredoc: left of pipe", "cat << EOF | wc -l\na\nb\nEOF"),
    ("heredoc: right of pipe", "echo piped | cat << EOF\nfromdoc\nEOF"),
    ("heredoc: one on each pipe side", "cat << A | cat << B\naaa\nA\nbbb\nB"),
    ("heredoc: three-stage pipe", "cat << A | cat | cat << B\n1\nA\n2\nB"),
    ("heredoc: without command", "<< EOF\nx\nEOF\necho st=$?"),
    ("heredoc: on a group", "(cat) << EOF\nin group\nEOF\necho alive"),
    ("heredoc: failing redirect after it",
     "cat << EOF > /nonexist/f\nx\nEOF\necho st=$?"),
    ("heredoc: body larger than a pipe buffer",
     "cat << EOF | wc -c\n" + "0123456789\n" * 7000 + "EOF"),
    ("heredoc: temp files are removed",
     "cat << EOF\nx\nEOF\nls -a /tmp | grep -c '^.minishell_hd_'"),
    # --- task 7: expansion happens right before each command runs
    ("bonus: $? updated before ||", "ls /nonexist || echo st=$?"),
    ("bonus: $? inside group", "(false || echo st=$?)"),
    ("bonus: export visible later on the line", "export X=1 && echo [$X]"),
    ("bonus: wildcard after cd on the line", "cd sub && echo *"),
    # --- task 8: wildcard redirects and */ patterns
    ("bonus: ambiguous wildcard redirect fails",
     "echo hi > *.txt\necho st=$?\nls"),
    ("bonus: single-match wildcard redirect", "echo hi > *.log\ncat c.log"),
    ("bonus: */ pattern", "echo */x.c"),
]

PROMPT_ECHO = re.compile(r"minishell> [^\n]*\n?")
HEREDOC_ECHO = re.compile(r"^> [^\n]*\n?", re.M)


def setup(d):
    for name in ("a.txt", "b.txt", "c.log", "art", ".hidden"):
        open(os.path.join(d, name), "w").close()
    os.mkdir(os.path.join(d, "home"))
    os.mkdir(os.path.join(d, "sub"))
    open(os.path.join(d, "sub", "x.c"), "w").close()


def run(argv, script, d):
    env = {"PATH": "/usr/bin:/bin", "HOME": os.path.join(d, "home"),
           "USER": "tester", "LANG": "C", "TERM": "dumb"}
    try:
        p = subprocess.run(argv, input=script + "\n", cwd=d, env=env,
                           capture_output=True, text=True, timeout=10)
        return p.stdout, p.stderr, p.returncode
    except subprocess.TimeoutExpired:
        return "", "", "TIMEOUT"


def clean_minishell(out):
    # readline echoes "minishell> <cmd>" and "> <line>" when stdin is a pipe
    out = HEREDOC_ECHO.sub("", PROMPT_ECHO.sub("", out))
    return out[:-5] if out.endswith("exit\n") else out


def main():
    args = sys.argv[1:]
    key = None
    if args[:1] == ["-k"]:
        key, args = args[1], args[2:]
    ms = os.path.abspath(args[0] if args else "./minishell")
    ran = fails = 0
    for name, script in CASES:
        if key and key not in name:
            continue
        ran += 1
        d1, d2 = tempfile.mkdtemp(), tempfile.mkdtemp()
        setup(d1)
        setup(d2)
        mo, me, mr = run([ms], script, d1)
        bo, be, br = run(["bash"], script, d2)
        mo = clean_minishell(mo).replace(d1, "<D>")
        bo = bo.replace(d2, "<D>")
        if (mo, mr) == (bo, br):
            print(f"ok   {name}")
        else:
            fails += 1
            print(f"FAIL {name}\n     script: {script[:200]!r}\n"
                  f"     minishell rc={mr} out={mo[:300]!r} err={me.strip()[:300]!r}\n"
                  f"     bash      rc={br} out={bo[:300]!r} err={be.strip()[:300]!r}")
        shutil.rmtree(d1, ignore_errors=True)
        shutil.rmtree(d2, ignore_errors=True)
    print(f"\n{ran - fails}/{ran} passed")
    sys.exit(1 if fails or not ran else 0)


if __name__ == "__main__":
    main()
