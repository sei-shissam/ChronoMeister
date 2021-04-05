# Complete Isolation Readme

This program configures coordinate cache and bank coloring (see References).

This program is written for Linux and gcc and has been tested for that. It was originally written for MS Visual C and to be run under Windows; it might still work for MS Visual C and Windows but the program has not been tested for that recently.

The program relies on Mixed-Integer Linear Programming (MILP) solvers. Initially, it used an open-source solver, lp_solve, but now it uses [Gurobi](https://www.gurobi.com/)---a commercial solver that is very fast. For academic users, Gurobi is free.

## Installation

### With Gurobi

1. Make sure that `COMPILEMSVC` is `0`. Do it as follows: `#define COMPILEMSVS 0`
2. `gcc -o completeisolation completeisolation.c -lm`

NB: Note that there is no need to link with any Gurobi library at compile time.

### With `lp_solve`

TBD

## Running

### prerequisite: Install a Required solver

#### Install Gurobi (using this solver)

1. Go to Gurobi's web page and download the Gurobi installation file. It will look something like this: `gurobi9.1.1_linux64.tar.gz`
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

#### Install `lp_solve` (Ubuntu, debian) (usiong this solver)

1. sudo apt update
2. sudo apt install lp-solve

#### Command line

```
./completeisolation -i taskset.txt
```

### With `lp_solve`


## References

N. Suzuki, H. Kim, D. de Niz, B. Andersson, L. Wrage, M. Klein, R. Rajkumar, "Coordinated Bank and Cache Coloring for Temporal Protection of Memory Accesses," ICESS, 2013.

## Reporting bugs and requesting features

TBD

## License

Copyright 2021 Carnegie Mellon University. See the [LICENSE.md](./LICENSE.md) files for details.
