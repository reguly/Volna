//
// auto-generated by op2.m on 08-Mar-2013 15:38:25
//

// user function

__device__
#include "getMaxElevation.h"


// CUDA kernel function

__global__ void op_cuda_getMaxElevation(
  float *arg0,
  float *arg1,
  int   offset_s,
  int   set_size ) {

  float arg0_l[4];
  int   tid = threadIdx.x%OP_WARPSIZE;

  extern __shared__ char shared[];

  char *arg_s = shared + offset_s*(threadIdx.x/OP_WARPSIZE);

  // process set elements

  for (int n=threadIdx.x+blockIdx.x*blockDim.x;
       n<set_size; n+=blockDim.x*gridDim.x) {

    int offset = n - tid;
    int nelems = MIN(OP_WARPSIZE,set_size-offset);

    // copy data into shared memory, then into local

    for (int m=0; m<4; m++)
      ((float *)arg_s)[tid+m*nelems] = arg0[tid+m*nelems+offset*4];

    for (int m=0; m<4; m++)
      arg0_l[m] = ((float *)arg_s)[m+tid*4];


    // user-supplied kernel call


    getMaxElevation(  arg0_l,
                      arg1+n );

    // copy back into shared memory, then to device

  }
}


// host stub function

void op_par_loop_getMaxElevation(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1 ){


  int    nargs   = 2;
  op_arg args[2];

  args[0] = arg0;
  args[1] = arg1;

  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  getMaxElevation\n");
  }

  op_mpi_halo_exchanges_cuda(set, nargs, args);

  // initialise timers

  double cpu_t1, cpu_t2, wall_t1=0, wall_t2=0;
  op_timing_realloc(14);
  OP_kernels[14].name      = name;
  OP_kernels[14].count    += 1;

  if (set->size >0) {

    op_timers_core(&cpu_t1, &wall_t1);

    // set CUDA execution parameters

    #ifdef OP_BLOCK_SIZE_14
      int nthread = OP_BLOCK_SIZE_14;
    #else
      // int nthread = OP_block_size;
      int nthread = 128;
    #endif

    int nblocks = 200;

    // work out shared memory requirements per element

    int nshared = 0;
    nshared = MAX(nshared,sizeof(float)*4);

    // execute plan

    int offset_s = nshared*OP_WARPSIZE;

    nshared = nshared*nthread;

    op_cuda_getMaxElevation<<<nblocks,nthread,nshared>>>( (float *) arg0.data_d,
                                                          (float *) arg1.data_d,
                                                          offset_s,
                                                          set->size );

    cutilSafeCall(cudaDeviceSynchronize());
    cutilCheckMsg("op_cuda_getMaxElevation execution failed\n");

  }


  op_mpi_set_dirtybit_cuda(nargs, args);

  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[14].time     += wall_t2 - wall_t1;
  OP_kernels[14].transfer += (float)set->size * arg0.size;
  OP_kernels[14].transfer += (float)set->size * arg1.size * 2.0f;
}

