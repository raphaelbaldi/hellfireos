#!/bin/bash

# Get precompiled cross compiler
filename="gcc-4.6.1.tar.gz"
fileid="0B0wA1DmGN-2wZHkxSTgxS0ZaT0U"
curl -L -o ${filename} "https://drive.google.com/uc?export=download&id=${fileid}"

# Extract it
tar -zxvf gcc-4.6.1.tar.gz

# Remove useless file
rm gcc-4.6.1.tar.gz

# Setup PATH to include the newly downloaded cross compiler
export PATH=$PATH:./gcc-4.6.1/bin

# Check that mips compiler is accessible
mips-elf-gcc -v

# Compile the simulator
cd usr/sim/hf_risc_sim
gcc -O2 hf_risc_sim.c -o hf_risk_sim

# Get out of there
cd ../../../

