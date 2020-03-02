# BCC

## Install

Based on https://github.com/iovisor/bcc/issues/2119 need to use ppa instead of official one to avoid

```text
./arch/x86/include/asm/jump_label.h:23:2: error: 'asm goto' constructs are not supported yet
```

```bash
sudo apt remove bpfcc-tools
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 4052245BD4284CDD
echo "deb https://repo.iovisor.org/apt/$(lsb_release -cs) $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/iovisor.list
sudo apt-get update
sudo apt-get install bcc-tools libbcc-examples linux-headers-$(uname -r)
```

Run tools shipped with bcc

```bash
/usr/share/bcc/tools/opensnoop
/usr/share/bcc/tools/funccount tcp_drop
```

Check installed

```bash
apt list --installed | grep bcc
```