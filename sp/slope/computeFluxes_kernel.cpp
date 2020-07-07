//
// auto-generated by op2.py
//

//user function
#include "../computeFluxes.h"

// host stub function
void op_par_loop_computeFluxes_slope(char const *name, op_set set,
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
  op_arg arg10,
  op_arg arg11,
  op_arg arg12,
  op_arg arg13,
  op_arg arg14,
  tile_t* tile){

  int nargs = 15;
  op_arg args[15];

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
  args[10] = arg10;
  args[11] = arg11;
  args[12] = arg12;
  args[13] = arg13;
  args[14] = arg14;

  int tileLoopSize;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(23);
  op_timers_core(&cpu_t1, &wall_t1);


  int set_size = op_mpi_halo_exchanges(set, nargs, args);

  if (set->size >0) {

    iterations_list& le2c_2 = tile_get_local_map (tile, 2, "sl_edgesToCells");
    iterations_list& iterations_2 = tile_get_iterations (tile, 2);
    tileLoopSize = tile_loop_size (tile, 2);

    //#pragma omp simd
    for (int k = 0; k < tileLoopSize; k++) {

      int map0idx = le2c_2[k * N_CELLSPEREDGE + 0];
      int map1idx = le2c_2[k * N_CELLSPEREDGE + 1];
      int n = iterations_2[k];

      computeFluxes(
            &((float*)arg0.data)[4 * map0idx],
            &((float*)arg0.data)[4 * map1idx],
            &((float*)arg2.data)[4 * map0idx],
            &((float*)arg2.data)[4 * map1idx],
            &((float*)arg4.data)[1 * n],
            &((float*)arg5.data)[2 * n],
            &((float*)arg6.data)[2 * map0idx],
            &((float*)arg6.data)[2 * map1idx],
            &((float*)arg8.data)[2 * n],
            &((float*)arg9.data)[8 * map0idx],
            &((float*)arg9.data)[8 * map1idx],
            &((int*)arg11.data)[1 * n],
            &((float*)arg12.data)[4 * n],
            &((float*)arg13.data)[3 * n],
            &((float*)arg14.data)[1 * n]);
    }


    // execute plan
    /*int block_offset = 0;
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


          computeFluxes(
            &((float*)arg0.data)[4 * map0idx],
            &((float*)arg0.data)[4 * map1idx],
            &((float*)arg2.data)[4 * map0idx],
            &((float*)arg2.data)[4 * map1idx],
            &((float*)arg4.data)[1 * n],
            &((float*)arg5.data)[2 * n],
            &((float*)arg6.data)[2 * map0idx],
            &((float*)arg6.data)[2 * map1idx],
            &((float*)arg8.data)[2 * n],
            &((float*)arg9.data)[8 * map0idx],
            &((float*)arg9.data)[8 * map1idx],
            &((int*)arg11.data)[1 * n],
            &((float*)arg12.data)[4 * n],
            &((float*)arg13.data)[3 * n],
            &((float*)arg14.data)[1 * n]);
        }
      }

      block_offset += nblocks;
    }
    OP_kernels[23].transfer  += Plan->transfer;
    OP_kernels[23].transfer2 += Plan->transfer2;*/
  }

  if (set_size == 0 || set_size == set->core_size) {
    op_mpi_wait_all(nargs, args);
  }
  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[23].name      = name;
  OP_kernels[23].count    += 1;
  OP_kernels[23].time     += wall_t2 - wall_t1;
}
