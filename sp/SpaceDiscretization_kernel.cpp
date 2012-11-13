//
// auto-generated by op2.m on 13-Nov-2012 18:47:46
//

// user function

#include "SpaceDiscretization.h"


// x86 kernel function

void op_x86_SpaceDiscretization(
  int    blockIdx,
  float *ind_arg0,
  float *ind_arg1,
  int   *ind_map,
  short *arg_map,
  float *arg2,
  float *arg3,
  float *arg4,
  int *arg5,
  int   *ind_arg_sizes,
  int   *ind_arg_offs,
  int    block_offset,
  int   *blkmap,
  int   *offset,
  int   *nelems,
  int   *ncolors,
  int   *colors,
  int   set_size) {

  float arg0_l[4];
  float arg1_l[4];
  float *arg1_vec[2];

  int   *ind_arg0_map, ind_arg0_size;
  int   *ind_arg1_map, ind_arg1_size;
  float *ind_arg0_s;
  float *ind_arg1_s;
  int    nelem, offset_b;

  char shared[128000];

  if (0==0) {

    // get sizes and shift pointers and direct-mapped data

    int blockId = blkmap[blockIdx + block_offset];
    nelem    = nelems[blockId];
    offset_b = offset[blockId];

    ind_arg0_size = ind_arg_sizes[0+blockId*2];
    ind_arg1_size = ind_arg_sizes[1+blockId*2];

    ind_arg0_map = &ind_map[0*set_size] + ind_arg_offs[0+blockId*2];
    ind_arg1_map = &ind_map[2*set_size] + ind_arg_offs[1+blockId*2];

    // set shared memory pointers

    int nbytes = 0;
    ind_arg0_s = (float *) &shared[nbytes];
    nbytes    += ROUND_UP(ind_arg0_size*sizeof(float)*4);
    ind_arg1_s = (float *) &shared[nbytes];
  }

  // copy indirect datasets into shared memory or zero increment

  for (int n=0; n<ind_arg0_size; n++)
    for (int d=0; d<4; d++)
      ind_arg0_s[d+n*4] = ZERO_float;

  for (int n=0; n<ind_arg1_size; n++)
    for (int d=0; d<1; d++)
      ind_arg1_s[d+n*1] = ind_arg1[d+ind_arg1_map[n]*1];


  // process set elements

  for (int n=0; n<nelem; n++) {

    // initialise local variables

    for (int d=0; d<4; d++)
      arg0_l[d] = ZERO_float;
    for (int d=0; d<4; d++)
      arg1_l[d] = ZERO_float;

    arg1_vec[0] = ind_arg1_s+arg_map[2*set_size+n+offset_b]*1;
    arg1_vec[1] = ind_arg1_s+arg_map[3*set_size+n+offset_b]*1;

    // user-supplied kernel call


    SpaceDiscretization(  arg0_l,
                          arg1_l,
                          arg2+(n+offset_b)*3,
                          arg3+(n+offset_b)*2,
                          arg4+(n+offset_b)*2,
                          arg5+(n+offset_b)*1,
                          arg1_vec);

    // store local variables

    int arg0_map = arg_map[0*set_size+n+offset_b];
    int arg1_map = arg_map[1*set_size+n+offset_b];

    for (int d=0; d<4; d++)
      ind_arg0_s[d+arg0_map*4] += arg0_l[d];

    for (int d=0; d<4; d++)
      ind_arg0_s[d+arg1_map*4] += arg1_l[d];
  }

  // apply pointered write/increment

  for (int n=0; n<ind_arg0_size; n++)
    for (int d=0; d<4; d++)
      ind_arg0[d+ind_arg0_map[n]*4] += ind_arg0_s[d+n*4];

}


// host stub function

void op_par_loop_SpaceDiscretization(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4,
  op_arg arg5,
  op_arg arg6 ){


  int    nargs   = 8;
  op_arg args[8];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;
  args[5] = arg5;
  arg6.idx = 0;
  args[6] = arg6;
  for (int v = 1; v < 2; v++) {
    args[6 + v] = op_arg_dat(arg6.dat, v, arg6.map, 1, "float", OP_READ);
  }

  int    ninds   = 2;
  int    inds[8] = {0,0,-1,-1,-1,-1,1,1};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: SpaceDiscretization\n");
  }

  // get plan

  #ifdef OP_PART_SIZE_18
    int part_size = OP_PART_SIZE_18;
  #else
    int part_size = OP_part_size;
  #endif

  int set_size = op_mpi_halo_exchanges(set, nargs, args);

  // initialise timers

  double cpu_t1, cpu_t2, wall_t1=0, wall_t2=0;
  op_timing_realloc(18);
  OP_kernels[18].name      = name;
  OP_kernels[18].count    += 1;

  if (set->size >0) {

    op_plan *Plan = op_plan_get(name,set,part_size,nargs,args,ninds,inds);

    op_timers_core(&cpu_t1, &wall_t1);

    // execute plan

    int block_offset = 0;

    for (int col=0; col < Plan->ncolors; col++) {
      if (col==Plan->ncolors_core) op_mpi_wait_all(nargs, args);

      int nblocks = Plan->ncolblk[col];

#pragma omp parallel for
      for (int blockIdx=0; blockIdx<nblocks; blockIdx++)
      op_x86_SpaceDiscretization( blockIdx,
         (float *)arg0.data,
         (float *)arg6.data,
         Plan->ind_map,
         Plan->loc_map,
         (float *)arg2.data,
         (float *)arg3.data,
         (float *)arg4.data,
         (int *)arg5.data,
         Plan->ind_sizes,
         Plan->ind_offs,
         block_offset,
         Plan->blkmap,
         Plan->offset,
         Plan->nelems,
         Plan->nthrcol,
         Plan->thrcol,
         set_size);

      block_offset += nblocks;
    }

  op_timing_realloc(18);
  OP_kernels[18].transfer  += Plan->transfer;
  OP_kernels[18].transfer2 += Plan->transfer2;

  }


  // combine reduction data

  op_mpi_set_dirtybit(nargs, args);

  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[18].time     += wall_t2 - wall_t1;
}

