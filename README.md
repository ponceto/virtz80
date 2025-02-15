# Z80 CPU EMULATOR

A highly portable (I hope) Z80 emulator written in C++.

## DESCRIPTION

This project is designed to emulate the venerable [Z80 CPU](https://en.wikipedia.org/wiki/Zilog_Z80) produced by [Zilog](https://en.wikipedia.org/wiki/Zilog) between July 1976 and June 2024.

## THE Z80 EMULATOR

The Z80 emulation core was extracted from [Xcpc](https://www.xcpc-emulator.net/), my Amstrad CPC 464/664/6128 emulator for Linux, BSD and UNIX.

The Z80 emulation core supports all documented and undocumented instructions and flags and passes `zexall` test suite.

## THE VIRTUAL MACHINE

This project comes with a simple virtual machine « compatible » with the one provided by the [MAME project](https://github.com/mamedev/mame/tree/master/src/zexall).

Specifications:

```
CPU: 1 x Z80 CPU
RAM: 4 x 16kB
MMU: 1 x Virtual MMU (Memory Management Unit)
PMU: 1 x Virtual PMU (Peripheral Management Unit)
I/O: 1 x Virtual Serial Output (memory mapped)
```

By default, the virtual machine loads and runs the `zexall` test suite available in the `assets` folder.

## HOW TO BUILD

### Install the dependencies

Under Debian and derivatives (Ubuntu, Mint, ...):

```
apt-get install build-essential
```

In order to build the [WASM](https://en.wikipedia.org/wiki/WebAssembly) version, you must install [Emscripten](https://emscripten.org/) on your system.

### Configure the project

The project is preconfigured for a `little endian` target architecture (least significant byte first, eg. `x86_64`).

If your target architecture is `big endian` (most significant byte first, e.g. `Sparc`), you must replace `-DLSB_FIRST` with `-DMSB_FIRST` in the Makefile.

In case of misconfigured endianess, you will get the following error message at startup:

```
E	the endianess was not set correctly
```

### Build the project

To build the project, simply type:

```
make
```

You can also use the `-j` option if you want to build in parallel with `GNU make`:

```
make -j{number-of-jobs}
```

To build the WASM version, please use `Makefile.wasm`:

```
make -f Makefile.wasm
```

### Clean the project

To clean the project, simply type:

```
make clean
```

To clean the WASM version, please use `Makefile.wasm`:

```
make -f Makefile.wasm clean
```

## HOW TO RUN

The project comes with a simple virtual machine which is able to run the `zexall` test suite.

### Usage

Command line options:

```
Usage: virtz80.bin [OPTIONS...]

Options:

  -h, --help                    display this help and exit

  --bank0={filename}            specifies the ram bank #0 (16kB)
  --bank1={filename}            specifies the ram bank #1 (16kB)
  --bank2={filename}            specifies the ram bank #2 (16kB)
  --bank3={filename}            specifies the ram bank #3 (16kB)

Values:

  Bank #0: assets/bank0.rom
  Bank #1: assets/bank1.rom
  Bank #2: assets/bank2.rom
  Bank #3: assets/bank3.rom

```

### Run

Run the virtual machine:

```
./virtz80.bin
```

Expected result:

```
Z80 instruction exerciser
<adc,sbc> hl,<bc,de,hl,sp>....  OK
add hl,<bc,de,hl,sp>..........  OK
add ix,<bc,de,ix,sp>..........  OK
add iy,<bc,de,iy,sp>..........  OK
aluop a,nn....................  OK
aluop a,<b,c,d,e,h,l,(hl),a>..  OK
aluop a,<ixh,ixl,iyh,iyl>.....  OK
aluop a,(<ix,iy>+1)...........  OK
bit n,(<ix,iy>+1).............  OK
bit n,<b,c,d,e,h,l,(hl),a>....  OK
cpd<r>........................  OK
cpi<r>........................  OK
<daa,cpl,scf,ccf>.............  OK
<inc,dec> a...................  OK
<inc,dec> b...................  OK
<inc,dec> bc..................  OK
<inc,dec> c...................  OK
<inc,dec> d...................  OK
<inc,dec> de..................  OK
<inc,dec> e...................  OK
<inc,dec> h...................  OK
<inc,dec> hl..................  OK
<inc,dec> ix..................  OK
<inc,dec> iy..................  OK
<inc,dec> l...................  OK
<inc,dec> (hl)................  OK
<inc,dec> sp..................  OK
<inc,dec> (<ix,iy>+1).........  OK
<inc,dec> ixh.................  OK
<inc,dec> ixl.................  OK
<inc,dec> iyh.................  OK
<inc,dec> iyl.................  OK
ld <bc,de>,(nnnn).............  OK
ld hl,(nnnn)..................  OK
ld sp,(nnnn)..................  OK
ld <ix,iy>,(nnnn).............  OK
ld (nnnn),<bc,de>.............  OK
ld (nnnn),hl..................  OK
ld (nnnn),sp..................  OK
ld (nnnn),<ix,iy>.............  OK
ld <bc,de,hl,sp>,nnnn.........  OK
ld <ix,iy>,nnnn...............  OK
ld a,<(bc),(de)>..............  OK
ld <b,c,d,e,h,l,(hl),a>,nn....  OK
ld (<ix,iy>+1),nn.............  OK
ld <b,c,d,e>,(<ix,iy>+1)......  OK
ld <h,l>,(<ix,iy>+1)..........  OK
ld a,(<ix,iy>+1)..............  OK
ld <ixh,ixl,iyh,iyl>,nn.......  OK
ld <bcdehla>,<bcdehla>........  OK
ld <bcdexya>,<bcdexya>........  OK
ld a,(nnnn) / ld (nnnn),a.....  OK
ldd<r> (1)....................  OK
ldd<r> (2)....................  OK
ldi<r> (1)....................  OK
ldi<r> (2)....................  OK
neg...........................  OK
<rrd,rld>.....................  OK
<rlca,rrca,rla,rra>...........  OK
shf/rot (<ix,iy>+1)...........  OK
shf/rot <b,c,d,e,h,l,(hl),a>..  OK
<set,res> n,<bcdehl(hl)a>.....  OK
<set,res> n,(<ix,iy>+1).......  OK
ld (<ix,iy>+1),<b,c,d,e>......  OK
ld (<ix,iy>+1),<h,l>..........  OK
ld (<ix,iy>+1),a..............  OK
ld (<bc,de>),a................  OK
Tests complete
```

### Run the WASM version

To run the WASM version, you can use the Python built-in http server:

```
python3 -m http.server
```

Then open your browser and point to [localhost/virtz80.html](http://127.0.0.1:8000/virtz80.html)

## LICENSING

### Z80 emulator

This project is released under the `GPLv2` license.

```
Z80 emulator - Copyright (c) 2001-2025 - Olivier Poncet

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
```

### Z80 instruction set exerciser

The Z80 instruction set exerciser is released under the `GPLv2` license.

```
Z80 instruction set exerciser - Copyright (c) 1994 - Frank D. Cringle

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published
by the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
```

### CP/M bdos emulation

The CP/M bdos emulation is released under the `GPLv2` license.

```
CP/M bdos emulation - Copyright (c) 2009 - Kevin Horton

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
```

