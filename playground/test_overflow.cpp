#include <bcc/BPF.h>
#include <iostream>

const std::string BPF_PROGRAM1 = R"(
BPF_TABLE("array", int, uint64_t, myarray, 1);
)";

// From https://lists.iovisor.org/g/iovisor-dev/topic/how_to_compile_c_programs/21386268
// g++ test_overflow.cpp -lbcc --std=c++11
// TODO: https://github.com/iovisor/bcc/issues/2301 -lbcc does not work ...
// g++ -I /usr/include/bcc/compat -lbcc --std=c++11 test_overflow.cpp
// clang++-8 -lbcc --std=c++11 test_overflow.cpp
// g++ test_overflow.cpp -M -lbcc --std=c++11
// 
// The only working command ...
// clang++-8 -I /usr/include/bcc/compat -lbcc --std=c++11 test_overflow.cpp
int main(void) {
  ebpf::BPF bpf1;
  ebpf::StatusTuple res(0);

  res = bpf1.init(BPF_PROGRAM1);
  if (res.code() != 0) {
    std::cout << "error on init " << std::endl;
    return -1;
  }
  std::cout << "init!" << std::endl;
  return 0;
}