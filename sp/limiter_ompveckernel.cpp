//
// auto-generated by op2.py
//

//user function
inline void limiter(const float *q, float *q2,
  const float *value, const float *gradient,
  const float *edgecenter1, const float *edgecenter2,
  const float *edgecenter3, const float *cellcenter)
{

  float facevalue[3], dx[3], dy[3], y;
  int i, j;
  float edgealpha[3];

  dx[0] = (edgecenter1[0] - cellcenter[0]);
  dy[0] = (edgecenter1[1] - cellcenter[1]);
  dx[1] = (edgecenter2[0] - cellcenter[0]);
  dy[1] = (edgecenter2[1] - cellcenter[1]);
  dx[2] = (edgecenter3[0] - cellcenter[0]);
  dy[2] = (edgecenter3[1] - cellcenter[1]);
  // If the cell is not on the wet/dry boundary
  if((value[0] > EPS) && (q[0]> EPS)){
  // The limiter is calculated for all physical variables using the
  // Venkatakrishnan formula.
  // q[0] - Hmin , q[1] - Hmax , q2[0] - Hlim
  // q[2] - Umin , q[3] - Umax , q2[1] - Ulim
  // q[4] - Vmin , q[5] - Vmax , q2[2] - Vlim
  // q[6] - Zmin , q[7] - Zmax , q2[3] - Zlim
    for(j=0;j<4;j++){
      for(i =0 ; i<3; i++){
        facevalue[i] = value[j] + ((gradient[2*j]*dx[i]) + (gradient[2*j + 1]*dy[i]));
        if(facevalue[i] > q[2*j+1]) {
          y = (q[2*j + 1] - value[j]) / (facevalue[i] - value[j]);
          edgealpha[i] = (y*y + 2.0f*y) / (y*y + y + 2.0f);
        } else if (facevalue[i] < q[2*j]){
          y = (q[2*j] - value[j]) / (facevalue[i] - value[j]);
          edgealpha[i] = (y*y + 2.0f*y) / (y*y + y + 2.0f);
        } else{
          edgealpha[i] = 1.0f;
        }
      }
      q2[j] = edgealpha[0] < edgealpha[1] ? q2[j] : edgealpha[1];
      q2[j] = q2[j] < edgealpha[2] ? q2[j]: edgealpha[2];
      q2[j] = q2[j] < 1.0f ? q2[j] : 1.0f;
      q2[j] = q2[j] > 0.0f ? q2[j] : 0.0f;
      q2[j] = q2[j] / 3.0f;
    }
  } else {
    q2[0] = 0.0f;
    q2[1] = 0.0f;
    q2[2] = 0.0f;
    q2[3] = 0.0f;
  }
}
#ifdef VECTORIZE
//user function -- modified for vectorisation
void limiter_vec( const float *q, float *q2, const float *value, const float *gradient, const float edgecenter1[*][SIMD_VEC], const float edgecenter2[*][SIMD_VEC], const float edgecenter3[*][SIMD_VEC], const float *cellcenter, int idx ) {

  float facevalue[3], dx[3], dy[3], y;
  int i, j;
  float edgealpha[3];

  dx[0] = (edgecenter1[0][idx] - cellcenter[0]);
  dy[0] = (edgecenter1[1][idx] - cellcenter[1]);
  dx[1] = (edgecenter2[0][idx] - cellcenter[0]);
  dy[1] = (edgecenter2[1][idx] - cellcenter[1]);
  dx[2] = (edgecenter3[0][idx] - cellcenter[0]);
  dy[2] = (edgecenter3[1][idx] - cellcenter[1]);

  if((value[0] > EPS) && (q[0]> EPS)){






    for(j=0;j<4;j++){
      for(i =0 ; i<3; i++){
        facevalue[i] = value[j] + ((gradient[2*j]*dx[i]) + (gradient[2*j + 1]*dy[i]));
        if(facevalue[i] > q[2*j+1]) {
          y = (q[2*j + 1] - value[j]) / (facevalue[i] - value[j]);
          edgealpha[i] = (y*y + 2.0f*y) / (y*y + y + 2.0f);
        } else if (facevalue[i] < q[2*j]){
          y = (q[2*j] - value[j]) / (facevalue[i] - value[j]);
          edgealpha[i] = (y*y + 2.0f*y) / (y*y + y + 2.0f);
        } else{
          edgealpha[i] = 1.0f;
        }
      }
      q2[j] = edgealpha[0] < edgealpha[1] ? q2[j] : edgealpha[1];
      q2[j] = q2[j] < edgealpha[2] ? q2[j]: edgealpha[2];
      q2[j] = q2[j] < 1.0f ? q2[j] : 1.0f;
      q2[j] = q2[j] > 0.0f ? q2[j] : 0.0f;
      q2[j] = q2[j] / 3.0f;
    }
  } else {
    q2[0] = 0.0f;
    q2[1] = 0.0f;
    q2[2] = 0.0f;
    q2[3] = 0.0f;
  }
}
#endif

// host stub function
void op_par_loop_limiter(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4,
  op_arg arg5,
  op_arg arg6,
  op_arg arg7){

  int nargs = 8;
  op_arg args[8];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;
  args[5] = arg5;
  args[6] = arg6;
  args[7] = arg7;
  //create aligned pointers for dats
  ALIGNED_float const float * __restrict__ ptr0 = (float *) arg0.data;
  __assume_aligned(ptr0,float_ALIGN);
  ALIGNED_float       float * __restrict__ ptr1 = (float *) arg1.data;
  __assume_aligned(ptr1,float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr2 = (float *) arg2.data;
  __assume_aligned(ptr2,float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr3 = (float *) arg3.data;
  __assume_aligned(ptr3,float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr4 = (float *) arg4.data;
  __assume_aligned(ptr4,float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr5 = (float *) arg5.data;
  __assume_aligned(ptr5,float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr6 = (float *) arg6.data;
  __assume_aligned(ptr6,float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr7 = (float *) arg7.data;
  __assume_aligned(ptr7,float_ALIGN);

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(6);
  op_timers_core(&cpu_t1, &wall_t1);

  int  ninds   = 1;
  int  inds[8] = {-1,-1,-1,-1,0,0,0,-1};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: limiter\n");
  }

  #ifdef OP_PART_SIZE_6
    int part_size = OP_PART_SIZE_6;
  #else
    int part_size = OP_part_size;
  #endif


  int set_size = op_mpi_halo_exchanges(set, nargs, args);

  if (set_size >0) {

    // get plan
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
        #ifdef VECTORIZE
        //peel left remainder
        for ( int n=offset_b; n<((offset_b-1)/SIMD_VEC+1)*SIMD_VEC; n++ ){
          int map4idx = arg4.map_data[n * arg4.map->dim + 0];
          int map5idx = arg4.map_data[n * arg4.map->dim + 1];
          int map6idx = arg4.map_data[n * arg4.map->dim + 2];

          limiter(
            &(ptr0)[8 * n],
            &(ptr1)[4 * n],
            &(ptr2)[4 * n],
            &(ptr3)[8 * n],
            &(ptr4)[2 * map4idx],
            &(ptr5)[2 * map5idx],
            &(ptr6)[2 * map6idx],
            &(ptr7)[2 * n]);
        }
        #pragma novector
        for ( int n=((offset_b-1)/SIMD_VEC+1)*SIMD_VEC; n<((offset_b+nelem)/SIMD_VEC)*SIMD_VEC; n+=SIMD_VEC ){
          if (n+SIMD_VEC >= set->core_size) {
            op_mpi_wait_all(nargs, args);
          }
          ALIGNED_float float dat4[2][SIMD_VEC];
          ALIGNED_float float dat5[2][SIMD_VEC];
          ALIGNED_float float dat6[2][SIMD_VEC];
          #pragma omp simd aligned(ptr0,ptr1,ptr2,ptr3,ptr4,ptr5,ptr6,ptr7)
          for ( int i=0; i<SIMD_VEC; i++ ){
            int idx4_2 = 2 * arg4.map_data[(n+i) * arg4.map->dim + 0];
            int idx5_2 = 2 * arg4.map_data[(n+i) * arg4.map->dim + 1];
            int idx6_2 = 2 * arg4.map_data[(n+i) * arg4.map->dim + 2];

            dat4[0][i] = (ptr4)[idx4_2 + 0];
            dat4[1][i] = (ptr4)[idx4_2 + 1];

            dat5[0][i] = (ptr5)[idx5_2 + 0];
            dat5[1][i] = (ptr5)[idx5_2 + 1];

            dat6[0][i] = (ptr6)[idx6_2 + 0];
            dat6[1][i] = (ptr6)[idx6_2 + 1];

          }
          #pragma omp simd aligned(ptr0,ptr1,ptr2,ptr3,ptr4,ptr5,ptr6,ptr7)
          for ( int i=0; i<SIMD_VEC; i++ ){
            limiter_vec(
              &(ptr0)[8 * (n+i)],
              &(ptr1)[4 * (n+i)],
              &(ptr2)[4 * (n+i)],
              &(ptr3)[8 * (n+i)],
              dat4,
              dat5,
              dat6,
              &(ptr7)[2 * (n+i)],
              i);
          }
          for ( int i=0; i<SIMD_VEC; i++ ){

          }
        }

        //remainder
        for ( int n=((offset_b+nelem)/SIMD_VEC)*SIMD_VEC; n<offset_b+nelem; n++ ){
        #else
        #pragma omp simd aligned(ptr0,ptr1,ptr2,ptr3,ptr4,ptr5,ptr6,ptr7)
        for ( int n=offset_b; n<offset_b+nelem; n++ ){
        #endif
          int map4idx = arg4.map_data[n * arg4.map->dim + 0];
          int map5idx = arg4.map_data[n * arg4.map->dim + 1];
          int map6idx = arg4.map_data[n * arg4.map->dim + 2];

          limiter(
            &(ptr0)[8 * n],
            &(ptr1)[4 * n],
            &(ptr2)[4 * n],
            &(ptr3)[8 * n],
            &(ptr4)[2 * map4idx],
            &(ptr5)[2 * map5idx],
            &(ptr6)[2 * map6idx],
            &(ptr7)[2 * n]);
        }
      }
      block_offset += nblocks;
    }
  }

  if (set_size == 0 || set_size == set->core_size) {
    op_mpi_wait_all(nargs, args);
  }
  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[6].name      = name;
  OP_kernels[6].count    += 1;
  OP_kernels[6].time     += wall_t2 - wall_t1;
  OP_kernels[6].transfer += (float)set->size * arg4.size;
  OP_kernels[6].transfer += (float)set->size * arg0.size;
  OP_kernels[6].transfer += (float)set->size * arg1.size;
  OP_kernels[6].transfer += (float)set->size * arg2.size;
  OP_kernels[6].transfer += (float)set->size * arg3.size;
  OP_kernels[6].transfer += (float)set->size * arg7.size;
  OP_kernels[6].transfer += (float)set->size * arg4.map->dim * 4.0f;
}
#undef VECTORIZE
