/*  Copyright (C) 2006 Imperial College London and others.

    Please see the AUTHORS file in the main source directory for a full list
    of copyright holders.

    Prof. C Pain
    Applied Modelling and Computation Group
    Department of Earth Science and Engineering
    Imperial College London

    amcgsoftware@imperial.ac.uk

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation,
    version 2.1 of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
    USA
*/
#include "fmangle.h"
#include "Usage.h"

#include "flmpi.h"

extern "C" {
  void supermesh_difference(const char*, size_t, const char*, size_t, const char*, size_t);
}

#include <unistd.h>

#ifndef _AIX
#include <getopt.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <map>
#include <string>
#include <iostream>

using namespace std;

void project_vtu_usage(char *binary){
  cerr<<"Usage: "<<binary<<" [OPTIONS] VTU1 VTU2 OUTPUTVTU\n"
      <<"Computes the exact difference between two vtus, via supermesh construction\n"
      <<"\t-h\t\tPrints out this message\n"
      <<"\t-v\t\tVerbose mode\n";
}

int main(int argc, char **argv){
 #ifdef HAVE_MPI
  // This must be called before we process any arguments
  MPI_Init(&argc, &argv);

  // Undo some MPI init shenanigans
  int ierr = chdir(getenv("PWD"));
  if (ierr == -1) {
        cerr << "Unable to switch to directory " << getenv("PWD");
        abort();
  }
#endif

  // Initialise PETSc (this also parses PETSc command line arguments)
  PetscInit(argc, argv);

  // Get any command line arguments
  // reset optarg so we can detect changes
  optarg = NULL;
  char c;
  map<char, string> args;
  while((c = getopt(argc, argv, "hv")) != -1){
    if (c != '?'){
      if (optarg == NULL){
        args[c] = "true";
      }else{
        args[c] = optarg;
      };
    }else{
      if (isprint(optopt)){
        cerr << "Unknown option " << optopt << endl;
      }else{
        cerr << "Unknown option " << hex << optopt << endl;
      }
      project_vtu_usage(argv[0]);
      exit(-1);
    }
  }

  // Help?
  if(args.count('h')){
    project_vtu_usage(argv[0]);
    exit(-1);
  }

  if (optind != argc - 3){
    cerr << "Need exactly four non-option arguments" << endl;
    project_vtu_usage(argv[0]);
    exit(-1);
  }

  // What to do with stdout?
  int val=3;
  if(args.count('v')==0)
    val = 0;
  set_global_debug_level_fc(&val);

  string vtu1_filename = argv[optind];
  size_t vtu1_filename_len = vtu1_filename.size();

  string vtu2_filename = argv[optind + 1];
  size_t vtu2_filename_len = vtu2_filename.size();

  string output_filename = argv[optind + 2];
  size_t output_filename_len = output_filename.size();

  supermesh_difference(vtu1_filename.c_str(), vtu1_filename_len, vtu2_filename.c_str(), vtu2_filename_len, output_filename.c_str(), output_filename_len);

#ifdef HAVE_PETSC
  PetscFinalize();
#endif

#ifdef HAVE_MPI
  MPI_Finalize();
#endif
  return(0);
}
