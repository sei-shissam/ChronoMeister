# Complete Isolation Readme

This program configures coordinate cache and bank coloring (see References).

This program is written for Linux and gcc and has been tested for that. It was originally written for MS Visual C and to be run under Windows; it might still work for MS Visual C and Windows but the program has not been tested for that recently.

The program relies on Mixed-Integer Linear Programming (MILP) solvers. Initially, it used an open-source solver, lp_solve, but now it uses [Gurobi](https://www.gurobi.com/)---a commercial solver that is very fast. For academic users, Gurobi is free.

## References

N. Suzuki, H. Kim, D. de Niz, B. Andersson, L. Wrage, M. Klein, R. Rajkumar, "Coordinated Bank and Cache Coloring for Temporal Protection of Memory Accesses," ICESS, 2013.

## Reporting bugs and requesting features

TBD

## License

Copyright 2021 Carnegie Mellon University. See the [LICENSE.md](./LICENSE.md) files for details.
