//
// auto-generated by op2.py
//

#include "../volna_common.h"

//user function
#include "../SpaceDiscretization.h"

#ifdef SLOPE
#include "executor.h"
#endif

void op_par_loop_SpaceDiscretization(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4,
  op_arg arg5,
  op_arg arg6,
  op_arg arg7,
  op_arg arg8,
  op_arg arg9){

  int nargs = 10;
  op_arg args[10];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;
  args[5] = arg5;
  args[6] = arg6;
  args[7] = arg7;
  args[8] = arg8;
  args[9] = arg9;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(25);
  op_timers_core(&cpu_t1, &wall_t1);

  int  ninds   = 3;
  int  inds[10] = {0,0,1,1,-1,-1,-1,-1,2,2};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: SpaceDiscretization\n");
  }

  // get plan
  #ifdef OP_PART_SIZE_25
    int part_size = OP_PART_SIZE_25;
  #else
    int part_size = OP_part_size;
  #endif

  int set_size = op_mpi_halo_exchanges(set, nargs, args);

  if (set->size >0) {

    op_plan *Plan = op_plan_get_stage_upload(name,set,part_size,nargs,args,ninds,inds,OP_STAGE_ALL,0);

    // execute plan
    int block_offset = 0;
    for ( int col=0; col<Plan->ncolors; col++ ){
      if (col==Plan->ncolors_core) {
        op_mpi_wait_all(nargs, args);
      }
      int nblocks = Plan->ncolblk[col];

      #pragma omp parallel for
      for ( int blockIdx=0; blockIdx<nblocks; blockIdx++ ){
        int blockId  = Plan->blkmap[blockIdx + block_offset];
        int nelem    = Plan->nelems[blockId];
        int offset_b = Plan->offset[blockId];
        for ( int n=offset_b; n<offset_b+nelem; n++ ){
          int map0idx = arg0.map_data[n * arg0.map->dim + 0];
          int map1idx = arg0.map_data[n * arg0.map->dim + 1];


          SpaceDiscretization(
            &((float*)arg0.data)[4 * map0idx],
            &((float*)arg0.data)[4 * map1idx],
            &((float*)arg2.data)[4 * map0idx],
            &((float*)arg2.data)[4 * map1idx],
            &((float*)arg4.data)[3 * n],
            &((float*)arg5.data)[4 * n],
            &((float*)arg6.data)[2 * n],
            &((int*)arg7.data)[1 * n],
            &((float*)arg8.data)[1 * map0idx],
            &((float*)arg8.data)[1 * map1idx]);
        }
      }

      block_offset += nblocks;
    }
    OP_kernels[25].transfer  += Plan->transfer;
    OP_kernels[25].transfer2 += Plan->transfer2;
  }

  if (set_size == 0 || set_size == set->core_size) {
    op_mpi_wait_all(nargs, args);
  }
  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[25].name      = name;
  OP_kernels[25].count    += 1;
  OP_kernels[25].time     += wall_t2 - wall_t1;
}

// host stub function
void op_par_loop_SpaceDiscretization_slope(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4,
  op_arg arg5,
  op_arg arg6,
  op_arg arg7,
  op_arg arg8,
  op_arg arg9,
  tile_t* tile){

  int nargs = 10;
  op_arg args[10];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;
  args[5] = arg5;
  args[6] = arg6;
  args[7] = arg7;
  args[8] = arg8;
  args[9] = arg9;

  int tileLoopSize;

  // initialise timers
  // double cpu_t1, cpu_t2, wall_t1, wall_t2;
  // op_timing_realloc(25);
  // op_timers_core(&cpu_t1, &wall_t1);
   sl_timers_core(&(sl_kernels[4].cpu_t1[omp_get_thread_num()]), &(sl_kernels[4].wall_t1[omp_get_thread_num()]));


  int set_size = op_mpi_halo_exchanges(set, nargs, args);

  if (set->size >0) {

    // loop SpaceDiscretization
    iterations_list& le2c_4 = tile_get_local_map (tile, 4, "sl_edgesToCells");
    iterations_list& iterations_4 = tile_get_iterations (tile, 4);
    tileLoopSize = tile_loop_size (tile, 4);

    for (int k = 0; k < tileLoopSize; k++) {

      int map0idx = le2c_4[k * N_CELLSPEREDGE + 0];
      int map1idx = le2c_4[k * N_CELLSPEREDGE + 1];
      int n = iterations_4[k];

      SpaceDiscretization(
            &((float*)arg0.data)[4 * map0idx],
            &((float*)arg0.data)[4 * map1idx],
            &((float*)arg2.data)[4 * map0idx],
            &((float*)arg2.data)[4 * map1idx],
            &((float*)arg4.data)[3 * n],
            &((float*)arg5.data)[4 * n],
            &((float*)arg6.data)[2 * n],
            &((int*)arg7.data)[1 * n],
            &((float*)arg8.data)[1 * map0idx],
            &((float*)arg8.data)[1 * map1idx]);

    }
  }

  if (set_size == 0 || set_size == set->core_size) {
    op_mpi_wait_all(nargs, args);
  }
  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  sl_kernels[4].name      = name;
  sl_timers_core(&(sl_kernels[4].cpu_t2[omp_get_thread_num()]), &(sl_kernels[4].wall_t2[omp_get_thread_num()]));
  sl_kernels[4].counts[omp_get_thread_num()]    += 1;
  sl_kernels[4].times[omp_get_thread_num()]     += sl_kernels[4].wall_t2[omp_get_thread_num()] - sl_kernels[4].wall_t1[omp_get_thread_num()];
  

  // update kernel record
  // op_timers_core(&cpu_t2, &wall_t2);
  // if(omp_get_thread_num() == TID) {
  //   OP_kernels[25].name      = name;
  //   OP_kernels[25].count    += 1;
  //   OP_kernels[25].time     += wall_t2 - wall_t1;
  // }
}

// host stub function
void op_par_loop_SpaceDiscretization_slope_1(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4,
  op_arg arg5,
  op_arg arg6,
  op_arg arg7,
  op_arg arg8,
  op_arg arg9,
  tile_t* tile){

  int nargs = 10;
  op_arg args[10];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;
  args[5] = arg5;
  args[6] = arg6;
  args[7] = arg7;
  args[8] = arg8;
  args[9] = arg9;

  int tileLoopSize;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(25);
  op_timers_core(&cpu_t1, &wall_t1);

  int set_size = op_mpi_halo_exchanges(set, nargs, args);

  if (set->size >0) {

    // loop SpaceDiscretization
    iterations_list& le2c_4 = tile_get_local_map (tile, 4, "sl_edgesToCells");
    iterations_list& iterations_4 = tile_get_iterations (tile, 4);
    tileLoopSize = tile_loop_size (tile, 4);

    for (int k = 0; k < tileLoopSize; k++) {

      int map0idx = le2c_4[k * N_CELLSPEREDGE + 0];
      int map1idx = le2c_4[k * N_CELLSPEREDGE + 1];
      int n = iterations_4[k];

      SpaceDiscretization(
            &((float*)arg0.data)[4 * map0idx],
            &((float*)arg0.data)[4 * map1idx],
            &((float*)arg2.data)[4 * map0idx],
            &((float*)arg2.data)[4 * map1idx],
            &((float*)arg4.data)[3 * n],
            &((float*)arg5.data)[4 * n],
            &((float*)arg6.data)[2 * n],
            &((int*)arg7.data)[1 * n],
            &((float*)arg8.data)[1 * map0idx],
            &((float*)arg8.data)[1 * map1idx]);

    }
  }

  if (set_size == 0 || set_size == set->core_size) {
    op_mpi_wait_all(nargs, args);
  }
  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  if(omp_get_thread_num() == TID) {
    OP_kernels[25].name      = name;
    OP_kernels[25].count    += 1;
    OP_kernels[25].time     += wall_t2 - wall_t1;
  }
}
