#include <pybind11/pybind11.h>
#include <string>

namespace py = pybind11;
typedef unsigned long long ull;


struct call_vmm_arg_t {
    call_vmm_arg_t(ull rax=0, ull rbx=0, ull rcx=0, ull rdx=0, ull rsi=0, ull rdi=0)
        : rax(rax), rbx(rbx), rcx(rcx), rdx(rdx), rsi(rsi), rdi(rdi) {}
    ull rax, rbx, rcx, rdx, rsi, rdi;
};

struct call_vmm_ret_t {
    call_vmm_ret_t(ull rax=0, ull rbx=0, ull rcx=0, ull rdx=0, ull rsi=0, ull rdi=0)
        : rax(rax), rbx(rbx), rcx(rcx), rdx(rdx), rsi(rsi), rdi(rdi) {}
    ull rax, rbx, rcx, rdx, rsi, rdi;
};

// TODO: AMD (vmmcall) support

call_vmm_ret_t vmcall(call_vmm_arg_t &arg){
    call_vmm_ret_t ret = {};

    asm volatile ("vmcall"
              : "=a" (ret.rax), "=b" (ret.rbx),
                "=c" (ret.rcx), "=d" (ret.rdx),
                "=S" (ret.rsi), "=D" (ret.rdi)
              : "a" (arg.rax), "b" (arg.rbx),
                "c" (arg.rcx), "d" (arg.rdx),
                "S" (arg.rsi), "D" (arg.rdi)
              : "memory");
    return ret;
}

ull get_function(const std::string &s){
    // XXX: We must ensure that string `s` is not swapped out
    //      but currently there is no guarantee
    call_vmm_arg_t arg = {};
    arg.rax = 0;
    arg.rbx = (ull)s.c_str();
    arg.rdi = 999;
    auto ret = vmcall(arg);
    return ret.rax;
}

PYBIND11_MODULE(vmcall, m) {
    m.doc() = "vmcall";
    m.def("vmcall", &vmcall, "do vmcall");
    m.def("get_function", &get_function, "get function");

    py::class_<call_vmm_arg_t>(m, "call_vmm_arg_t")
        .def(py::init<ull,ull,ull,ull,ull,ull>(),
             py::arg("rax") = 0, py::arg("rbx") = 0, py::arg("rcx") = 0,
             py::arg("rdx") = 0, py::arg("rsi") = 0, py::arg("rdi") = 0)
        .def_readwrite("rax", &call_vmm_arg_t::rax)
        .def_readwrite("rbx", &call_vmm_arg_t::rbx)
        .def_readwrite("rcx", &call_vmm_arg_t::rcx)
        .def_readwrite("rdx", &call_vmm_arg_t::rdx)
        .def_readwrite("rsi", &call_vmm_arg_t::rsi)
        .def_readwrite("rdi", &call_vmm_arg_t::rdi)
        .def("__repr__",
                [](const call_vmm_arg_t &arg){
                    return "(rax=" + std::to_string(arg.rax) + ", " +
                           " rbx=" + std::to_string(arg.rbx) + ", " +
                           " rcx=" + std::to_string(arg.rcx) + ", " +
                           " rdx=" + std::to_string(arg.rdx) + ", " +
                           " rsi=" + std::to_string(arg.rsi) + ", " +
                           " rdi=" + std::to_string(arg.rdi) + ")";
                });

    py::class_<call_vmm_ret_t>(m, "call_vmm_ret_t")
        .def_readonly("rax", &call_vmm_ret_t::rax)
        .def_readonly("rbx", &call_vmm_ret_t::rbx)
        .def_readonly("rcx", &call_vmm_ret_t::rcx)
        .def_readonly("rdx", &call_vmm_ret_t::rdx)
        .def_readonly("rsi", &call_vmm_ret_t::rsi)
        .def_readonly("rdi", &call_vmm_ret_t::rdi)
        .def("__repr__",
                [](const call_vmm_ret_t &ret){
                    return "(rax=" + std::to_string(ret.rax) + ", " +
                           " rbx=" + std::to_string(ret.rbx) + ", " +
                           " rcx=" + std::to_string(ret.rcx) + ", " +
                           " rdx=" + std::to_string(ret.rdx) + ", " +
                           " rsi=" + std::to_string(ret.rsi) + ", " +
                           " rdi=" + std::to_string(ret.rdi) + ")";
                });
}
