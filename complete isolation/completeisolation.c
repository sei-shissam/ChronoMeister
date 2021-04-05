// Source code for coordinated cache and bank coloring (C code)
// Copyright 2021 Carnegie Mellon University.
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.
// Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.
// DM21-0234
// License.txt file:
// Source code for coordinated cache and bank coloring (C code)
// Copyright 2021 Carnegie Mellon University.
// BSD (SEI)
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
// 3. Products derived from this software may not include “Carnegie Mellon University,” "SEI” and/or “Software Engineering Institute" in the name of such derived product, nor shall “Carnegie Mellon University,” "SEI” and/or “Software Engineering Institute" be used to endorse or promote products derived from this software without prior written permission. For written permission, please contact permission@sei.cmu.edu.
// ACKNOWLEDGMENTS AND DISCLAIMERS:
// This material is based upon work funded and supported by the Department of Defense under Contract No. FA8702-15-D-0002 with Carnegie Mellon University for the operation of the Software Engineering Institute, a federally funded research and development center.
// The view, opinions, and/or findings contained in this material are those of the author(s) and should not be construed as an official Government position, policy, or decision, unless designated by other documentation.
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.
// DM21-0234
// 
// What does this program do?
//   This program configures coordinate cache and bank coloring. See
//     N. Suzuki, H. Kim, D. de Niz, B. Andersson, L. Wrage, M. Klein, R. Rajkumar,
//     "Coordinated Bank and Cache Coloring for Temporal Protection of Memory Accesses," ICESS, 2013.
//   This program is written for Linux and gcc and has been tested for that. It was originally written for MS Visual C and to be run under Windows; it might
//   still work for MS Visual C and Windows but the program has not been tested for that recently.
//   The program relies on Mixed-Integer Linear Programming (MILP) solvers. Initially, it used an open-source solver, lp_solve,
//   but now it uses Gurobi---a commercial solver that is very fast. For academic users, Gurobi is free.
//
// How do I compile it on Linux with Gurobi?
//   1. Make sure that COMPILEMSVC is 0. Do it as follows: "#define COMPILEMSVS 0"
//   2. gcc -o completeisolation completeisolation.c -lm
//   3. Note that there is no need to link with any Gurobi library.
//
// How do I run it on Linux with Gurobi?
//   1. Install Gurobi. See details on this below.
//   2. Create an input file. See details below on how to create an input file.
//   3. If the input file is called "taskset.txt" then run it as follows:
//        ./completeisolation -i taskset.txt
//
// How do I install Gurobi on Linux?
//   1. Go to Gurobi's web page and download the Gurobi installation file. It will look something like this: gurobi9.1.1_linux64.tar.gz
//   2. Choose a directory where to install. For example /home/ba/gurobi
//   3. Copy the gurobi file to this chosen directory.
//   4. Unzip and untar it. For example, run gunzip gurobi9.1.1_linux64.tar.gz and then tar -xf gurobi9.1.1_linux64.tar
//   5. Now you have all the files in this chosen directory. For example, you should have gurobi_cl under gurobi911/linux64/bin
//   6. Set environment variables. For example, in .bashrc, set them as follows:
//        export GUROBI_HOME="/home/ba/gurobi/gurobi911/linux64"
//        export PATH="${PATH}:${GUROBI_HOME}/bin"
//        export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${GUROBI_HOME}/lib"
//   7. Get a license. Do it as follows. Login to gurobi's webpage and get the instructions.
//      It will say something like this: "grbgetkey XXXXXX" where XXXXXX is a code.
//   8. Now, you have Gurobi installed. You may need to do source or open a new terminal to make sure that the environment variables take effect.
//   9. A simple way to check if you have installed Gurobi properly is to type "gurobi_cl" in a terminal.
//
// How do I install lp_solve on Ubuntu Linux?
//   1. sudo apt update
//   2. sudo apt install lp-solve
//
// How do I create an input file?
//   Open a text editor, write the following, and save (for example name the file "taskset.txt"):
//  
// 1
// 4
// 32
// 16
// 8
// 1
// 8
// 1.000000 1.000000
// 8
// 8
// 0.300000 1 
// 0.200000 2 
// 0.160000 3 
// 0.120000 4 
// 0.080000 5 
// 0.040000 6 
// 0.030000 7 
// 0.020000 8 
// 2
// 6
// 1.990000 1.990000
// 8
// 8
// 1.900000 1 
// 1.750000 2 
// 1.740000 3 
// 1.730000 4 
// 1.720000 5 
// 1.710000 6 
// 1.700000 7 
// 1.690000 8 
// 3
// 7
// 1.000000 1.000000
// 8
// 8
// 0.250000 1 
// 0.200000 2 
// 0.160000 3 
// 0.120000 4 
// 0.110000 5 
// 0.100000 6 
// 0.090000 7 
// 0.080000 8 
// 4
// 5
// 1.990000 1.990000
// 8
// 8
// 1.850000 1 
// 1.590000 2 
// 1.580000 3 
// 1.570000 4 
// 1.560000 5 
// 1.550000 6 
// 1.540000 7 
// 1.530000 8 
// 5
// 4
// 100.000000 100.000000
// 8
// 8
// 30.000000 1 
// 20.000000 2 
// 16.000000 3 
// 12.000000 4 
// 11.000000 5 
// 10.000000 6 
// 9.000000 7 
// 8.000000 8 
// 6
// 2
// 199.000000 199.000000
// 8
// 8
// 190.000000 1 
// 175.000000 2 
// 174.000000 3 
// 173.000000 4 
// 172.000000 5 
// 171.000000 6 
// 170.000000 7 
// 169.000000 8 
// 7
// 3
// 100.000000 100.000000
// 8
// 8
// 30.000000 1 
// 20.000000 2 
// 16.000000 3 
// 12.000000 4 
// 11.000000 5 
// 10.000000 6 
// 9.000000 7 
// 8.000000 8 
// 8
// 1
// 199.000000 199.000000
// 8
// 8
// 185.000000 1 
// 159.000000 2 
// 158.000000 3 
// 157.000000 4 
// 156.000000 5 
// 155.000000 6 
// 154.000000 7 
// 153.000000 8 
// 
// You can understand the meaning of these numbers by looking at the function readtasksetfromfile.
// ncells of a task describes the memory footprint of a task---see the ICESS'13 paper.
//
// Current limitations:
//   1. The code requires the use of Gurobi. The code generates a MILP instance for lpsolve but it does not invoke lpsolve; it
//      converts this instance to a Gurob instance and then invokes Gurobi to solve it.
//   2. The code does not use the native APIs of solvers. Specifically, it does not use the native API of Gurobi.
//      The code generates a python script and runs it and this python script invokes Gurobi.
//      A consequence of that is that when obtaining the solution, the code parses a text file (rather than getting the
//      solution through Gurobi API calls).
//   3. The code implements configuration for both partitioned EDF and partition fixed-priority preemptive scheduling.
//      Only the former has been tested.
//   4. The program is written for Linux and gcc. If you use it with Windows and MS visual studio, be aware that it has
//      not been tested for that recently.
//
// #define COMPILEMSVS 1
#define COMPILEMSVS 0

#if COMPILEMSVS
  #include "stdafx.h"
  #include "lp_lib.h"
#endif
#if COMPILEMSVS

#else
  #define REAL double
#endif
#include <stdlib.h>
#include <stdio.h>
#if COMPILEMSVS
#include <io.h>
#endif
#include <fcntl.h>
#if COMPILEMSVS
#include <process.h>
#endif
#include <math.h>
#include <sys/stat.h>
#include <string.h>
#if COMPILEMSVS
  #include <direct.h>
#else
  #include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#define USE_LP_SOLVE 0
#define USE_GUROBI 1

// functions in DLL which solves ILP
#if COMPILEMSVS
make_lp_func             *_make_lp;
delete_lp_func           *_delete_lp;
add_constraint_func      *_add_constraint;
set_obj_fn_func          *_set_obj_fn;
set_add_rowmode_func     *_set_add_rowmode;

set_maxim_func           *_set_maxim;
write_lp_func            *_write_lp;
set_verbose_func         *_set_verbose;
solve_func               *_solve;
get_objective_func       *_get_objective;
get_primal_solution_func *_get_primal_solution;

set_int_func             *_set_int;
get_Nrows_func           *_get_Nrows;

read_LP_func             *_read_LP;
set_anti_degen_func      *_set_anti_degen;
#endif

#define MAXNTASKS            10000
#define MAXNEXECUTIOTIMES    10000

int counterforILPsolver;
int generate_experiments = 0;
int run_experiments = 0;
char inputfilename[20000]="taskset.txt";
char outputfilename[20000]="taskset_results_from_analysis.txt";

struct executiontimestruct {
  REAL C;
  int requiresnumberofcachepartitions;
};

struct task {
  int id;
  int priority;
  REAL T;
  REAL D;
  int ncells; // this is the memory requirements of the task, counted in terms of number of memory cells---see ICESS'13 paper for details. In ths paper, this is called M_i
  int nexecutiontimes;
  struct executiontimestruct executiontime[MAXNEXECUTIOTIMES+1]; // there is no element in index 0.
  REAL R;                     // this is computed by the MILP
  REAL selectedexecutiontime; // this is computed by the MILP
  int processor_assigned_to;  // this is computed by the MILP
  int selectednumberofcachepartitions;               // this is computed by the MILP
};

int feasibleassignmentfound;

int useEDF;
int nprocessors;
int ncachecolors;
int nbankcolors;
int ntasks;
struct task tasks[MAXNTASKS+1];

// MS Visual studio does not allow write. So we call this function mywrite that in turn
// calls _write. If you compile this under Linux then just change
// _write to write
int mywrite( int fh, char* tempstr, int len) {
  #if COMPILEMSVS
  return _write( fh, tempstr, len );
  #else
  return write( fh, tempstr, len );
  #endif
}

int myopen( char* fn, int m1, int m2) {
  #if COMPILEMSVS
  return _open( fn, m1, m2);
  #else
  return open( fn, m1, m2);
  #endif
}

int myclose( int fh) {
  #if COMPILEMSVS
  return _close( fh);
  #else
  return close( fh);
  #endif
}

void printtask(FILE* myfile, int i) {
  int h;
  fprintf( myfile, "task id = %d. task priority = %d, T = %lf, D = %lf, ncells = %d \n",
           tasks[i].id, tasks[i].priority, tasks[i].T, tasks[i].D, tasks[i].ncells);
  for (h=1;h<=tasks[i].nexecutiontimes;h++) {
    fprintf( myfile, "  ");
    fprintf( myfile, "C=%lf if selectednumberofcachepartitions is %d",
      tasks[i].executiontime[h].C, tasks[i].executiontime[h].requiresnumberofcachepartitions );
    fprintf( myfile, "\n");
  }
  fprintf( myfile, "  R = %lf\n", tasks[i].R);
  fprintf( myfile, "  selectedexecutiontime = %lf\n", tasks[i].selectedexecutiontime);
  fprintf( myfile, "  processor_assigned_to = %d\n",  tasks[i].processor_assigned_to);
  fprintf( myfile, "  selectednumberofcachepartitions              = %d\n",  tasks[i].selectednumberofcachepartitions);
  fprintf( myfile, "\n");
}

void printtasksinternal(FILE* myfile) {
  int i;
  fprintf( myfile, "useEDF = %d\n",  useEDF);
  fprintf( myfile, "nprocessors = %d\n",  nprocessors);
  fprintf( myfile, "ncachecolors = %d\n", ncachecolors);
  fprintf( myfile, "nbankcolors = %d\n",  nbankcolors);
  fprintf( myfile, "ntasks = %d\n",  ntasks);
  for (i=1;i<=ntasks;i++) {
    printtask( myfile, i);
  }
}

void printtasks() {
  printtasksinternal( stdout);
}

void printtaskstooutputfile() {
  FILE* myfile;
  myfile = fopen( outputfilename, "w");
  printtasksinternal( myfile);
  fclose( myfile);
}

void readtasksetfromfile(char* p) {
  int temp;
  FILE* myfile;
  int i; int h; int v;
  myfile = fopen( p, "rt");
  if (myfile==NULL) {
    printf("Error opening file.\n");
    exit(0);
  }
  fscanf(myfile, "%d", &useEDF);
  fscanf(myfile, "%d", &nprocessors);
  fscanf(myfile, "%d", &ncachecolors);
  fscanf(myfile, "%d", &nbankcolors);
  fscanf(myfile, "%d", &ntasks);
  for (i=1;i<=ntasks;i++) {
    fscanf(myfile,"%d",  &(tasks[i].id));
    fscanf(myfile,"%d",  &(tasks[i].priority));
    fscanf(myfile,"%lf", &(tasks[i].T));
    fscanf(myfile,"%lf", &(tasks[i].D));
    fscanf(myfile,"%d",  &(tasks[i].ncells));
    fscanf(myfile,"%d",  &(tasks[i].nexecutiontimes));
    for (h=1;h<=tasks[i].nexecutiontimes;h++) {
      fscanf(myfile,"%lf", &(tasks[i].executiontime[h].C));
      fscanf(myfile,"%d",  &(tasks[i].executiontime[h].requiresnumberofcachepartitions));
    }
  }
  fclose( myfile);
}

void writetasksettofile(char* fn) {
  int temp;
  FILE* myfile;
  int i; int h; int v;
  myfile = fopen( fn, "w");
  if (myfile==NULL) {
    printf("Error opening file.\n");
    exit(0);
  }
  fprintf( myfile, "%d\n", useEDF);
  fprintf( myfile, "%d\n", nprocessors);
  fprintf( myfile, "%d\n", ncachecolors);
  fprintf( myfile, "%d\n", nbankcolors);
  fprintf( myfile, "%d\n", ntasks);
  for (i=1;i<=ntasks;i++) {
    fprintf( myfile,"%d\n",   tasks[i].id);
    fprintf( myfile,"%d\n",   tasks[i].priority);
    fprintf( myfile,"%lf ",   tasks[i].T);
    fprintf( myfile,"%lf\n",  tasks[i].D);
    fprintf( myfile,"%d\n",   tasks[i].ncells);
    fprintf( myfile,"%d\n",   tasks[i].nexecutiontimes);
    for (h=1;h<=tasks[i].nexecutiontimes;h++) {
      fprintf( myfile,"%lf ", tasks[i].executiontime[h].C);
      fprintf( myfile,"%d ",  tasks[i].executiontime[h].requiresnumberofcachepartitions);
      fprintf( myfile,"\n");
    }
    fprintf( myfile, "  R = %lf\n", tasks[i].R);
    fprintf( myfile, "  selectedexecutiontime = %lf\n", tasks[i].selectedexecutiontime);
    fprintf( myfile, "  processor_assigned_to = %d\n",  tasks[i].processor_assigned_to);
    fprintf( myfile, "  selectednumberofcachepartitions              = %d\n",  tasks[i].selectednumberofcachepartitions);
  }
  fclose( myfile);
}

void writeassignmenttofile(char* fn) {
  int temp;
  FILE* myfile;
  int i; int h; int v;
  myfile = fopen( fn, "w");
  if (myfile==NULL) {
    printf("Error opening file\n");
    exit(0);
  }
  if (feasibleassignmentfound==1) {
    fprintf( myfile, "F. Feasible assignment found.\n");
  } else {
    fprintf( myfile, "NF. Feasible assignment not found.\n");
  }
  for (i=1;i<=ntasks;i++) {
    fprintf( myfile,"Task with id=%d, priority = %d, T=%lf, D=%lf, ncells = %d\n",
	     tasks[i].id, tasks[i].priority, tasks[i].T, tasks[i].D, tasks[i].ncells);
    fprintf( myfile, "  R = %lf\n", tasks[i].R);
    fprintf( myfile, "  selectedexecutiontime = %lf\n", tasks[i].selectedexecutiontime);
    fprintf( myfile, "  processor_assigned_to = %d\n",  tasks[i].processor_assigned_to);
    fprintf( myfile, "  selectednumberofcachepartitions              = %d\n",  tasks[i].selectednumberofcachepartitions);
  }
  fclose( myfile);
}

void initializetasksandnumberofcacheandbankpartitions() {
  useEDF = 1;
  nprocessors  =  4;
  ncachecolors = 32;
  nbankcolors  = 16;

  ntasks = 8;

  tasks[1].id =        1;
  tasks[1].priority =  8;
  tasks[1].T =         1.00;
  tasks[1].D =         1.00;
  tasks[1].ncells =    8;
  tasks[1].nexecutiontimes = 8;
  tasks[1].executiontime[1].C                =  0.30;
  tasks[1].executiontime[1].requiresnumberofcachepartitions =  1;
  tasks[1].executiontime[2].C                =  0.20;
  tasks[1].executiontime[2].requiresnumberofcachepartitions =  2;
  tasks[1].executiontime[3].C                =  0.16;
  tasks[1].executiontime[3].requiresnumberofcachepartitions =  3;
  tasks[1].executiontime[4].C                =  0.12;
  tasks[1].executiontime[4].requiresnumberofcachepartitions =  4;
  tasks[1].executiontime[5].C                =  0.08;
  tasks[1].executiontime[5].requiresnumberofcachepartitions =  5;
  tasks[1].executiontime[6].C                =  0.04;
  tasks[1].executiontime[6].requiresnumberofcachepartitions =  6;
  tasks[1].executiontime[7].C                =  0.03;
  tasks[1].executiontime[7].requiresnumberofcachepartitions =  7;
  tasks[1].executiontime[8].C                =  0.02;
  tasks[1].executiontime[8].requiresnumberofcachepartitions =  8;
  tasks[1].R = -1;

  tasks[2].id =        2;
  tasks[2].priority =  6;
  tasks[2].T =         1.99;
  tasks[2].D =         1.99;
  tasks[2].ncells =    8;
  tasks[2].nexecutiontimes = 8;
  tasks[2].executiontime[1].C                =  1.90;
  tasks[2].executiontime[1].requiresnumberofcachepartitions =  1;
  tasks[2].executiontime[2].C                =  1.75;
  tasks[2].executiontime[2].requiresnumberofcachepartitions =  2;
  tasks[2].executiontime[3].C                =  1.74;
  tasks[2].executiontime[3].requiresnumberofcachepartitions =  3;
  tasks[2].executiontime[4].C                =  1.73;
  tasks[2].executiontime[4].requiresnumberofcachepartitions =  4;
  tasks[2].executiontime[5].C                =  1.72;
  tasks[2].executiontime[5].requiresnumberofcachepartitions =  5;
  tasks[2].executiontime[6].C                =  1.71;
  tasks[2].executiontime[6].requiresnumberofcachepartitions =  6;
  tasks[2].executiontime[7].C                =  1.70;
  tasks[2].executiontime[7].requiresnumberofcachepartitions =  7;
  tasks[2].executiontime[8].C                =  1.69;
  tasks[2].executiontime[8].requiresnumberofcachepartitions =  8;
  tasks[2].R = -1;

  tasks[3].id =        3;
  tasks[3].priority =  7;
  tasks[3].T =         1.00;
  tasks[3].D =         1.00;
  tasks[3].ncells =    8;
  tasks[3].nexecutiontimes = 8;
  tasks[3].executiontime[1].C                =  0.25;
  tasks[3].executiontime[1].requiresnumberofcachepartitions =  1;
  tasks[3].executiontime[2].C                =  0.20;
  tasks[3].executiontime[2].requiresnumberofcachepartitions =  2;
  tasks[3].executiontime[3].C                =  0.16;
  tasks[3].executiontime[3].requiresnumberofcachepartitions =  3;
  tasks[3].executiontime[4].C                =  0.12;
  tasks[3].executiontime[4].requiresnumberofcachepartitions =  4;
  tasks[3].executiontime[5].C                =  0.11;
  tasks[3].executiontime[5].requiresnumberofcachepartitions =  5;
  tasks[3].executiontime[6].C                =  0.10;
  tasks[3].executiontime[6].requiresnumberofcachepartitions =  6;
  tasks[3].executiontime[7].C                =  0.09;
  tasks[3].executiontime[7].requiresnumberofcachepartitions =  7;
  tasks[3].executiontime[8].C                =  0.08;
  tasks[3].executiontime[8].requiresnumberofcachepartitions =  8;
  tasks[3].R = -1;

  tasks[4].id =        4;
  tasks[4].priority =  5;
  tasks[4].T =         1.99;
  tasks[4].D =         1.99;
  tasks[4].ncells =    8;
  tasks[4].nexecutiontimes = 8;
  tasks[4].executiontime[1].C                =  1.85;
  tasks[4].executiontime[1].requiresnumberofcachepartitions =  1;
  tasks[4].executiontime[2].C                =  1.59;
  tasks[4].executiontime[2].requiresnumberofcachepartitions =  2;
  tasks[4].executiontime[3].C                =  1.58;
  tasks[4].executiontime[3].requiresnumberofcachepartitions =  3;
  tasks[4].executiontime[4].C                =  1.57;
  tasks[4].executiontime[4].requiresnumberofcachepartitions =  4;
  tasks[4].executiontime[5].C                =  1.56;
  tasks[4].executiontime[5].requiresnumberofcachepartitions =  5;
  tasks[4].executiontime[6].C                =  1.55;
  tasks[4].executiontime[6].requiresnumberofcachepartitions =  6;
  tasks[4].executiontime[7].C                =  1.54;
  tasks[4].executiontime[7].requiresnumberofcachepartitions =  7;
  tasks[4].executiontime[8].C                =  1.53;
  tasks[4].executiontime[8].requiresnumberofcachepartitions =  8;
  tasks[4].R = -1;

  tasks[5].id =        5;
  tasks[5].priority =  4;
  tasks[5].T =         100;
  tasks[5].D =         100;
  tasks[5].ncells =    8;
  tasks[5].nexecutiontimes = 8;
  tasks[5].executiontime[1].C                =   30;
  tasks[5].executiontime[1].requiresnumberofcachepartitions =  1;
  tasks[5].executiontime[2].C                =   20;
  tasks[5].executiontime[2].requiresnumberofcachepartitions =  2;
  tasks[5].executiontime[3].C                =   16;
  tasks[5].executiontime[3].requiresnumberofcachepartitions =  3;
  tasks[5].executiontime[4].C                =   12;
  tasks[5].executiontime[4].requiresnumberofcachepartitions =  4;
  tasks[5].executiontime[5].C                =   11;
  tasks[5].executiontime[5].requiresnumberofcachepartitions =  5;
  tasks[5].executiontime[6].C                =   10;
  tasks[5].executiontime[6].requiresnumberofcachepartitions =  6;
  tasks[5].executiontime[7].C                =    9;
  tasks[5].executiontime[7].requiresnumberofcachepartitions =  7;
  tasks[5].executiontime[8].C                =    8;
  tasks[5].executiontime[8].requiresnumberofcachepartitions =  8;
  tasks[5].R = -1;

  tasks[6].id =        6;
  tasks[6].priority =  2;
  tasks[6].T =         199;
  tasks[6].D =         199;
  tasks[6].ncells =    8;
  tasks[6].nexecutiontimes = 8;
  tasks[6].executiontime[1].C                =  190;
  tasks[6].executiontime[1].requiresnumberofcachepartitions =  1;
  tasks[6].executiontime[2].C                =  175;
  tasks[6].executiontime[2].requiresnumberofcachepartitions =  2;
  tasks[6].executiontime[3].C                =  174;
  tasks[6].executiontime[3].requiresnumberofcachepartitions =  3;
  tasks[6].executiontime[4].C                =  173;
  tasks[6].executiontime[4].requiresnumberofcachepartitions =  4;
  tasks[6].executiontime[5].C                =  172;
  tasks[6].executiontime[5].requiresnumberofcachepartitions =  5;
  tasks[6].executiontime[6].C                =  171;
  tasks[6].executiontime[6].requiresnumberofcachepartitions =  6;
  tasks[6].executiontime[7].C                =  170;
  tasks[6].executiontime[7].requiresnumberofcachepartitions =  7;
  tasks[6].executiontime[8].C                =  169;
  tasks[6].executiontime[8].requiresnumberofcachepartitions =  8;
  tasks[6].R = -1;

  tasks[7].id =        7;
  tasks[7].priority =  3;
  tasks[7].T =         100;
  tasks[7].D =         100;
  tasks[7].ncells =    8;
  tasks[7].nexecutiontimes = 8;
  tasks[7].executiontime[1].C                =   30;
  tasks[7].executiontime[1].requiresnumberofcachepartitions =  1;
  tasks[7].executiontime[2].C                =   20;
  tasks[7].executiontime[2].requiresnumberofcachepartitions =  2;
  tasks[7].executiontime[3].C                =   16;
  tasks[7].executiontime[3].requiresnumberofcachepartitions =  3;
  tasks[7].executiontime[4].C                =   12;
  tasks[7].executiontime[4].requiresnumberofcachepartitions =  4;
  tasks[7].executiontime[5].C                =   11;
  tasks[7].executiontime[5].requiresnumberofcachepartitions =  5;
  tasks[7].executiontime[6].C                =   10;
  tasks[7].executiontime[6].requiresnumberofcachepartitions =  6;
  tasks[7].executiontime[7].C                =    9;
  tasks[7].executiontime[7].requiresnumberofcachepartitions =  7;
  tasks[7].executiontime[8].C                =    8;
  tasks[7].executiontime[8].requiresnumberofcachepartitions =  8;
  tasks[7].R = -1;

  tasks[8].id =        8;
  tasks[8].priority =  1;
  tasks[8].T =         199;
  tasks[8].D =         199;
  tasks[8].ncells =    8;
  tasks[8].nexecutiontimes = 8;
  tasks[8].executiontime[1].C                =  185;
  tasks[8].executiontime[1].requiresnumberofcachepartitions =  1;
  tasks[8].executiontime[2].C                =  159;
  tasks[8].executiontime[2].requiresnumberofcachepartitions =  2;
  tasks[8].executiontime[3].C                =  158;
  tasks[8].executiontime[3].requiresnumberofcachepartitions =  3;
  tasks[8].executiontime[4].C                =  157;
  tasks[8].executiontime[4].requiresnumberofcachepartitions =  4;
  tasks[8].executiontime[5].C                =  156;
  tasks[8].executiontime[5].requiresnumberofcachepartitions =  5;
  tasks[8].executiontime[6].C                =  155;
  tasks[8].executiontime[6].requiresnumberofcachepartitions =  6;
  tasks[8].executiontime[7].C                =  154;
  tasks[8].executiontime[7].requiresnumberofcachepartitions =  7;
  tasks[8].executiontime[8].C                =  153;
  tasks[8].executiontime[8].requiresnumberofcachepartitions =  8;
  tasks[8].R = -1;
  readtasksetfromfile( inputfilename );
}

void deletespacesinthebeginningofstring(char* tempstr) {
  int l; int firstnonspace; int i;
  l = strlen(tempstr);
  firstnonspace = 0;
  while ((tempstr[firstnonspace]=' ') && (firstnonspace<l)) {
	  firstnonspace = firstnonspace + 1; 
  }
  for (i=0;i<=l-firstnonspace;i++) {
    tempstr[i] = tempstr[i+firstnonspace];
  }
}

void replace_word_with_word(char* fromstring, char* tostring, char* findword, char* replaceword) {
  if (strncmp( fromstring, findword, strlen(findword))==0) {
    sprintf(tostring,"%s%s", replaceword, fromstring+strlen(findword)); 
  } else {
    printf("This should not happen. There should be a substring %s in %s.\n", fromstring, findword);
  }  
}

void remove_character_in_the_end(char* tempstr, char c) {
  int l;
  l = strlen(tempstr); 
  while (tempstr[l-1]==c) {
    tempstr[l-1] = 0;
    l--;
  }
}

void remove_LRLF_and_semicolor_in_the_end(char* tempstr) {
  remove_character_in_the_end(tempstr,10);
  remove_character_in_the_end(tempstr,13);
  remove_character_in_the_end(tempstr,10);
  remove_character_in_the_end(tempstr,13);
  remove_character_in_the_end(tempstr,';');
  remove_character_in_the_end(tempstr,';');
}

int isblankline(char * s) {
  if (strlen(s)==0) {
    return 1;  
  }
  if (strcmp(s,"\n")==0) {
    return 1;  
  }
  if (strcmp(s," \n")==0) {
    return 1;  
  }
  return 0;
}

void fill_gurobifile( char* fn, char* gurobifn) {
  char *readstring;
  char *tempstr;
  int l; int cont;
  FILE* pf;
  FILE* pgf;
  readstring = (char*) malloc( sizeof(char)*200000*10);
  tempstr    = (char*) malloc( sizeof(char)*200000*10);
  cont = 1;
  pf  = fopen(  fn,       "r");
  pgf = fopen(  gurobifn, "w");
  if (pf == NULL) {
    printf("ERROR: pf is null.\n");
    printf("ERROR: filename:%s.\n",fn);
    exit(-1);
  }
  if (fgets( readstring, 200000*10, pf)==NULL) {
    printf("ERROR: there is nothing to read.\n");
    exit(-1);
  }
  if ((strncmp( readstring, "min:", 4)==0) || (strncmp( readstring, "max:", 4)==0)) {
    if (strncmp( readstring, "min:", 4)==0) {
      replace_word_with_word(readstring, tempstr, "min:", "Minimize\n  ");
    } else {
      replace_word_with_word(readstring, tempstr, "max:", "Maximize\n  ");
    }
    remove_LRLF_and_semicolor_in_the_end(tempstr);
    fprintf( pgf, "%s\n", tempstr);
  } else {
    printf("ERROR: Expected to see min or max but found neither.\n");
    exit(-1); 
  }
  fprintf( pgf, "Subject to \n" );
  if (fgets( readstring, 200000*10, pf)==NULL) {
    printf("ERROR: there is nothing to read after objective function.\n");
    exit(-1);
  }
  if (isblankline(readstring)) {
    if (fgets( readstring, 200000*10, pf)==NULL) {
      printf("ERROR: there is nothing to read after objective function.\n");
      exit(-1);
    }  
  }
  cont = 1;
  while ((strncmp( readstring, "bin", 3)!=0) && (cont==1)) {
    // OK, we know that we have not reached the section with bin; we have a normal constraint
    strcpy( tempstr, readstring);
    remove_LRLF_and_semicolor_in_the_end(tempstr);
    fprintf( pgf, "  %s \n", tempstr);
    if (fgets( readstring, 200000*10, pf)==NULL) {
      cont = 0;
    }
  }
  fprintf( pgf, "Binary\n" );
  // OK, we have reached the section with bin
  while ((strncmp( readstring, "bin", 3)==0) && (cont==1)) {
    replace_word_with_word(readstring, tempstr, "bin", "");
    remove_LRLF_and_semicolor_in_the_end(tempstr);
    fprintf( pgf, "  %s\n", tempstr);
    if (fgets( readstring, 200000*10, pf)==NULL) {
      cont = 0;
    }
  }
  fprintf( pgf, "General\n" );
  // OK, we have reached the section with bin
  while ((strncmp( readstring, "int", 3)==0) && (cont==1)) {
    replace_word_with_word(readstring, tempstr, "int", "");
    remove_LRLF_and_semicolor_in_the_end(tempstr);
    fprintf( pgf, "  %s\n", tempstr);
    if (fgets( readstring, 200000*10, pf)==NULL) {
      cont = 0;
    }
  }
  fprintf( pgf, "End\n" );
  free( tempstr );
  free( readstring );
  fclose(  pf );
  fclose( pgf );
}

void fill_gurobifile_change_optimization_to_satisfaction( char* gurobifn, char* gurobifnnewfile) {
  FILE* pf;
  FILE* pgf;
  char readstring[200000];
  char readstring2[200000];
  char readstring3[200000];
  char tempstr[200000];
  int len;

  pf  = fopen(  gurobifn,       "r");
  pgf = fopen(  gurobifnnewfile, "w");
  fgets( readstring, 200000, pf);            // This line states "Minimize"
  fgets( readstring2, 200000, pf);           // This line states "  loadratio"

  fgets( readstring3, 200000, pf);           // This line states "Subject to"
  fprintf( pgf, "%s", readstring3);

  strcpy( tempstr, readstring2);
  remove_LRLF_and_semicolor_in_the_end(tempstr);
  fprintf( pgf, "%s <= 1\n", tempstr);

  while (fgets( readstring3, 200000, pf)!=NULL) {
    fprintf( pgf, "%s", readstring3);
  }
  fclose(  pf );
  fclose( pgf );
}


void create_Gurobi_command_line_string( char* commandstring, char* gurobifn, char* gurobiresultfn) {
  sprintf( commandstring, "gurobi_cl Resultfile=%s %s", gurobiresultfn, gurobifn);
}                                                                                                                                                                

void use_readstring_in_Gurobi_solution_to_update_variables(char* tempstr1, double v) {
  char searchforstring[20000];
  int i; int p; int q;
  for (i=1;i<=ntasks;i++) {
    sprintf(searchforstring,"R_%d", i);
    if (strcmp(tempstr1, searchforstring)==0) {
      tasks[i].R = v;
    }
  }
  for (i=1;i<=ntasks;i++) {
    for (p=1;p<=nprocessors;p++) {
      sprintf(searchforstring,"x_%d_%d", i, p);
      if (strcmp(tempstr1, searchforstring)==0) {
        if (v==1) {
          tasks[i].processor_assigned_to = p;
        }
      }
    }
  }
  for (i=1;i<=ntasks;i++) {
    for (q=1;q<=ncachecolors;q++) {
      sprintf(searchforstring,"y_%d_%d", i, q);
      if (strcmp(tempstr1, searchforstring)==0) {
        if (v==1) {
          tasks[i].selectednumberofcachepartitions = q;
        }
      }
    }
  }
}

// If the instance is feasible and we are doing optimization, then 
//   we are expecting to see something like
//     # Objective value = 0.949325
//
// If the instance is feasible and we are not doing optimization (i.e., we are doing satisfaction), then 
//   we are expecting to see something like
//     # Objective value = 0
//
// If the instance is infeasible and we are not doing optimization (i.e., we are doing satisfaction), then 
//   we are expecting to see something like
//     [the solution file does not exist]
//

void extract_objective_function_from_solution_from_Gurobi(char* gurobiresultfn, int* feas, REAL* value) {
  char readstring[20000];
  char tempstr1[20000];
  char tempstr2[20000];
  char tempstr3[20000];
  char tempstr4[20000];
  char searchforstring[20000];
  int i; int p; int q;
  REAL v;
  FILE* f;
  f  = fopen(  gurobiresultfn, "r");
  if (f!=NULL) {
    if (fgets( readstring, 20000, f)!=NULL) {
      if (sscanf( readstring, "%s %s %s %s %lf", tempstr1, tempstr2, tempstr3, tempstr4, &v)==5) {
        *feas = 1;
        *value = v;
        while (fgets( readstring, 20000, f)!=NULL) {
          if (sscanf( readstring, "%s %lf", tempstr1,&v)==2) {
            use_readstring_in_Gurobi_solution_to_update_variables(tempstr1,v);                   
          } else {
            printf("The solution file from Gurobi was formatted in a weird way.\n");
            exit(-1);
          }
        }
        for (i=1;i<=ntasks;i++) {
          q = tasks[i].selectednumberofcachepartitions;
          tasks[i].selectedexecutiontime = tasks[i].executiontime[q].C;
        }
      } else {
        *feas = 0;
      }
    } else {
      *feas = 0;
    }
    fclose(f);
  } else {
    *feas = 0;  
  }
}

void produce_filename_adding_initial_g_and_changing_extension(char* fn, char* created_fn, char* extension) {
  char* temp_str_pointer;
  sprintf(created_fn,"g%s", fn);
  if (extension!=NULL) {
    temp_str_pointer = strrchr(created_fn,'.');
    strcpy(temp_str_pointer,extension);
  }
}

void create_Gurobi_script( char* fn) {
  int i; int k;
  FILE* f;
  char gurobifn[20000];
  char pythonscriptfn[20000];
  char gurobiresultfn[20000];
  produce_filename_adding_initial_g_and_changing_extension(fn, gurobifn, NULL);
  produce_filename_adding_initial_g_and_changing_extension(fn, pythonscriptfn,".py");
  produce_filename_adding_initial_g_and_changing_extension(fn, gurobiresultfn,".sol");
  f = fopen(  pythonscriptfn, "w");
  fprintf( f, "from gurobipy import *\n");
  fprintf( f, "m = read('%s')\n", gurobifn);
  fprintf( f, "m.optimize()\n");
  fprintf( f, "if (m.status==GRB.OPTIMAL):\n");
  fprintf( f, "  m.write('%s')\n", gurobiresultfn);
  fprintf( f, "exit()\n");
  fclose( f);
}

void create_Gurobi_command_line_for_executing_script( char* commandstringwithscript, char* fn) {
  char pythonscriptfn[20000];
  produce_filename_adding_initial_g_and_changing_extension(fn, pythonscriptfn,".py");
  #if COMPILEMSVS
  sprintf( commandstringwithscript, "gurobi.bat %s", pythonscriptfn);
  #else
  sprintf( commandstringwithscript, "gurobi.sh %s", pythonscriptfn);
  #endif
}

void addobjectivefunctionminloadratio( int fh) {
  char tempstr[20000];
  sprintf( tempstr, "min: loadratio;\n\n");
  mywrite( fh, tempstr, strlen( tempstr) );
}

double calcBIGNUM() {
  int j; int k; double sum; double DMAX; double CMAXforthistask;
  DMAX = tasks[1].D;
  for (k=2;k<=ntasks;k++) {
    if (tasks[k].D>DMAX) {
      DMAX = tasks[k].D;
    }
  }
  sum = 0;
  for (j=1;j<=ntasks;j++) {
    CMAXforthistask = tasks[j].executiontime[1].C;
    for (k=2;k<=tasks[j].nexecutiontimes;k++) {
      if (tasks[j].executiontime[k].C>CMAXforthistask) {
        CMAXforthistask = tasks[j].executiontime[k].C;
      }
    }
    sum = sum + ceil(DMAX/tasks[j].T)*CMAXforthistask;
  }
  return sum;
}

void addconstraintsloadratioprocessorandloadratio( int fh) {
  int p; char tempstr[20000];
  for (p=1;p<=nprocessors;p++) {
    sprintf( tempstr, "loadratioprocessor_%d - loadratio <= 0;\n", p);
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintsloadratioprocessor( int fh) {
  int i; int p;
  char tempstr[20000];
  for (p=1;p<=nprocessors;p++) {
    sprintf( tempstr, "loadratioprocessor_%d ", p);
    mywrite( fh, tempstr, strlen( tempstr) );
    for (i=1;i<=ntasks;i++) {
      sprintf( tempstr, "-%lf c_%d_%d ", 1.0/tasks[i].T, i, p);
      mywrite( fh, tempstr, strlen( tempstr) );
    }
    sprintf( tempstr, "= 0;\n");
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintseachtaskisassignedtoexactlyoneprocessor( int fh) {
  int i;
  int p;
  char tempstr[20000];
  int firstone;
  for (i=1;i<=ntasks;i++) {
    firstone = 1;
    for (p=1;p<=nprocessors;p++) {   
      if (firstone) {
        sprintf( tempstr, "x_%d_%d ", i,p );
        firstone = 0;
      } else {
        sprintf( tempstr, " + x_%d_%d ", i,p );
      }
      mywrite( fh, tempstr, strlen( tempstr) );
    }
    sprintf( tempstr, "= 1;\n");
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

REAL calculateCMAX() {
  int i; int t;
  REAL max_so_far;
  max_so_far = 0.0;
  for (i=1;i<=ntasks;i++) {
    for (t=1;((t<=tasks[i].nexecutiontimes) && (tasks[i].executiontime[t].requiresnumberofcachepartitions<=ncachecolors) && (t<=tasks[i].ncells));t++) {
      if (tasks[i].executiontime[t].C > max_so_far) {
        max_so_far = tasks[i].executiontime[t].C;
      }
    }
  }
  return max_so_far;
}

void addconstraintsxipcip( int fh) {
  int i; int p; char tempstr[20000]; REAL CMAX;
  CMAX = calculateCMAX();
  for (i=1;i<=ntasks;i++) {
    for (p=1;p<=nprocessors;p++) {
      sprintf( tempstr, "c_%d_%d - c_%d + %lf x_%d_%d <= %lf;\n", i,p, i, CMAX, i,p, CMAX);
      mywrite( fh, tempstr, strlen( tempstr) );
      sprintf( tempstr, "c_%d_%d - c_%d - %lf x_%d_%d >= -%lf;\n", i,p, i, CMAX, i,p, CMAX);
      mywrite( fh, tempstr, strlen( tempstr) );
    }
  }
  for (i=1;i<=ntasks;i++) {
    for (p=1;p<=nprocessors;p++) {
      sprintf( tempstr, "c_%d_%d - %lf x_%d_%d <= 0;\n", i,p, CMAX, i,p);
      mywrite( fh, tempstr, strlen( tempstr) );
      sprintf( tempstr, "c_%d_%d + %lf x_%d_%d >= 0;\n", i,p, CMAX, i,p);
      mywrite( fh, tempstr, strlen( tempstr) );
    }
  }
}

void addconstraintsyit( int fh) {
  int i; int t;
  char tempstr[20000];
  int firstone;
  for (i=1;i<=ntasks;i++) {
    firstone = 1;
    for (t=1;((t<=tasks[i].nexecutiontimes) && (tasks[i].executiontime[t].requiresnumberofcachepartitions<=ncachecolors) && (t<=tasks[i].ncells));t++) {
      if (firstone) {
        sprintf( tempstr, "y_%d_%d ", i,t );
        firstone = 0;
      } else {
        sprintf( tempstr, " + y_%d_%d ", i,t );
      }
      mywrite( fh, tempstr, strlen( tempstr) );
    }
    sprintf( tempstr, "= 1;\n");
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintscicityit( int fh) {
  int i; int t;
  char tempstr[20000];
  int firstone;
  for (i=1;i<=ntasks;i++) {
    firstone = 1;
    for (t=1;((t<=tasks[i].nexecutiontimes) && (tasks[i].executiontime[t].requiresnumberofcachepartitions<=ncachecolors) && (t<=tasks[i].ncells));t++) {
      if (firstone) {
        sprintf( tempstr, "%lf y_%d_%d ", tasks[i].executiontime[t].C, i,t );
        firstone = 0;
      } else {
        sprintf( tempstr, "+ %lf y_%d_%d ", tasks[i].executiontime[t].C, i,t );
      }
      mywrite( fh, tempstr, strlen( tempstr) );
    }
    sprintf( tempstr, " - c_%d = 0;\n", i );
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintsyitncachecolorsoftaskiequaltot( int fh) {
  int i; int t;
  char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    for (t=1;((t<=tasks[i].nexecutiontimes) && (tasks[i].executiontime[t].requiresnumberofcachepartitions<=ncachecolors) && (t<=tasks[i].ncells));t++) {
      sprintf( tempstr, "ncachecolorsoftask_%d + %d y_%d_%d <= %d;\n", i,  ncachecolors, i,t, t+ncachecolors );
      mywrite( fh, tempstr, strlen( tempstr) );
      sprintf( tempstr, "ncachecolorsoftask_%d - %d y_%d_%d >= %d;\n", i,  ncachecolors, i,t, t-ncachecolors );
      mywrite( fh, tempstr, strlen( tempstr) );
    }
  }  
}

void addconstraintscapacityconstraintnumberofcachecolors( int fh) {
  int p; char tempstr[20000];
  int firstone;
  firstone = 1;
  for (p=1;p<=nprocessors;p++) {
    if (firstone) {
      sprintf( tempstr, "ncachecolorsofprocessor_%d ", p);
      firstone = 0;
    } else {
      sprintf( tempstr, "+ ncachecolorsofprocessor_%d ", p);
    }
    mywrite( fh, tempstr, strlen( tempstr) );
  }
  sprintf( tempstr, " <= %d;\n", ncachecolors );
  mywrite( fh, tempstr, strlen( tempstr) );
}

void addconstraintscapacityconstraintnumberofbankcolors( int fh) {
  int p; char tempstr[20000];
  int firstone;
  firstone = 1;
  for (p=1;p<=nprocessors;p++) {
    if (firstone) {
      sprintf( tempstr, "nbankcolorsofprocessor_%d ", p);
      firstone = 0;
    } else {
      sprintf( tempstr, "+ nbankcolorsofprocessor_%d ", p);
    }
    mywrite( fh, tempstr, strlen( tempstr) );
  }
  sprintf( tempstr, " <= %d;\n", nbankcolors );
  mywrite( fh, tempstr, strlen( tempstr) );
}

void addconstraintsncachecolorsofprocessorpdefinition( int fh) {
  int i; int p; int t;
  char tempstr[20000];
  for (p=1;p<=nprocessors;p++) {
    sprintf( tempstr, "ncachecolorsofprocessor_%d ", p);
    mywrite( fh, tempstr, strlen( tempstr) );
    for (i=1;i<=ntasks;i++) {
      for (t=1;((t<=tasks[i].nexecutiontimes) && (tasks[i].executiontime[t].requiresnumberofcachepartitions<=ncachecolors) && (t<=tasks[i].ncells));t++) {
        sprintf( tempstr, "-%d z_%d_%d_%d ", t, i, p, t);
        mywrite( fh, tempstr, strlen( tempstr) );
      }
    }
    sprintf( tempstr, "= 0;\n");
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintsdefinitionofzipt( int fh) {
  int i; int p; int t;
  char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    for (p=1;p<=nprocessors;p++) {
      for (t=1;((t<=tasks[i].nexecutiontimes) && (tasks[i].executiontime[t].requiresnumberofcachepartitions<=ncachecolors) && (t<=tasks[i].ncells));t++) {
        sprintf( tempstr, "x_%d_%d + y_%d_%d - z_%d_%d_%d <= 1;\n",  i,p,   i,t,   i,p,t);
        mywrite( fh, tempstr, strlen( tempstr) );
        sprintf( tempstr, "x_%d_%d + y_%d_%d - 2 z_%d_%d_%d >= 0;\n",  i,p,   i,t,   i,p,t);
        mywrite( fh, tempstr, strlen( tempstr) );
      }
    }
  }
}

void addconstraintslowerandupperboundsonncells( int fh) {
  int i; int p; int t;
  char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    for (p=1;p<=nprocessors;p++) {
      for (t=1;((t<=tasks[i].nexecutiontimes) && (tasks[i].executiontime[t].requiresnumberofcachepartitions<=ncachecolors) && (t<=tasks[i].ncells));t++) {
        sprintf( tempstr, "%d nbankcolorsofprocessor_%d + %d z_%d_%d_%d <= %d;\n",  t-1, p, nbankcolors*ncachecolors, i,p,t, tasks[i].ncells+nbankcolors*ncachecolors-1);
        mywrite( fh, tempstr, strlen( tempstr) );
        sprintf( tempstr, "%d nbankcolorsofprocessor_%d + %d z_%d_%d_%d >= 0;\n",  t, p, -tasks[i].ncells, i,p,t );
        mywrite( fh, tempstr, strlen( tempstr) );
      }
    }
  }
}

void addconstraintsupperboundsonncells( int fh) {
  int i; int p; int t; char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    for (p=1;p<=nprocessors;p++) {
      for (t=1;((t<=tasks[i].nexecutiontimes) && (tasks[i].executiontime[t].requiresnumberofcachepartitions<=ncachecolors) && (t<=tasks[i].ncells));t++) {
        sprintf( tempstr, "%d nbankcolorsofprocessor_%d + %d z_%d_%d_%d >= 0;\n",  t, p, -tasks[i].ncells, i,p,t );
        mywrite( fh, tempstr, strlen( tempstr) );
      }
    }
  }
}


void addconstraintsnumberofcachepartitionsofataskisatmostthenumberofcells( int fh) {
  int i; int p; int t;
  char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    sprintf( tempstr, "ncachecolorsoftask_%d <= %d;\n", i,  tasks[i].ncells );
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintsRi( int fh) {
  int i; int p; int j; int k; int q;
  double BIGNUM;
  char tempstr[20000];
  BIGNUM = calcBIGNUM();
  for (i=1;i<=ntasks;i++) {
    for (p=1;p<=nprocessors;p++) {
      sprintf( tempstr, "R_%d ", i );
      mywrite( fh, tempstr, strlen( tempstr) );
      for (j=1;j<=ntasks;j++) {
        for (k=1;k<=ceil( tasks[i].D/tasks[j].T );k++) {
          for (q=0;q<=ncachecolors;q++) {
            if (tasks[j].priority >= tasks[i].priority) {
              sprintf( tempstr, " - %lf   x_%d_%d_%d_%d_%d ", k*tasks[j].executiontime[q].C, i,p,j,k,q );
              mywrite( fh, tempstr, strlen( tempstr) );
            }
          }          
        }
      }
      sprintf( tempstr, " - %lf   z_%d_%d >= -%lf;\n", BIGNUM, i, p, BIGNUM );
      mywrite( fh, tempstr, strlen( tempstr) );
    }
  }
  
  for (i=1;i<=ntasks;i++) {
    for (p=1;p<=nprocessors;p++) {
      sprintf( tempstr, "R_%d ", i );
      mywrite( fh, tempstr, strlen( tempstr) );
      for (j=1;j<=ntasks;j++) {
        for (k=1;k<=ceil( tasks[i].D/tasks[j].T );k++) {
          for (q=0;q<=ncachecolors;q++) {
            if (tasks[j].priority >= tasks[i].priority) {
              sprintf( tempstr, " - %lf   x_%d_%d_%d_%d_%d ", k*tasks[j].executiontime[q].C, i,p,j,k,q );
              mywrite( fh, tempstr, strlen( tempstr) );
            }
          }
        }
      }
      sprintf( tempstr, " + %lf   z_%d_%d <= %lf;\n", BIGNUM, i, p, BIGNUM );
      mywrite( fh, tempstr, strlen( tempstr) );
    }
  }
}

void addconstraintshowRidivides( int fh) {
  int i; int p; int j; int k; int q;
  char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    for (j=1;j<=ntasks;j++) {
      sprintf( tempstr, "R_%d ", i );
      mywrite( fh, tempstr, strlen( tempstr) );
      for (k=1;k<=ceil( tasks[i].D/tasks[j].T );k++) {
        sprintf( tempstr, " - %lf   r_%d_%d_%d ", k*tasks[j].T, i,j,k ); mywrite( fh, tempstr, strlen( tempstr) );
      }
      sprintf( tempstr, " + rem_%d_%d = 0;\n", i, j );
      mywrite( fh, tempstr, strlen( tempstr) );
      sprintf( tempstr, "rem_%d_%d >= 0;\n", i, j );
      mywrite( fh, tempstr, strlen( tempstr) );
      sprintf( tempstr, "rem_%d_%d <= %lf;\n", i, j, tasks[j].T );
      mywrite( fh, tempstr, strlen( tempstr) );
    }
  }
}

void addconstraintscapacityLLC( int fh) {
  int i; int p; int j; int k; int q;
  char tempstr[20000];
  int first;
  first = 1;
  for (i=1;i<=ntasks;i++) {
    for (q=0;q<=ncachecolors;q++) {
      if (first) {
        sprintf( tempstr, "   %d   y_%d_%d ", q, i, q );
	mywrite( fh, tempstr, strlen( tempstr) );
	first = 0;
      } else {
        sprintf( tempstr, " + %d   y_%d_%d ", q, i, q );
        mywrite( fh, tempstr, strlen( tempstr) );
      }
    }
  }
  sprintf( tempstr, " <= %d;\n", ncachecolors );
  mywrite( fh, tempstr, strlen( tempstr) );
}

void addconstraintsdeadlinesmustbemet( int fh) {
  int i; char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    sprintf( tempstr, "R_%d - %lf deadlinecompressratio <= 0;\n", i, tasks[i].D );
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintsataskisassignedtoexactlyoneprocessor( int fh) {
  int i; int p;
  char tempstr[20000];
  int first;
  for (i=1;i<=ntasks;i++) {
    first = 1;
    for (p=1;p<=nprocessors;p++) {
      if (first) {
        sprintf( tempstr, "   z_%d_%d", i, p );
        mywrite( fh, tempstr, strlen( tempstr) );
        first = 0;
      } else {
        sprintf( tempstr, " + z_%d_%d", i, p );
        mywrite( fh, tempstr, strlen( tempstr) );
      }
    }
    sprintf( tempstr, " = 1;\n"  );
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintsatasktakeexactlyoneoptionforLLC( int fh) {
  int i; int j; int k; int q;
  char tempstr[20000];
  int first;
  for (i=1;i<=ntasks;i++) {
    first = 1;
    for (q=0;q<=ncachecolors;q++) {
      if (first) {
        sprintf( tempstr, "   y_%d_%d ", i,q );
        mywrite( fh, tempstr, strlen( tempstr) );
        first = 0;
      } else {
        sprintf( tempstr, " + y_%d_%d ", i,q );
        mywrite( fh, tempstr, strlen( tempstr) );
      }
    }
    sprintf( tempstr, " = 1;\n");
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintsatasktakeexactlyoneoptionforrespdividingTj( int fh) {
  int i; int j; int k;
  char tempstr[20000];
  int first;
  for (i=1;i<=ntasks;i++) {
    for (j=1;j<=ntasks; j++ ) {
      first = 1;
      for (k=1;k<=ceil( tasks[i].D/tasks[j].T );k++) {
        if (first) {
          sprintf( tempstr, "   r_%d_%d_%d ", i,j,k );
          mywrite( fh, tempstr, strlen( tempstr) );
          first = 0;
        } else {
          sprintf( tempstr, " + r_%d_%d_%d ", i,j,k );
          mywrite( fh, tempstr, strlen( tempstr) );
        }
      }
      sprintf( tempstr, " = 1;\n");
      mywrite( fh, tempstr, strlen( tempstr) );
    }
  }
}

void addconstrainthowxdependsonyzr( int fh) {
  int i; int p; int j; int k; int q;
  char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    for (p=1;p<=nprocessors;p++) {
      for (j=1;j<=ntasks;j++) {
        for (k=1;k<=ceil( tasks[i].D/tasks[j].T );k++) {
          for (q=0;q<=ncachecolors;q++) {
            sprintf( tempstr, " y_%d_%d + z_%d_%d + z_%d_%d + r_%d_%d_%d -   x_%d_%d_%d_%d_%d <= 3;\n", j,q,  i,p, j,p,  i,j,k,  i,p,j,k,q);
	    mywrite( fh, tempstr, strlen( tempstr) );
            sprintf( tempstr, "-y_%d_%d - z_%d_%d - z_%d_%d - r_%d_%d_%d + 4 x_%d_%d_%d_%d_%d <= 0;\n", j,q,  i,p,  j,p,  i,j,k,  i,p,j,k,q);
	    mywrite( fh, tempstr, strlen( tempstr) );
	  }
	}
      }
    }
  }
}

void addconstraintsaboutthatvariablesarebinary( int fh) {
  int i; int p; int j; int k; int q;
  char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    for (p=1;p<=nprocessors;p++) {
      for (j=1;j<=ntasks;j++) {
        for (k=1;k<=ceil( tasks[i].D/tasks[j].T );k++) {
          for (q=0;q<=ncachecolors;q++) {
            sprintf( tempstr, "bin x_%d_%d_%d_%d_%d;\n", i,p,j,k,q );
            mywrite( fh, tempstr, strlen( tempstr) );
          }
        }
      }
    }
  }
  for (i=1;i<=ntasks;i++) {
    for (q=0;q<=ncachecolors;q++) {
      sprintf( tempstr, "bin y_%d_%d;\n", i,q );
      mywrite( fh, tempstr, strlen( tempstr) );
    }
  }
  for (i=1;i<=ntasks;i++) {
    for (p=1;p<=nprocessors;p++) {
      sprintf( tempstr, "bin z_%d_%d;\n", i,p );
      mywrite( fh, tempstr, strlen( tempstr) );
    }
  }
  for (i=1;i<=ntasks;i++) {
    for (j=1;j<=ntasks;j++) {
      for (k=1;k<=ceil( tasks[i].D/tasks[j].T );k++) {
        sprintf( tempstr, "bin r_%d_%d_%d;\n", i,j,k );
        mywrite( fh, tempstr, strlen( tempstr) );
      }
    }
  }
}

void addconstraintsthatncachecolorsoftaskiisatleastone( int fh) {
  int i; char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    sprintf( tempstr, "ncachecolorsoftask_%d >= 1;\n", i );
    mywrite( fh, tempstr, strlen( tempstr) );
    sprintf( tempstr, "ncachecolorsoftask_%d <= %d;\n", i, ncachecolors );
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintsthatncachecolorsofprocessorpisatleastone( int fh) {
  int p; char tempstr[20000];
  for (p=1;p<=nprocessors;p++) {
    sprintf( tempstr, "ncachecolorsofprocessor_%d >= 1;\n", p );
    mywrite( fh, tempstr, strlen( tempstr) );
    sprintf( tempstr, "ncachecolorsofprocessor_%d <= %d;\n", p, ncachecolors );
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintsthatnbankcolorsofprocessorpisatleastone( int fh) {
  int p;
  char tempstr[20000];
  for (p=1;p<=nprocessors;p++) {
    sprintf( tempstr, "nbankcolorsofprocessor_%d >= 1;\n", p );
    mywrite( fh, tempstr, strlen( tempstr) );
    sprintf( tempstr, "nbankcolorsofprocessor_%d <= %d;\n", p, nbankcolors );
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintsthatxipisbinary( int fh) {
  int i; int p; char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    for (p=1;p<=nprocessors;p++) {
      sprintf( tempstr, "bin x_%d_%d;\n", i,p );
      mywrite( fh, tempstr, strlen( tempstr) );
    }
  }
}

void addconstraintsthatyitisbinary( int fh) {
  int i; int t; char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    for (t=1;t<=tasks[i].nexecutiontimes;t++) {
      sprintf( tempstr, "bin y_%d_%d;\n", i,t );
      mywrite( fh, tempstr, strlen( tempstr) );
    }
  }
}

void addconstraintsthatziptisbinary( int fh) {
  int i; int p; int t; char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    for (p=1;p<=nprocessors;p++) {
      for (t=1;t<=tasks[i].nexecutiontimes;t++) {
        sprintf( tempstr, "bin z_%d_%d_%d;\n", i,p,t );
        mywrite( fh, tempstr, strlen( tempstr) );
      }
    }
  }
}

void addconstraintsthatncachecolorsoftaskiisinteger( int fh) {
  int i; char tempstr[20000];
  for (i=1;i<=ntasks;i++) {
    sprintf( tempstr, "int ncachecolorsoftask_%d;\n", i );
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintsthatncachecolorsofprocessorpisinteger( int fh) {
  int p; char tempstr[20000];
  for (p=1;p<=nprocessors;p++) {
    sprintf( tempstr, "int ncachecolorsofprocessor_%d;\n", p );
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void addconstraintsthatnbankcolorsofprocessorpisinteger( int fh) {
  int p; char tempstr[20000];
  for (p=1;p<=nprocessors;p++) {
    sprintf( tempstr, "int nbankcolorsofprocessor_%d;\n", p );
    mywrite( fh, tempstr, strlen( tempstr) );
  }
}

void mycopyfile(char* source,char* destination) {
  char tempstr[20000];
  #if COMPILEMSVS
  sprintf( tempstr, "copy %s %s", source, destination );
  #else
  sprintf( tempstr, "cp %s %s", source, destination );
  #endif
  system(tempstr);
}

// this code writes to a file .lp that is suited for lp_solve solver. Then this .lp file is converted to a format that is suited for Gurobi
// Note that we don't need the variable ncachecolorsoftask_%d for the configuration of EDF.
// Therefore, we don't need to call addconstraintsthatncachecolorsoftaskiisatleastone
// and we don't need to call addconstraintsthatncachecolorsoftaskiisinteger
void solve_optimization_problem_implicit_deadline_sporadic_tasks_EDF( int* feas, REAL* value ) {
   #if COMPILEMSVS
   lprec* mylprec;
   #endif
   char fn[20000];
   char gurobifn[20000];
   char gurobiresultfn[20000];
   char commandstring[20000];
   char commandstringwithscript[20000];
   int fh;
   int ret;
   char gurobifn2[20000];
   sprintf( fn, "anilp%d.lp", counterforILPsolver);
   counterforILPsolver++;
   sprintf( gurobifn2, "apa");
   #if COMPILEMSVS
   fh = myopen( fn, O_CREAT | O_WRONLY, S_IWRITE);
   #else
   fh = myopen( fn, O_CREAT | O_RDWR, S_IRWXU);
   #endif
   if (fh<0) {
     printf("Error in resp. Failed to open file.\n"); exit(-1);  fflush(stdout);
   }
   addobjectivefunctionminloadratio( fh);
   addconstraintsloadratioprocessorandloadratio( fh);
   addconstraintsloadratioprocessor( fh);
   addconstraintseachtaskisassignedtoexactlyoneprocessor( fh);
   addconstraintsxipcip( fh);
   addconstraintsyit( fh);
   addconstraintscicityit( fh);
   addconstraintscapacityconstraintnumberofcachecolors( fh);
   addconstraintscapacityconstraintnumberofbankcolors( fh);
   addconstraintsncachecolorsofprocessorpdefinition( fh);
   addconstraintsdefinitionofzipt( fh);
   addconstraintsupperboundsonncells( fh);
   addconstraintsthatncachecolorsofprocessorpisatleastone( fh);
   addconstraintsthatnbankcolorsofprocessorpisatleastone( fh);
   addconstraintsthatxipisbinary( fh);
   addconstraintsthatyitisbinary( fh);
   addconstraintsthatziptisbinary( fh);
   addconstraintsthatncachecolorsofprocessorpisinteger( fh);
   addconstraintsthatnbankcolorsofprocessorpisinteger( fh);
   myclose( fh);
   produce_filename_adding_initial_g_and_changing_extension(fn, gurobifn, NULL);  
   produce_filename_adding_initial_g_and_changing_extension(fn, gurobiresultfn,".sol");
   remove(gurobiresultfn);
   fill_gurobifile( fn, gurobifn);
   mycopyfile(gurobifn,gurobifn2);
   fill_gurobifile_change_optimization_to_satisfaction( gurobifn2, gurobifn);
   #if COMPILEMSVS
   if (USE_LP_SOLVE==1) {
     mylprec = _read_LP(fn, NORMAL, NULL);
     if (mylprec==NULL) {
       printf("Error in resp\n"); exit(-1);
     }
     _set_anti_degen( mylprec, ANTIDEGEN_FIXEDVARS | ANTIDEGEN_DYNAMIC );
     ret = _solve( mylprec);
     if (ret==OPTIMAL) {
       *feas = 1;
    	 *value = _get_objective(mylprec);
     } else {
       *feas = 0;    
     }
     _delete_lp( mylprec);
   }
   #endif
   if (USE_GUROBI==1) {
     create_Gurobi_script( fn);
     create_Gurobi_command_line_for_executing_script( commandstringwithscript, fn );
     system( commandstringwithscript);
     extract_objective_function_from_solution_from_Gurobi(gurobiresultfn, feas, value);
   } else {
     printf("Feature not implemented. solve_optimization_problem_implicit_deadline_sporadic_tasks_EDF.\n");
     exit(-1);
   }
}

void solve_optimization_problem_constrained_deadline_sporadic_tasks_DM( int* feas, REAL* value ) {
  #if COMPILEMSVS
  lprec* mylprec;
  #endif
  char fn[20000];
  char gurobifn[20000];
  char gurobiresultfn[20000];
  char commandstring[20000];
  char commandstringwithscript[20000];
  int fh;
  int ret;
  char gurobifn2[20000];
  sprintf( fn, "anilp%d.lp", counterforILPsolver);
  counterforILPsolver++;
  sprintf( gurobifn2, "apa");
  #if COMPILEMSVS
  fh = myopen( fn, O_CREAT | O_WRONLY, S_IWRITE);
  #else
  fh = myopen( fn, O_CREAT | O_WRONLY , S_IRWXU);
  #endif
  if (fh<0) {
    printf("Error in resp. Failed to open file.\n"); exit(-1);  fflush(stdout);
  }
  addobjectivefunctionminloadratio( fh);
  addconstraintsloadratioprocessorandloadratio( fh);
  
  addconstraintsloadratioprocessor( fh);
  addconstraintseachtaskisassignedtoexactlyoneprocessor( fh);
  addconstraintsxipcip( fh);
  addconstraintsyit( fh);
  addconstraintscicityit( fh);
  addconstraintsyitncachecolorsoftaskiequaltot( fh);
  addconstraintscapacityconstraintnumberofcachecolors( fh);
  addconstraintscapacityconstraintnumberofbankcolors( fh);
  addconstraintsncachecolorsofprocessorpdefinition( fh);
  addconstraintsdefinitionofzipt( fh);
  addconstraintslowerandupperboundsonncells( fh);
  addconstraintsnumberofcachepartitionsofataskisatmostthenumberofcells( fh);
  addconstraintsthatncachecolorsoftaskiisatleastone( fh);
  addconstraintsthatncachecolorsofprocessorpisatleastone( fh);
  addconstraintsthatnbankcolorsofprocessorpisatleastone( fh);
 
  addconstraintsthatxipisbinary( fh);
  addconstraintsthatyitisbinary( fh);
  addconstraintsthatziptisbinary( fh);
  
  addconstraintsthatncachecolorsoftaskiisinteger( fh);
  addconstraintsthatncachecolorsofprocessorpisinteger( fh);
  addconstraintsthatnbankcolorsofprocessorpisinteger( fh);
  
  myclose( fh);
  
  produce_filename_adding_initial_g_and_changing_extension(fn, gurobifn, NULL);  
  produce_filename_adding_initial_g_and_changing_extension(fn, gurobiresultfn,".sol");
  remove(gurobiresultfn);
  fill_gurobifile( fn, gurobifn);
  
  mycopyfile(gurobifn,gurobifn2);
  fill_gurobifile_change_optimization_to_satisfaction( gurobifn2, gurobifn);
  
  #if COMPILEMSVS
  if (USE_LP_SOLVE==1) {
    mylprec = _read_LP(fn, NORMAL, NULL);
    if (mylprec==NULL) {
      printf("Error in resp\n"); exit(-1);
    }
    _set_anti_degen( mylprec, ANTIDEGEN_FIXEDVARS | ANTIDEGEN_DYNAMIC );
    ret = _solve( mylprec);
    if (ret==OPTIMAL) {
      *feas = 1;
      *value = _get_objective(mylprec);
    } else {
      *feas = 0;
    }
    _delete_lp( mylprec);
  }
  #endif
 
  if (USE_GUROBI==1) {
    create_Gurobi_script( fn);
    create_Gurobi_command_line_for_executing_script( commandstringwithscript, fn );
    printf("%s\n", commandstringwithscript);
    system( commandstringwithscript);
    extract_objective_function_from_solution_from_Gurobi(gurobiresultfn, feas, value);
  }
}

void solveit() {
  int feas; REAL value;
  if (useEDF==1) {
    solve_optimization_problem_implicit_deadline_sporadic_tasks_EDF( &feas, &value);
    feasibleassignmentfound = feas;
  } else {
    solve_optimization_problem_constrained_deadline_sporadic_tasks_DM( &feas, &value);
    feasibleassignmentfound = feas;
  }
}

void parsearguments( int argc, char *argv[], char *envp[]) {
  int temp; int i;
  i = 1;
  while (i<=argc-1) {
    if (strcmp( argv[i], "-i")==0) {
      strcpy( inputfilename, argv[i+1] );
      i = i + 2;
    } else {
      if (strcmp( argv[i], "-o")==0) {
        strcpy( outputfilename, argv[i+1] );
        i = i + 2;
      } else {
        if (strcmp( argv[i], "-g")==0) {
          generate_experiments = 1;
          i = i + 1;
        } else {
          if (strcmp( argv[i], "-r")==0) {
            run_experiments = 1;
            i = i + 1;
          } else {
            i++;
          }
        }
      }
    }
  }
}

int main( int argc, char *argv[], char *envp[]) {
  printf("Starting tool that optimally assigns tasks and configures cache colors\n");
  parsearguments( argc, argv, envp);
  initializetasksandnumberofcacheandbankpartitions(); // if we have an input file then these will be overwritten by the parameters of the input file
  solveit();
  writeassignmenttofile(outputfilename);
  return 0;
}
