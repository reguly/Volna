//
// auto-generated by op2.m on 07-Nov-2012 09:39:44
//

// user function

#include "getTotalVol.h"


// x86 kernel function

void op_x86_getTotalVol(
  float *arg0,
  float *arg1,
  float *arg2,
  int   start,
  int   finish ) {


  // process set elements

  for (int n=start; n<finish; n++) {

    // user-supplied kernel call


    getTotalVol(  arg0+n*1,
                  arg1+n*4,
                  arg2 );
  }
}


// host stub function

void op_par_loop_getTotalVol(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2 ){

  float *arg2h = (float *)arg2.data;

  int    nargs   = 3;
  op_arg args[3];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;

  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  getTotalVol\n");
  }

  op_mpi_halo_exchanges(set, nargs, args);

  // initialise timers

  double cpu_t1, cpu_t2, wall_t1=0, wall_t2=0;
  op_timing_realloc(12);
  OP_kernels[12].name      = name;
  OP_kernels[12].count    += 1;

  // set number of threads

#ifdef _OPENMP
  int nthreads = omp_get_max_threads( );
#else
  int nthreads = 1;
#endif

  // allocate and initialise arrays for global reduction

  float arg2_l[1+64*64];
  for (int thr=0; thr<nthreads; thr++)
    for (int d=0; d<1; d++) arg2_l[d+thr*64]=ZERO_float;

  if (set->size >0) {

    op_timers_core(&cpu_t1, &wall_t1);

  // execute plan

#pragma omp parallel for
  for (int thr=0; thr<nthreads; thr++) {
    int start  = (set->size* thr   )/nthreads;
    int finish = (set->size*(thr+1))/nthreads;
    op_x86_getTotalVol( (float *) arg0.data,
                        (float *) arg1.data,
                        arg2_l + thr*64,
                        start, finish );
  }

  }


  // combine reduction data

  for (int thr=0; thr<nthreads; thr++)
    for(int d=0; d<1; d++) arg2h[d] += arg2_l[d+thr*64];

  op_mpi_reduce(&arg2,arg2h);

  op_mpi_set_dirtybit(nargs, args);

  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[12].time     += wall_t2 - wall_t1;
  OP_kernels[12].transfer += (float)set->size * arg0.size;
  OP_kernels[12].transfer += (float)set->size * arg1.size;
}

