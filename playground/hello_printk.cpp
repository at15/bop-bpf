#include <bcc/BPF.h>
#include <iostream>
#include <fstream>
#include <string>

// https://stackoverflow.com/questions/1135841/c-multiline-string-literal
// C++11 Raw String Literal
const std::string BPF_PROGRAM = R"CODE(
int kprobe__do_nanosleep()
{
    bpf_trace_printk("Hello, World!\\n");
    return 0;
}
)CODE";

// clang++-8 -I /usr/include/bcc/compat -lbcc --std=c++11 hello_printk.cpp
int main(void) {
  ebpf::BPF bpf;
  ebpf::StatusTuple res(0);

  res = bpf.init(BPF_PROGRAM);
  if (res.code() != 0) {
    std::cout << "error on init " << res.msg() << std::endl;
    return 1;
  }
  std::cout << "init!" << std::endl;

  std::ifstream pipe("/sys/kernel/debug/tracing/trace_pipe");
  std::string line;
  while (true) {
    std::cout << "Waiting to read from trace_pipe" << std::endl;
    // FIXME: it seems it can't read anything from the trace_pipe or because no kprobe is attached?
    // TODO: does the naming convention only works in python bcc?
    if (std::getline(pipe, line)) {
        std::cout << line << std::endl;
        std::cout << "Got line" << std::endl;
    } else {
        sleep(1);
    }
  }
  return 0;
}