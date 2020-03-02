#!/usr/bin/python3

from bcc import BPF
b = BPF(text="""
int kprobe__do_nanosleep()
{
    bpf_trace_printk("Hello, World!\\n");
    return 0;
}
""")
b.trace_print()