//
// auto-generated by op2.py on 2014-06-24 15:11
//

//user function
#include "EvolveValuesRK2_1.h"

// host stub function
void op_par_loop_EvolveValuesRK2_1(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4){

  int nargs = 5;
  op_arg args[5];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(0);
  op_timers_core(&cpu_t1, &wall_t1);


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  EvolveValuesRK2_1");
  }

  op_mpi_halo_exchanges(set, nargs, args);
  // set number of threads
  #ifdef _OPENMP
    int nthreads = omp_get_max_threads();
  #else
    int nthreads = 1;
  #endif

  if (set->size >0) {

    // execute plan
    #pragma omp parallel for
    for ( int thr=0; thr<nthreads; thr++ ){
      int start  = (set->size* thr)/nthreads;
      int finish = (set->size*(thr+1))/nthreads;
      for ( int n=start; n<finish; n++ ){
        EvolveValuesRK2_1(
          (float*)arg0.data,
          &((float*)arg1.data)[4*n],
          &((float*)arg2.data)[4*n],
          &((float*)arg3.data)[4*n],
          &((float*)arg4.data)[4*n]);
      }
    }
  }

  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[0].name      = name;
  OP_kernels[0].count    += 1;
  OP_kernels[0].time     += wall_t2 - wall_t1;
  OP_kernels[0].transfer += (float)set->size * arg1.size * 2.0f;
  OP_kernels[0].transfer += (float)set->size * arg2.size;
  OP_kernels[0].transfer += (float)set->size * arg3.size;
  OP_kernels[0].transfer += (float)set->size * arg4.size;
}
