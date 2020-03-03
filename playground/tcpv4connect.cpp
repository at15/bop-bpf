
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>

#include <bcc/BPF.h>

const std::string BPF_PROGRAM = R"(
#include <uapi/linux/ptrace.h>
#include <net/sock.h>
#include <bcc/proto.h>
BPF_HASH(currsock, u32, struct sock *);
int on_tcp_v4_connect(struct pt_regs *ctx, struct sock *sk)
{
	u32 pid = bpf_get_current_pid_tgid();
	// stash the sock ptr for lookup on return
	currsock.update(&pid, &sk);
	return 0;
};
int on_ret_tcp_v4_connect(struct pt_regs *ctx)
{
	int ret = PT_REGS_RC(ctx);
	u32 pid = bpf_get_current_pid_tgid();
	struct sock **skpp;
	skpp = currsock.lookup(&pid);
	if (skpp == 0) {
		return 0;	// missed entry
	}
	if (ret != 0) {
		// failed to send SYNC packet, may not have populated
		// socket __sk_common.{skc_rcv_saddr, ...}
		currsock.delete(&pid);
		return 0;
	}
	// pull in details
	struct sock *skp = *skpp;
	u32 saddr = skp->__sk_common.skc_rcv_saddr;
	u32 daddr = skp->__sk_common.skc_daddr;
	u16 dport = skp->__sk_common.skc_dport;
	// output
	bpf_trace_printk("trace_tcp4connect %x %x %d\n", saddr, daddr, ntohs(dport));
	currsock.delete(&pid);
	return 0;
}
)";

// clang++-8 -I /usr/include/bcc/compat -lbcc --std=c++11 tcpv4connect.cpp
int main() {
  ebpf::BPF bpf;
  auto init_res = bpf.init(BPF_PROGRAM);
  if (init_res.code() != 0) {
    std::cerr << init_res.msg() << std::endl;
    return 1;
  }

  std::ifstream pipe("/sys/kernel/debug/tracing/trace_pipe");
  std::string line;
// TODO: does cpp requires attach kprobe manually? the kprobe___ magic didn't work
// std::string probe_fnname = bpf.get_syscall_fnname("tcp_v4_connect");
// std::cout << probe_fnname << std::endl;
// kprobe seems can be found on cat /proc/kallsyms | grep tcp
  auto attach_res = bpf.attach_kprobe("tcp_v4_connect", "on_tcp_v4_connect");
  if (attach_res.code() != 0) {
    std::cerr << attach_res.msg() << std::endl;
    return 1;
  }
  std::cout << "attached" << std::endl;
  // TODO: cpp doc should be updated
  attach_res = bpf.attach_kprobe("tcp_v4_connect", "on_ret_tcp_v4_connect", 0, BPF_PROBE_RETURN);
  if (attach_res.code() != 0) {
    std::cerr << attach_res.msg() << std::endl;
    return 1;
  }
  std::cout << "attached return" << std::endl;

  while (true) {
    // FIXME: somehow I can't read from debug?
    // FIXED: I didn't use \n when using cpp raw string literal, python is not using \n
    if (std::getline(pipe, line)) {
      std::cout << line << std::endl;
    //   break;
    } else {
      std::cout << "Waiting for a tcp_v4_connect" << std::endl;
      sleep(1);
    }
  }

  return 0;
}