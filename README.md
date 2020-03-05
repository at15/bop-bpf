# bop-bpf

[Box Of Pain](https://github.com/disorderlylabs/box-of-pain) implementation using eBPF.

## Overview

This is a work in progress (i.e. README only) repo to implement (part of) box of pain using [eBPF](https://www.iovisor.org/technology/ebpf).
The main motivations of using eBPF over ptrace are

- [eBPF is widely adopted](http://www.brendangregg.com/blog/2019-01-01/learn-ebpf-tracing.html)
- the kernel part of code is [verified](https://github.com/torvalds/linux/blob/master/kernel/bpf/verifier.c)

The drawbacks are

- eBPF is mainly for tracing, so the fault injection is often not doable due to its read only nature.
  - it is not the case for network, BPF stands for Berkley Packet Filter so it is good at dropping packet.
- the logic inside eBPF is limited (due to the verifier) so complex logic still need to be implemented in user space.

## TODO

BOP specific

- [ ] compare with historical trace and only keep new trace (does this operation scale?)
- [ ] generate the graph and everything between the trace and the graph

BOP extension

- [ ] allow the tfj (tracer + fault injector) to provide an API for integrating with external system like [molly](https://github.com/palvaro/molly) so it can trace/inject dynamically and form a feedback control loop.
- [ ] collect and analysis traces across boxes

Env

- [ ] Vagrant environment so people w/o a linux box (hi Mac) can develop it
- [ ] cloud environment so it really works on cloud and people can run it with a browser
- [ ] check how ebpf work w/ container, i.e. do we only need to load a single ebpf code for the host (vm/metal), and how to distinguish different containers in the ebpf code

Lang

- BPF code can be written in a limited subset of C
- [ ] for user space language, currently I'd prefer Go because
  - it's easy to learn, faster to write, and cloud native
  - you can't reuse user space code inside bpf code
  - the go binding is being used in production so its performance should not be too bad

Trace

- [ ] keep record of tcp connection, accept, connect, read, write etc.
- [ ] snoop content

Fault injection

- [ ] drop packet
  - it seems using [AF_PACKET is not working](https://github.com/bpftools/linux-observability-with-bpf/issues/23), might need to switch to qdisc

Data

- [ ] bop has its own format, but I prefer using [protobuf](https://developers.google.com/protocol-buffers) (except inside kernel) so no logic is needed for serialization and works across language.
- [ ] a list of proto shard by app and time should be good. A dedicated database for tracing would be better, are there dedicated database for tracing, can we do compute inside database? Many people are using Cassandra, like they did for time series data, and they are wrong for [tsdb](https://xephonhq.github.io/awesome-time-series-database/)

## Related

- [disorderlylabs/box-of-pain](https://github.com/disorderlylabs/box-of-pain)
- [at15/ldfi-istio](https://github.com/at15/ldfi-istio)

## References

### eBPF

- https://github.com/zoidbergwill/awesome-ebpf

### Tracing

- [weaveworks/tcptracer-bpf](https://github.com/weaveworks/tcptracer-bpf) Use kprobes to traces TCP events
- [ntop/libebpfflow](https://github.com/ntop/libebpfflow) Container traffic visibility library based on eBPF
  - [nDPI](https://github.com/ntop/nDPI) deep packet inspection
- [iovisor/kubectl-trace](https://github.com/iovisor/kubectl-trace) schedule bpftrace program using kubectl

### Fault injection 

Code

- [trailofbits/ebpfault](https://github.com/trailofbits/ebpfault) A BPF-based syscall fault injector
- [cilium/chaos-testing-examples](https://github.com/cilium/chaos-testing-examples) Use Cillium (BPF + XDP based container network) for chaos testing and fault injection

Reading

- [BPF for security—and chaos—in Kubernetes](https://lwn.net/Articles/790684/)
- [Using eBPF in Kubernetes](https://kubernetes.io/blog/2017/12/using-ebpf-in-kubernetes/)

### Fault injection on k8s

- [pingcap/chaos-mesh](https://github.com/pingcap/chaos-mesh) A Chaos Engineering Platform for Kubernetes
  - [chaos-mesh/bpfki](https://github.com/chaos-mesh/bpfki) A BPF-based kernel fault injection service

## License

GPL v3