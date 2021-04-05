# Complete Isolation Readme

This program configures coordinate cache and bank coloring (see References).

This program is written for Linux and gcc and has been tested for that. It was originally written for MS Visual C and to be run under Windows; it might still work for MS Visual C and Windows but the program has not been tested for that recently.

The program relies on Mixed-Integer Linear Programming (MILP) solvers. Initially, it used an open-source solver, lp_solve, but now it uses [Gurobi](https://www.gurobi.com/)---a commercial solver that is very fast. For academic users, Gurobi is free.

A Microsoft PowerPoint presentation is included in the repo which explains the process and operation of this tool.

## Installation

### With Gurobi

1. Make sure that `COMPILEMSVC` is `0`. Do it as follows: `#define COMPILEMSVS 0`
2. `gcc -o completeisolation completeisolation.c -lm`

NB: Note that there is no need to link with any Gurobi library at compile time.

### With `lp_solve`

TBD

## Running

### Prerequisite: Install a Required solver

#### Install Gurobi (using this solver)

1. Go to Gurobi's web page and download the Gurobi Optimizer installation file (free registration required). It will look something like this: `gurobi9.1.1_linux64.tar.gz`
2. Choose a directory where to install. For example, `/home/ba/gurobi`
3. Copy the gurobi file to this chosen directory.
4. Unzip and untar it. For example, run `gunzip gurobi9.1.1_linux64.tar.gz` and then `tar -xf gurobi9.1.1_linux64.tar`
5. Now you have all the files in this chosen directory. For example, you should have `gurobi_cl` under `gurobi911/linux64/bin`
6. Set environment variables. For example, in `.bashrc`, set them as follows:
```
     export GUROBI_HOME="/home/ba/gurobi/gurobi911/linux64"
     export PATH="${PATH}:${GUROBI_HOME}/bin"
     export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${GUROBI_HOME}/lib"
```
7. Get a license. Do it as follows. Login to gurobi's webpage and get the instructions.
   It will say something like this: `grbgetkey XXXXXX` where `XXXXXX` is a code.
8. Now, you have Gurobi installed. You may need to do source or open a new terminal to make sure that the environment variables take effect.
9. A simple way to check if you have installed Gurobi properly is to type `gurobi_cl` in a terminal.

#### Install `lp_solve` (Ubuntu, debian) (using this solver)

1. sudo apt update
2. sudo apt install lp-solve

#### Command line

```
./completeisolation -i taskset.txt
```

## Input File

TBD, but for now, the C function in the source file called `readtasksetfromfile` parses the input file to an internal structure for computation. `ncells` of a task describes the memory footprint of a task---see the ICESS'13 paper in the References.

The input file is passed to the program on the command line.

## Output

TBD

## Limitations

1. The code requires the use of Gurobi. The code generates a MILP instance for `lp_solve` but it does not invoke `lp_solve`; it converts this instance to a Gurob instance and then invokes Gurobi to solve it.
2. The code does not use the native APIs of solvers. Specifically, it does not use the native API of Gurobi.  The code generates a python script and runs it and this python script invokes Gurobi.  A consequence of that is that when obtaining the solution, the code parses a text file (rather than getting the solution through Gurobi API calls).
3. The code implements configuration for both partitioned EDF and partition fixed-priority preemptive scheduling.  Only the former has been tested.
4. The program is written for Linux and gcc. If you use it with Windows and MS visual studio, be aware that it has not been tested for that recently.

## References

N. Suzuki, H. Kim, D. de Niz, B. Andersson, L. Wrage, M. Klein, R. Rajkumar, "Coordinated Bank and Cache Coloring for Temporal Protection of Memory Accesses," ICESS, 2013.

## Reporting bugs and requesting features

TBD

## License

Copyright 2021 Carnegie Mellon University. See the [LICENSE.md](./LICENSE.md) files for details.
