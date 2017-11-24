#!/usr/bin/env python3

import ctypes
import sys
import os

import vmcall


def vmcall_getstatus():
    func = vmcall.get_function('get_status')
    if func == 0:
        print("Failed to get get_status function")
        sys.exit(1)

    buflen = 65536
    buf = ctypes.create_string_buffer(buflen)

    # XXX: we must ensure that buf is not swapped out
    arg = vmcall.call_vmm_arg_t()
    arg.rax = func
    arg.rbx = ctypes.cast(ctypes.pointer(buf), ctypes.c_void_p).value
    arg.rcx = buflen
    ret = vmcall.vmcall(arg)

    if ret.rax != 0:
        print("get_status return error {}".format(r.rax))
        sys.exit(1)

    return buf.raw[:ret.rcx].decode()


def main():
    buf = vmcall_getstatus()
    print("{}".format(buf))


if __name__ == '__main__':
    main()
