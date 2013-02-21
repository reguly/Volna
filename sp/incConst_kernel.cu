//
// auto-generated by op2.m on 21-Feb-2013 15:19:34
//

// user function

__device__
#include "incConst.h"


// CUDA kernel function

__global__ void op_cuda_incConst(
  float *arg0,
  float *arg1,
  const int *arg2,
  int   offset_s,
  int   set_size ) {

  float arg1_l[4];
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
      ((float *)arg_s)[tid+m*nelems] = arg1[tid+m*nelems+offset*4];

    for (int m=0; m<4; m++)
      arg1_l[m] = ((float *)arg_s)[m+tid*4];


    // user-supplied kernel call


    incConst(  arg0+n,
               arg1_l,
               arg2 );

    // copy back into shared memory, then to device

    for (int m=0; m<4; m++)
      ((float *)arg_s)[m+tid*4] = arg1_l[m];

    for (int m=0; m<4; m++)
      arg1[tid+m*nelems+offset*4] = ((float *)arg_s)[tid+m*nelems];

  }
}


// host stub function

void op_par_loop_incConst(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2 ){

  int *arg2h = (int *)arg2.data;

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

  if (set->size >0) {

    op_timers_core(&cpu_t1, &wall_t1);

    // transfer constants to GPU

    int consts_bytes = 0;
    consts_bytes += ROUND_UP(1*sizeof(int));

    reallocConstArrays(consts_bytes);

    consts_bytes = 0;
    arg2.data   = OP_consts_h + consts_bytes;
    arg2.data_d = OP_consts_d + consts_bytes;
    for (int d=0; d<1; d++) ((int *)arg2.data)[d] = arg2h[d];
    consts_bytes += ROUND_UP(1*sizeof(int));

    mvConstArraysToDevice(consts_bytes);

    // set CUDA execution parameters

    #ifdef OP_BLOCK_SIZE_6
      int nthread = OP_BLOCK_SIZE_6;
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

    op_cuda_incConst<<<nblocks,nthread,nshared>>>( (float *) arg0.data_d,
                                                   (float *) arg1.data_d,
                                                   (int *) arg2.data_d,
                                                   offset_s,
                                                   set->size );

    cutilSafeCall(cudaDeviceSynchronize());
    cutilCheckMsg("op_cuda_incConst execution failed\n");

  }


  op_mpi_set_dirtybit(nargs, args);

  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[6].time     += wall_t2 - wall_t1;
  OP_kernels[6].transfer += (float)set->size * arg0.size;
  OP_kernels[6].transfer += (float)set->size * arg1.size * 2.0f;
}

