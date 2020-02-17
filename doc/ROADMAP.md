# Roadmap

## Upcoming

### v0.0.1

Allow tracking simple tcp client and server w/o snooping packet for normal process (i.e. not container).

- no packet snooping and packet drop, leave it for v0.0.2
- no trace analysis and de duplication
- dump trace out i.e. not simply printing it out, have bpf code and user space code communicate (via bpf map I suppose?)