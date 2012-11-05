//
// auto-generated by op2.m on 21-Oct-2012 11:11:06
//

// user function

#include "initU_formula.h"


// x86 kernel function

void op_x86_initU_formula(
  float *arg0,
  float *arg1,
  const float *arg2,
  int   start,
  int   finish ) {


  // process set elements

  for (int n=start; n<finish; n++) {

    // user-supplied kernel call


    initU_formula(  arg0+n*2,
                    arg1+n*4,
                    arg2 );
  }
}


// host stub function

void op_par_loop_initU_formula(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2 ){


  int    nargs   = 3;
  op_arg args[3];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;

  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  initU_formula\n");
  }

  op_mpi_halo_exchanges(set, nargs, args);

  // initialise timers

  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timers_core(&cpu_t1, &wall_t1);

  // set number of threads

#ifdef _OPENMP
  int nthreads = omp_get_max_threads( );
#else
  int nthreads = 1;
#endif

  if (set->size >0) {


  // execute plan

#pragma omp parallel for
  for (int thr=0; thr<nthreads; thr++) {
    int start  = (set->size* thr   )/nthreads;
    int finish = (set->size*(thr+1))/nthreads;
    op_x86_initU_formula( (float *) arg0.data,
                          (float *) arg1.data,
                          (float *) arg2.data,
                          start, finish );
  }

  }


  // combine reduction data

  op_mpi_set_dirtybit(nargs, args);

  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  op_timing_realloc(5);
  OP_kernels[5].name      = name;
  OP_kernels[5].count    += 1;
  OP_kernels[5].time     += wall_t2 - wall_t1;
  OP_kernels[5].transfer += (float)set->size * arg0.size;
  OP_kernels[5].transfer += (float)set->size * arg1.size * 2.0f;
}

