//
// auto-generated by op2.m on 21-Feb-2013 15:19:34
//

// user function

#include "incConst.h"


// x86 kernel function

void op_x86_incConst(
  float *arg0,
  float *arg1,
  const int *arg2,
  int   start,
  int   finish ) {


  // process set elements

  for (int n=start; n<finish; n++) {

    // user-supplied kernel call


    incConst(  arg0+n*1,
               arg1+n*4,
               arg2 );
  }
}


// host stub function

void op_par_loop_incConst(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2 ){


  int    nargs   = 3;
  op_arg args[3];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;

  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  incConst\n");
  }

  op_mpi_halo_exchanges(set, nargs, args);

  // initialise timers

  double cpu_t1, cpu_t2, wall_t1=0, wall_t2=0;
  op_timing_realloc(6);
  OP_kernels[6].name      = name;
  OP_kernels[6].count    += 1;

  // set number of threads

#ifdef _OPENMP
  int nthreads = omp_get_max_threads( );
#else
  int nthreads = 1;
#endif

  if (set->size >0) {

    op_timers_core(&cpu_t1, &wall_t1);

  // execute plan

#pragma omp parallel for
  for (int thr=0; thr<nthreads; thr++) {
    int start  = (set->size* thr   )/nthreads;
    int finish = (set->size*(thr+1))/nthreads;
    op_x86_incConst( (float *) arg0.data,
                     (float *) arg1.data,
                     (int *) arg2.data,
                     start, finish );
  }

  }


  // combine reduction data

  op_mpi_set_dirtybit(nargs, args);

  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[6].time     += wall_t2 - wall_t1;
  OP_kernels[6].transfer += (float)set->size * arg0.size;
  OP_kernels[6].transfer += (float)set->size * arg1.size * 2.0f;
}

