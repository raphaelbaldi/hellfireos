# HellfireOS Realtime Operating System

---
### The operating system structure is organized as follows:

- app/ - applications
- arch/ - architecture specific device drivers and kernel bootstrapping
- drivers/ - general purpose device drivers
- fs/ - filesystem drivers
- lib/ - standard system / application libraries
- net/ - lightweight network stack
- platform/ - image building scripts for different platforms (application, kernel, architecture)
- sys/ - kernel core
- usr/ - simulators and documentation

### How to initialize the environment
```
$ source init_environment.sh
```

### Running the simulator
After initializing the environment and compiling your image, run:
```
$ usr/sim/hf_risc_sim/hf_risk_sim path/to/your/image.bin
```
