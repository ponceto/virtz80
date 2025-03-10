# Z80 CPU EMULATOR

A highly portable (I hope) Z80 emulator written in C++.

## DESCRIPTION

This project is designed to emulate the venerable [Z80 CPU](https://en.wikipedia.org/wiki/Zilog_Z80) produced by [Zilog](https://en.wikipedia.org/wiki/Zilog) between July 1976 and June 2024.

## THE Z80 EMULATOR

The Z80 emulator was extracted from [Xcpc](https://www.xcpc-emulator.net/), my Amstrad CPC 464/664/6128 emulator for Linux, BSD and UNIX.

The Z80 emulation core supports all documented and undocumented instructions and flags and passes the well known `zexall` and `zexdoc` test suites.

## THE VIRTUAL MACHINE

This project comes with a simple virtual machine to play with the Z80 emulation core.

This virtual machine is quite « compatible » with:

  - the one provided by the [MAME project](https://github.com/mamedev/mame/tree/master/src/zexall) for zexall testing purpose.
  - the [Grant Searle Z80 computer](http://searle.x10host.com/z80/SimpleZ80.html), a fully operational Z80 computer running Microsoft BASIC.
  - the [RC2014](https://rc2014.co.uk/) simple 8 bit Z80 based modular computer, originally built to run Microsoft BASIC.

Specifications:

```
CPU: 1 x Z80 CPU
RAM: 4 x 16kB
MMU: 1 x Virtual MMU (Memory Management Unit)
VDU: 1 x Virtual VDU (Video Display Unit)
SIO: 2 x Virtual SIO (Serial Input/Output)
```

Notes:

  - The `CPU` is clocked at `7.372800Mhz`, just like a standard [RC2014](https://rc2014.co.uk/) board.
  - The `VDU` is clocked at `4.134375Mhz`, emulating a virtual 60Hz display (only used for real-time synchronization purpose).
  - The `SIO` are clocked at `115.200KHz`, emulating two MC6850 ACIA (Asynchronous Communications Interface Adapter) for serial Input/Output.

By default, the virtual machine will load and runs the `zexall` test suite available in the `assets` folder, but you can also run the Microsoft BASIC or the Small Computer Monitor.

## HOW TO BUILD

### Install the dependencies

Under Debian and derivatives (Ubuntu, Mint, ...):

```
apt-get install build-essential
```

In order to build the [WASM](https://en.wikipedia.org/wiki/WebAssembly) version, you must install [Emscripten](https://emscripten.org/) on your system.

### Configure the project

The project is preconfigured for a `little endian` host architecture (least significant byte first, eg. `x86_64`).

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

The project comes with a simple virtual machine which is able to run the Z80 instruction set exerciser, the Microsoft BASIC and the Small Computer Monitor.

### Usage

Command line options:

```
Usage: virtz80.bin [OPTIONS...] APP

Options:

  -h, --help                    display this help and exit
  -v, --verbose                 verbose mode
  -q, --quiet                   quiet mode

  --turbo                       run the emulation at maximum speed
  --bank0={filename}            specifies the ram bank #0 (16kB)
  --bank1={filename}            specifies the ram bank #1 (16kB)
  --bank2={filename}            specifies the ram bank #2 (16kB)
  --bank3={filename}            specifies the ram bank #3 (16kB)

Applications:

  zexall                        run the Zexall test suite
  zexdoc                        run the Zexdoc test suite
  basic                         run the Microsoft BASIC
  monitor                       run the Small Computer Monitor

```

### How to run the Z80 instruction set exerciser

The virtual machine is able to run the Zexall and Zexdoc Z80 instruction set exerciser.

Run the virtual machine with or without the `zexall` or `zexdoc` argument.

It is strongly recommended to pass the `--turbo` option as the Zexall and Zexdoc tests can run for a very long time.

```
./virtz80.bin --turbo
```

or

```
./virtz80.bin --turbo zexall
```

or

```
./virtz80.bin --turbo zexdoc
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

### How to run the Microsoft BASIC

The virtual machine is able to run the Microsoft BASIC 4.7 from the original [Nascom 2](https://en.wikipedia.org/wiki/Nascom) which was adapted by Grant Searle for his [simple Z80 board](http://searle.x10host.com/z80/SimpleZ80.html).

Run the virtual machine with the `basic` argument.

```
./virtz80.bin basic
```

Expected result:

```
Z80 SBC By Grant Searle

Memory top? 

Z80 BASIC Ver 4.7b
Copyright (C) 1978 by Microsoft
32382 Bytes free
Ok
```

### How to run the Small Computer Monitor

The virtual machine is able to run the [Small Computer Monitor](https://smallcomputercentral.com/small-computer-monitor/) written by Stephen C Cousins.

Run the virtual machine with the `monitor` argument.

```
./virtz80.bin monitor
```

Expected result:

```
UU

Small Computer Monitor - RC2014
*
```

Then, type `help`:

```
*help
Small Computer Monitor by Stephen C Cousins (www.scc.me.uk)
Version 1.0.0 configuration R1 for Z80 based RC2014 systems

Monitor commands:
A [<address>]  = Assemble        |  D [<address>]   = Disassemble
M [<address>]  = Memory display  |  E [<address>]   = Edit memory
R [<name>]     = Registers/edit  |  F [<name>]      = Flags/edit
B [<address>]  = Breakpoint      |  S [<address>]   = Single step
I <port>       = Input from port |  O <port> <data> = Output to port
G [<address>]  = Go to program
BAUD <device> <rate>             |  CONSOLE <device>
FILL <start> <end> <byte>        |  API <function> [<A>] [<DE>]
DEVICES, DIR, HELP, RESET
*
```

### How to run the WASM version

To run the WASM version, you can use the Python built-in http server:

```
python3 -m http.server
```

or use the `Makefile.wasm`:

```
make -f Makefile.wasm serve
```

Expected results:

```
python3 -m http.server
Serving HTTP on 0.0.0.0 port 8000 (http://0.0.0.0:8000/) ...
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

### Microsoft BASIC

The Microsoft BASIC is released under the copyright of Microsoft.

```
NASCOM ROM BASIC Ver 4.7, (C) 1978 Microsoft
```

### Small Computer Monitor

The Small Computer Monitor is released under the copyright of Stephen C Cousins.

```
**************************************************************
**                     Copyright notice                     **
**                                                          **
**  This software is very nearly 100% my own work so I am   **
**  entitled to claim copyright and to grant licences to    **
**  others.                                                 **
**                                                          **
**  You are free to use this software for non-commercial    **
**  purposes provided it retains this copyright notice and  **
**  is clearly credited to me where appropriate.            **
**                                                          **
**  You are free to modify this software as you see fit     **
**  for your own use. You may also distribute derived       **
**  works provided they remain free of charge, are          **
**  appropriately credited and grant the same freedoms.     **
**                                                          **
**                    Stephen C Cousins                     **
**************************************************************
```

