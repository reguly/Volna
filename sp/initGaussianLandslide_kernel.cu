//
// auto-generated by op2.m on 12-Nov-2012 12:05:08
//

// user function

__device__
#include "initGaussianLandslide.h"


// CUDA kernel function

__global__ void op_cuda_initGaussianLandslide(
  float *arg0,
  float *arg1,
  const float *arg2,
  const float *arg3,
  const float *arg4,
  const float *arg5,
  const float *arg6,
  const float *arg7,
  int   offset_s,
  int   set_size ) {

  float arg0_l[2];
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

    for (int m=0; m<2; m++)
      ((float *)arg_s)[tid+m*nelems] = arg0[tid+m*nelems+offset*2];

    for (int m=0; m<2; m++)
      arg0_l[m] = ((float *)arg_s)[m+tid*2];

    for (int m=0; m<4; m++)
      ((float *)arg_s)[tid+m*nelems] = arg1[tid+m*nelems+offset*4];

    for (int m=0; m<4; m++)
      arg1_l[m] = ((float *)arg_s)[m+tid*4];


    // user-supplied kernel call


    initGaussianLandslide(  arg0_l,
                            arg1_l,
                            arg2,
                            arg3,
                            arg4,
                            arg5,
                            arg6,
                            arg7 );

    // copy back into shared memory, then to device

    for (int m=0; m<4; m++)
      ((float *)arg_s)[m+tid*4] = arg1_l[m];

    for (int m=0; m<4; m++)
      arg1[tid+m*nelems+offset*4] = ((float *)arg_s)[tid+m*nelems];

  }
}


// host stub function

void op_par_loop_initGaussianLandslide(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4,
  op_arg arg5,
  op_arg arg6,
  op_arg arg7 ){

  float *arg2h = (float *)arg2.data;
  float *arg3h = (float *)arg3.data;
  float *arg4h = (float *)arg4.data;
  float *arg5h = (float *)arg5.data;
  float *arg6h = (float *)arg6.data;
  float *arg7h = (float *)arg7.data;

  int    nargs   = 8;
  op_arg args[8];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;
  args[5] = arg5;
  args[6] = arg6;
  args[7] = arg7;

  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  initGaussianLandslide\n");
  }

  op_mpi_halo_exchanges(set, nargs, args);

  // initialise timers

  double cpu_t1, cpu_t2, wall_t1=0, wall_t2=0;
  op_timing_realloc(12);
  OP_kernels[12].name      = name;
  OP_kernels[12].count    += 1;

  if (set->size >0) {

    op_timers_core(&cpu_t1, &wall_t1);

    // transfer constants to GPU

    int consts_bytes = 0;
    consts_bytes += ROUND_UP(1*sizeof(float));
    consts_bytes += ROUND_UP(1*sizeof(float));
    consts_bytes += ROUND_UP(1*sizeof(float));
    consts_bytes += ROUND_UP(1*sizeof(float));
    consts_bytes += ROUND_UP(1*sizeof(float));
    consts_bytes += ROUND_UP(1*sizeof(float));

    reallocConstArrays(consts_bytes);

    consts_bytes = 0;
    arg2.data   = OP_consts_h + consts_bytes;
    arg2.data_d = OP_consts_d + consts_bytes;
    for (int d=0; d<1; d++) ((float *)arg2.data)[d] = arg2h[d];
    consts_bytes += ROUND_UP(1*sizeof(float));
    arg3.data   = OP_consts_h + consts_bytes;
    arg3.data_d = OP_consts_d + consts_bytes;
    for (int d=0; d<1; d++) ((float *)arg3.data)[d] = arg3h[d];
    consts_bytes += ROUND_UP(1*sizeof(float));
    arg4.data   = OP_consts_h + consts_bytes;
    arg4.data_d = OP_consts_d + consts_bytes;
    for (int d=0; d<1; d++) ((float *)arg4.data)[d] = arg4h[d];
    consts_bytes += ROUND_UP(1*sizeof(float));
    arg5.data   = OP_consts_h + consts_bytes;
    arg5.data_d = OP_consts_d + consts_bytes;
    for (int d=0; d<1; d++) ((float *)arg5.data)[d] = arg5h[d];
    consts_bytes += ROUND_UP(1*sizeof(float));
    arg6.data   = OP_consts_h + consts_bytes;
    arg6.data_d = OP_consts_d + consts_bytes;
    for (int d=0; d<1; d++) ((float *)arg6.data)[d] = arg6h[d];
    consts_bytes += ROUND_UP(1*sizeof(float));
    arg7.data   = OP_consts_h + consts_bytes;
    arg7.data_d = OP_consts_d + consts_bytes;
    for (int d=0; d<1; d++) ((float *)arg7.data)[d] = arg7h[d];
    consts_bytes += ROUND_UP(1*sizeof(float));

    mvConstArraysToDevice(consts_bytes);

    // set CUDA execution parameters

    #ifdef OP_BLOCK_SIZE_12
      int nthread = OP_BLOCK_SIZE_12;
    #else
      // int nthread = OP_block_size;
      int nthread = 128;
    #endif

    int nblocks = 200;

    // work out shared memory requirements per element

    int nshared = 0;
    nshared = MAX(nshared,sizeof(float)*2);
    nshared = MAX(nshared,sizeof(float)*4);

    // execute plan

    int offset_s = nshared*OP_WARPSIZE;

    nshared = nshared*nthread;

    op_cuda_initGaussianLandslide<<<nblocks,nthread,nshared>>>( (float *) arg0.data_d,
                                                                (float *) arg1.data_d,
                                                                (float *) arg2.data_d,
                                                                (float *) arg3.data_d,
                                                                (float *) arg4.data_d,
                                                                (float *) arg5.data_d,
                                                                (float *) arg6.data_d,
                                                                (float *) arg7.data_d,
                                                                offset_s,
                                                                set->size );

    cutilSafeCall(cudaThreadSynchronize());
    cutilCheckMsg("op_cuda_initGaussianLandslide execution failed\n");

  }


  op_mpi_set_dirtybit(nargs, args);

  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[12].time     += wall_t2 - wall_t1;
  OP_kernels[12].transfer += (float)set->size * arg0.size;
  OP_kernels[12].transfer += (float)set->size * arg1.size * 2.0f;
}

