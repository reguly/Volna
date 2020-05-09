//
// auto-generated by op2.py
//

//user function
#include "../NumericalFluxes1.h"

// host stub function
void op_par_loop_NumericalFluxes1(char const *name, op_set set,
  op_arg arg0){

  int nargs = 1;
  op_arg args[1];

  args[0] = arg0;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(24);
  op_timers_core(&cpu_t1, &wall_t1);


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  NumericalFluxes1");
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
        NumericalFluxes1(
          &((float*)arg0.data)[4*n]);
      }
    }
  }

  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[24].name      = name;
  OP_kernels[24].count    += 1;
  OP_kernels[24].time     += wall_t2 - wall_t1;
  OP_kernels[24].transfer += (float)set->size * arg0.size * 2.0f;
}
