#!/usr/bin/env python3

import atexit
import ctypes
import sys
import termios
import os

import vmcall

MINUS_ONE = ctypes.c_ulong(-1).value


def vmcall_dbgsh(ch):
    func = vmcall.get_function('dbgsh')
    if func == 0:
        print("Failed to get dbgsh function")
        sys.exit(1)
    arg = vmcall.call_vmm_arg_t(rax=func, rbx=ch)
    ret = vmcall.vmcall(arg)
    return ret.rax


def dbgsh():
    vmcall_dbgsh(MINUS_ONE)
    s = MINUS_ONE
    while True:
        r = ctypes.c_long(vmcall_dbgsh(s)).value
        if r == (0x100 | ord('\n')):
            vmcall_dbgsh(0)
            break

        s = MINUS_ONE
        if r == 0:
            ch = sys.stdin.read(1)
            s = 0x100 if len(ch) == 0 else ord(ch)
        elif r > 0:
            r &= 0xff
            sys.stdout.write(chr(r))
            sys.stdout.flush()
            s = 0


def disable_echo():
    fd = sys.stdin.fileno()
    attr = termios.tcgetattr(fd)
    new_attr = attr
    new_attr[3] = new_attr[3] & ~termios.ICANON & ~termios.ECHO
    termios.tcsetattr(fd, termios.TCSADRAIN, new_attr)
    return fd, attr


def restore_attr(fd, attr):
    termios.tcsetattr(fd, termios.TCSADRAIN, attr)


def main():
    fd, attr = disable_echo()
    atexit.register(restore_attr, fd, attr)
    dbgsh()


if __name__ == '__main__':
    main()
