//
// auto-generated by op2.py
//

//user function
inline void computeFluxes(const float *cellLeft, const float *cellRight,
                                const float *alphaleft, const float *alpharight,
                                const float *edgeLength, const float *edgeNormals,
                                const float *leftcellCenters, const float *rightcellCenters,
                                const float *edgeCenters,
                                const float *leftGradient, const float *rightGradient,
                                const int *isRightBoundary, //OP_READ
                                float *bathySource, float *out, //OP_WRITE
                                float *maxEdgeEigenvalues) //OP_WRITE
{
  //begin EdgesValuesFromCellValues
  float leftCellValues[4];
  float rightCellValues[4];
  float InterfaceBathy;
  leftCellValues[0] = cellLeft[0];
  leftCellValues[1] = cellLeft[1];
  leftCellValues[2] = cellLeft[2];
  leftCellValues[3] = cellLeft[3];
  float dxl, dyl, dxr, dyr;
  dxl = (edgeCenters[0] - leftcellCenters[0]);
  dyl = (edgeCenters[1] - leftcellCenters[1]);

  dxr = (edgeCenters[0] - rightcellCenters[0]);
  dyr = (edgeCenters[1] - rightcellCenters[1]);

  if (!isRightBoundary[0]) {
    rightCellValues[0] = cellRight[0];
    rightCellValues[1] = cellRight[1];
    rightCellValues[2] = cellRight[2];
    rightCellValues[3] = cellRight[3];
  } else {
    rightCellValues[3] = cellLeft[3];
    float nx = edgeNormals[0];
    float ny = edgeNormals[1];
    float inNormalVelocity = cellLeft[1] * nx + cellLeft[2] * ny;
    float inTangentVelocity = -1.0f *  cellLeft[1] * ny + cellLeft[2] * nx;
    float outNormalVelocity = 0.0f;
    float outTangentVelocity = 0.0f;

    //WALL
    rightCellValues[0] = cellLeft[0];
    outNormalVelocity = -1.0f * inNormalVelocity;
    outTangentVelocity = inTangentVelocity;


    /* //HEIGHTSUBC
     rightCellValues[0] = -1.0 * rightCellValues[3];
     rightCellValues[0] += 0.1 * sin(10.0*t);
     outNormalVelocity = inNormalVelocity;
     outNormalVelocity +=
     2.0 * sqrt( g * cellLeft[0] );
     outNormalVelocity -=
     2.0 * sqrt( g * rightCellValues[0] );

     outTangentVelocity = inTangentVelocity;
     */ //end HEIGHTSUBC

    /* //FLOWSUBC
     outNormalVelocity = 1;

     //rightCellValues[0] = - rightCellValues[3];

     rightCellValues[0] = (inNormalVelocity - outNormalVelocity);
     rightCellValues[0] *= .5 / sqrt( g );

     rightCellValues[0] += sqrt( cellLeft[0] );

     outTangentVelocity = inTangentVelocity;
     */
    rightCellValues[1] = outNormalVelocity * nx - outTangentVelocity * ny;
    rightCellValues[2] = outNormalVelocity * ny + outTangentVelocity * nx;
  }

  // ------------------------------------------------------------------------------------
  // Audusse Reconstruction(2004) Source Discretization
  InterfaceBathy = leftCellValues[3] > rightCellValues[3] ? leftCellValues[3] : rightCellValues[3];
  bathySource[0] = .5f * g * (leftCellValues[0]*leftCellValues[0]);
  bathySource[1] = .5f * g * (rightCellValues[0]*rightCellValues[0]);

  float hL = (leftCellValues[0] + leftCellValues[3] - InterfaceBathy);
  hL = hL > 0.0f? hL : 0.0f;
  float hR = (rightCellValues[0] + rightCellValues[3] - InterfaceBathy);
  hR = hR > 0.0f ? hR : 0.0f;
  bathySource[0] -= .5f * g * (hL * hL);
  bathySource[1] -= .5f * g * (hR * hR);

  bathySource[0] *= *edgeLength;
  bathySource[1] *= *edgeLength;

  // Second order Reconstruction
  if (!isRightBoundary[0]) {
  leftCellValues[0] +=  alphaleft[0] * ((dxl * leftGradient[0])+(dyl * leftGradient[1]));
  rightCellValues[0] +=  alpharight[0] * ((dxr * rightGradient[0])+(dyr * rightGradient[1]));
  leftCellValues[0] = leftCellValues[0] > 0.0f ? leftCellValues[0] : 0.0f;
  rightCellValues[0] = rightCellValues[0] > 0.0f ? rightCellValues[0] : 0.0f;

  leftCellValues[3] += alphaleft[3] * ((dxl * leftGradient[6])+(dyl * leftGradient[7]));
  rightCellValues[3] += alpharight[3] * ((dxr * rightGradient[6])+(dyr * rightGradient[7]));
  InterfaceBathy = leftCellValues[3] > rightCellValues[3] ? leftCellValues[3] : rightCellValues[3];
  leftCellValues[0] = (leftCellValues[0] + leftCellValues[3] - InterfaceBathy);
  leftCellValues[0] = leftCellValues[0] > 0.0f ? leftCellValues[0] : 0.0f;
  rightCellValues[0] = (rightCellValues[0] + rightCellValues[3] - InterfaceBathy);
  rightCellValues[0] = rightCellValues[0] > 0.0f ? rightCellValues[0] : 0.0f;

  leftCellValues[1] += alphaleft[1] * ((dxl * leftGradient[2])+(dyl * leftGradient[3]));
  leftCellValues[2] += alphaleft[2] * ((dxl * leftGradient[4])+(dyl * leftGradient[5]));

  rightCellValues[1] += alpharight[1] * ((dxr * rightGradient[2])+(dyr * rightGradient[3]));
  rightCellValues[2] += alpharight[2] * ((dxr * rightGradient[4])+(dyr * rightGradient[5]));
  }

  // ------------------------------------------------------------------------------------
  // HLL Riemann Solver
  // Estimation of the wave speeds at the interface.
  float cL = sqrt(g * leftCellValues[0]);
  cL = cL > 0.0f ? cL : 0.0f;
  float cR = sqrt(g * rightCellValues[0]);
  cR = cR > 0.0f ? cR : 0.0f;
  float uLn = leftCellValues[1] * edgeNormals[0] + leftCellValues[2] * edgeNormals[1];
  float uRn = rightCellValues[1] * edgeNormals[0] + rightCellValues[2] * edgeNormals[1];

  float unStar = 0.5f * (uLn + uRn) + (cL-cR);
  float cStar = 0.5f * (cL + cR) - 0.25f* (uRn-uLn);
  float sL = (uLn - cL) < (unStar - cStar) ? (uLn - cL) : (unStar - cStar);
  float sR = (uRn + cR) > (unStar + cStar) ? (uRn + cR) : (unStar + cStar);

  // sStar is needed for the HLLC extension.
  float sStar;
  sStar = (sL*rightCellValues[0]*(uRn - sR) - sR*leftCellValues[0]*(uLn - sL))/
          (rightCellValues[0]*(uRn - sR) - leftCellValues[0]*(uLn - sL));

  // Wet/dry implementation
  /*if ( (leftCellValues[0] <= EPS) && (rightCellValues[0] <= EPS)) {
      sL = 0.0f;
      sR = 0.0f;
  }
  if ((leftCellValues[0] <= EPS) && (rightCellValues[0] > EPS)) {
      sL = uRn - 2.0f*cR;
      sR = uRn + cR;
      sStar = sL;
  }
  if ((rightCellValues[0] <= EPS) && (leftCellValues[0] > EPS)) {
      sR = uLn + 2.0f*cL;
      sL =  uLn - cL;
      sStar = sR;
  }*/

  float sLMinus = sL < 0.0f ? sL : 0.0f;
  float sRPlus = sR > 0.0f ? sR : 0.0f;
  float sRMinussL = sRPlus - sLMinus;
  sRMinussL = sRMinussL < EPS ?  EPS : sRMinussL;
  //-------------------------------------------------
  float t1 = sRPlus / sRMinussL;
  //assert( ( 0 <= t1 ) && ( t1 <= 1 ) );

  float t2 = ( -1.0 * sLMinus ) / sRMinussL;
  //assert( ( 0 <= t2 ) && ( t2 <= 1 ) );

  float t3 = ( sRPlus * sLMinus ) / sRMinussL;
  // ------------------------------------------------------------------------------------
  // Velocities parallel to the interface.
  float uLp = leftCellValues[2]*edgeNormals[0] - leftCellValues[1]*edgeNormals[1];
  float uRp = rightCellValues[2]*edgeNormals[0] - rightCellValues[1]*edgeNormals[1];


  float LeftFluxes_H, LeftFluxes_U, LeftFluxes_V, LeftFluxes_N;
  //inlined ProjectedPhysicalFluxes(leftCellValues, Normals, params, LeftFluxes);
  float HuDotN = (leftCellValues[0] * leftCellValues[1]) * edgeNormals[0] +
  (leftCellValues[0] * leftCellValues[2]) * edgeNormals[1];

  LeftFluxes_H = HuDotN;
  LeftFluxes_U = HuDotN * leftCellValues[1];
  LeftFluxes_V = HuDotN * leftCellValues[2];
  // Normal Momentum flux term
  LeftFluxes_N = HuDotN * uLn;

  LeftFluxes_U += (.5f * g * edgeNormals[0] ) * ( leftCellValues[0] * leftCellValues[0] );
  LeftFluxes_V += (.5f * g * edgeNormals[1] ) * ( leftCellValues[0] * leftCellValues[0] );
  LeftFluxes_N += (.5f * g ) * ( leftCellValues[0] * leftCellValues[0] );
  //end of inlined

  float RightFluxes_H,RightFluxes_U, RightFluxes_V, RightFluxes_N;
  //inlined ProjectedPhysicalFluxes(rightCellValues, Normals, params, RightFluxes);
  HuDotN = (rightCellValues[0] * rightCellValues[1] * edgeNormals[0]) +
  (rightCellValues[0] * rightCellValues[2] * edgeNormals[1]);

  RightFluxes_H =   HuDotN;
  RightFluxes_U =   HuDotN * rightCellValues[1];
  RightFluxes_V =   HuDotN * rightCellValues[2];
  // Normal Momentum flux term
  RightFluxes_N =   HuDotN * uRn;

  RightFluxes_U += (.5f * g * edgeNormals[0] ) * ( rightCellValues[0] * rightCellValues[0] );
  RightFluxes_V += (.5f * g * edgeNormals[1] ) * ( rightCellValues[0] * rightCellValues[0] );
  RightFluxes_N += (.5f * g) * ( rightCellValues[0] * rightCellValues[0] );

  float FStar[3];
  FStar[0] =
  ( t1 * LeftFluxes_H ) +
  ( t2 * RightFluxes_H ) +
  ( t3 * ( rightCellValues[0] - leftCellValues[0] ) );


  FStar[1] =
  ( t1 * LeftFluxes_N ) +
  ( t2 * RightFluxes_N ) +
  ( t3 * ( (rightCellValues[0] * uRn) -
          (leftCellValues[0] * uLn) ) );

  // ------------------------------------------------------------------------
  // HLLC Flux Solver
  if( sL >= 0.0f) {
    out[0] = LeftFluxes_H;
    out[1] = LeftFluxes_U;
    out[2] = LeftFluxes_V;
  } else if ((sL < 0.0f) && (sStar >= 0.0f)){
    out[0] = FStar[0];
    FStar[2] = FStar[0] * uLp;
    out[1] = FStar[1]*edgeNormals[0] - FStar[2]*edgeNormals[1];
    out[2] = FStar[1]*edgeNormals[1] + FStar[2]*edgeNormals[0];
  } else if((sStar < 0.0f) && (sR >= 0.0f)){
    out[0] = FStar[0];
    FStar[2] = FStar[0] * uRp;
    out[1] = FStar[1]*edgeNormals[0] - FStar[2]*edgeNormals[1];
    out[2] = FStar[1]*edgeNormals[1] + FStar[2]*edgeNormals[0];
  } else {
    out[0] = RightFluxes_H;
    out[1] = RightFluxes_U;
    out[2] = RightFluxes_V;
  }
  out[0] *= *edgeLength;
  out[1] *= *edgeLength;
  out[2] *= *edgeLength;
  float maximum = fabs(uLn + cL);
  maximum = maximum > fabs(uLn - cL) ? maximum : fabs(uLn - cL);
  maximum = maximum > fabs(uRn + cR) ? maximum : fabs(uRn + cR);
  maximum = maximum > fabs(uRn - cR) ? maximum : fabs(uRn - cR);
  maxEdgeEigenvalues[0] = maximum;
}
#ifdef VECTORIZE
//user function -- modified for vectorisation
void computeFluxes_vec( const float cellLeft[*][SIMD_VEC], const float cellRight[*][SIMD_VEC], const float alphaleft[*][SIMD_VEC], const float alpharight[*][SIMD_VEC], const float *edgeLength, const float *edgeNormals, const float leftcellCenters[*][SIMD_VEC], const float rightcellCenters[*][SIMD_VEC], const float *edgeCenters, const float leftGradient[*][SIMD_VEC], const float rightGradient[*][SIMD_VEC], const int *isRightBoundary, float *bathySource, float *out, float *maxEdgeEigenvalues, int idx ) {

  float leftCellValues[4];
  float rightCellValues[4];
  float InterfaceBathy;
  leftCellValues[0] = cellLeft[0][idx];
  leftCellValues[1] = cellLeft[1][idx];
  leftCellValues[2] = cellLeft[2][idx];
  leftCellValues[3] = cellLeft[3][idx];
  float dxl, dyl, dxr, dyr;
  dxl = (edgeCenters[0] - leftcellCenters[0][idx]);
  dyl = (edgeCenters[1] - leftcellCenters[1][idx]);

  dxr = (edgeCenters[0] - rightcellCenters[0][idx]);
  dyr = (edgeCenters[1] - rightcellCenters[1][idx]);

  if (!isRightBoundary[0]) {
    rightCellValues[0] = cellRight[0][idx];
    rightCellValues[1] = cellRight[1][idx];
    rightCellValues[2] = cellRight[2][idx];
    rightCellValues[3] = cellRight[3][idx];
  } else {
    rightCellValues[3] = cellLeft[3][idx];
    float nx = edgeNormals[0];
    float ny = edgeNormals[1];
    float inNormalVelocity = cellLeft[1][idx] * nx + cellLeft[2][idx] * ny;
    float inTangentVelocity = -1.0f *  cellLeft[1][idx] * ny + cellLeft[2][idx] * nx;
    float outNormalVelocity = 0.0f;
    float outTangentVelocity = 0.0f;

    rightCellValues[0] = cellLeft[0][idx];
    outNormalVelocity = -1.0f * inNormalVelocity;
    outTangentVelocity = inTangentVelocity;



    rightCellValues[1] = outNormalVelocity * nx - outTangentVelocity * ny;
    rightCellValues[2] = outNormalVelocity * ny + outTangentVelocity * nx;
  }


  InterfaceBathy = leftCellValues[3] > rightCellValues[3] ? leftCellValues[3] : rightCellValues[3];
  bathySource[0] = .5f * g * (leftCellValues[0]*leftCellValues[0]);
  bathySource[1] = .5f * g * (rightCellValues[0]*rightCellValues[0]);

  float hL = (leftCellValues[0] + leftCellValues[3] - InterfaceBathy);
  hL = hL > 0.0f? hL : 0.0f;
  float hR = (rightCellValues[0] + rightCellValues[3] - InterfaceBathy);
  hR = hR > 0.0f ? hR : 0.0f;
  bathySource[0] -= .5f * g * (hL * hL);
  bathySource[1] -= .5f * g * (hR * hR);

  bathySource[0] *= *edgeLength;
  bathySource[1] *= *edgeLength;

  if (!isRightBoundary[0]) {
  leftCellValues[0] +=  alphaleft[0][idx] * ((dxl * leftGradient[0][idx])+(dyl * leftGradient[1][idx]));
  rightCellValues[0] +=  alpharight[0][idx] * ((dxr * rightGradient[0][idx])+(dyr * rightGradient[1][idx]));
  leftCellValues[0] = leftCellValues[0] > 0.0f ? leftCellValues[0] : 0.0f;
  rightCellValues[0] = rightCellValues[0] > 0.0f ? rightCellValues[0] : 0.0f;

  leftCellValues[3] += alphaleft[3][idx] * ((dxl * leftGradient[6][idx])+(dyl * leftGradient[7][idx]));
  rightCellValues[3] += alpharight[3][idx] * ((dxr * rightGradient[6][idx])+(dyr * rightGradient[7][idx]));
  InterfaceBathy = leftCellValues[3] > rightCellValues[3] ? leftCellValues[3] : rightCellValues[3];
  leftCellValues[0] = (leftCellValues[0] + leftCellValues[3] - InterfaceBathy);
  leftCellValues[0] = leftCellValues[0] > 0.0f ? leftCellValues[0] : 0.0f;
  rightCellValues[0] = (rightCellValues[0] + rightCellValues[3] - InterfaceBathy);
  rightCellValues[0] = rightCellValues[0] > 0.0f ? rightCellValues[0] : 0.0f;

  leftCellValues[1] += alphaleft[1][idx] * ((dxl * leftGradient[2][idx])+(dyl * leftGradient[3][idx]));
  leftCellValues[2] += alphaleft[2][idx] * ((dxl * leftGradient[4][idx])+(dyl * leftGradient[5][idx]));

  rightCellValues[1] += alpharight[1][idx] * ((dxr * rightGradient[2][idx])+(dyr * rightGradient[3][idx]));
  rightCellValues[2] += alpharight[2][idx] * ((dxr * rightGradient[4][idx])+(dyr * rightGradient[5][idx]));
  }



  float cL = sqrt(g * leftCellValues[0]);
  cL = cL > 0.0f ? cL : 0.0f;
  float cR = sqrt(g * rightCellValues[0]);
  cR = cR > 0.0f ? cR : 0.0f;
  float uLn = leftCellValues[1] * edgeNormals[0] + leftCellValues[2] * edgeNormals[1];
  float uRn = rightCellValues[1] * edgeNormals[0] + rightCellValues[2] * edgeNormals[1];

  float unStar = 0.5f * (uLn + uRn) + (cL-cR);
  float cStar = 0.5f * (cL + cR) - 0.25f* (uRn-uLn);
  float sL = (uLn - cL) < (unStar - cStar) ? (uLn - cL) : (unStar - cStar);
  float sR = (uRn + cR) > (unStar + cStar) ? (uRn + cR) : (unStar + cStar);

  float sStar;
  sStar = (sL*rightCellValues[0]*(uRn - sR) - sR*leftCellValues[0]*(uLn - sL))/
          (rightCellValues[0]*(uRn - sR) - leftCellValues[0]*(uLn - sL));



  float sLMinus = sL < 0.0f ? sL : 0.0f;
  float sRPlus = sR > 0.0f ? sR : 0.0f;
  float sRMinussL = sRPlus - sLMinus;
  sRMinussL = sRMinussL < EPS ?  EPS : sRMinussL;

  float t1 = sRPlus / sRMinussL;


  float t2 = ( -1.0 * sLMinus ) / sRMinussL;


  float t3 = ( sRPlus * sLMinus ) / sRMinussL;


  float uLp = leftCellValues[2]*edgeNormals[0] - leftCellValues[1]*edgeNormals[1];
  float uRp = rightCellValues[2]*edgeNormals[0] - rightCellValues[1]*edgeNormals[1];

  float LeftFluxes_H, LeftFluxes_U, LeftFluxes_V, LeftFluxes_N;

  float HuDotN = (leftCellValues[0] * leftCellValues[1]) * edgeNormals[0] +
  (leftCellValues[0] * leftCellValues[2]) * edgeNormals[1];

  LeftFluxes_H = HuDotN;
  LeftFluxes_U = HuDotN * leftCellValues[1];
  LeftFluxes_V = HuDotN * leftCellValues[2];

  LeftFluxes_N = HuDotN * uLn;

  LeftFluxes_U += (.5f * g * edgeNormals[0] ) * ( leftCellValues[0] * leftCellValues[0] );
  LeftFluxes_V += (.5f * g * edgeNormals[1] ) * ( leftCellValues[0] * leftCellValues[0] );
  LeftFluxes_N += (.5f * g ) * ( leftCellValues[0] * leftCellValues[0] );


  float RightFluxes_H,RightFluxes_U, RightFluxes_V, RightFluxes_N;

  HuDotN = (rightCellValues[0] * rightCellValues[1] * edgeNormals[0]) +
  (rightCellValues[0] * rightCellValues[2] * edgeNormals[1]);

  RightFluxes_H =   HuDotN;
  RightFluxes_U =   HuDotN * rightCellValues[1];
  RightFluxes_V =   HuDotN * rightCellValues[2];

  RightFluxes_N =   HuDotN * uRn;

  RightFluxes_U += (.5f * g * edgeNormals[0] ) * ( rightCellValues[0] * rightCellValues[0] );
  RightFluxes_V += (.5f * g * edgeNormals[1] ) * ( rightCellValues[0] * rightCellValues[0] );
  RightFluxes_N += (.5f * g) * ( rightCellValues[0] * rightCellValues[0] );

  float FStar[3];
  FStar[0] =
  ( t1 * LeftFluxes_H ) +
  ( t2 * RightFluxes_H ) +
  ( t3 * ( rightCellValues[0] - leftCellValues[0] ) );

  FStar[1] =
  ( t1 * LeftFluxes_N ) +
  ( t2 * RightFluxes_N ) +
  ( t3 * ( (rightCellValues[0] * uRn) -
          (leftCellValues[0] * uLn) ) );


  if( sL >= 0.0f) {
    out[0] = LeftFluxes_H;
    out[1] = LeftFluxes_U;
    out[2] = LeftFluxes_V;
  } else if ((sL < 0.0f) && (sStar >= 0.0f)){
    out[0] = FStar[0];
    FStar[2] = FStar[0] * uLp;
    out[1] = FStar[1]*edgeNormals[0] - FStar[2]*edgeNormals[1];
    out[2] = FStar[1]*edgeNormals[1] + FStar[2]*edgeNormals[0];
  } else if((sStar < 0.0f) && (sR >= 0.0f)){
    out[0] = FStar[0];
    FStar[2] = FStar[0] * uRp;
    out[1] = FStar[1]*edgeNormals[0] - FStar[2]*edgeNormals[1];
    out[2] = FStar[1]*edgeNormals[1] + FStar[2]*edgeNormals[0];
  } else {
    out[0] = RightFluxes_H;
    out[1] = RightFluxes_U;
    out[2] = RightFluxes_V;
  }
  out[0] *= *edgeLength;
  out[1] *= *edgeLength;
  out[2] *= *edgeLength;
  float maximum = fabs(uLn + cL);
  maximum = maximum > fabs(uLn - cL) ? maximum : fabs(uLn - cL);
  maximum = maximum > fabs(uRn + cR) ? maximum : fabs(uRn + cR);
  maximum = maximum > fabs(uRn - cR) ? maximum : fabs(uRn - cR);
  maxEdgeEigenvalues[0] = maximum;
}
#endif

// host stub function
void op_par_loop_computeFluxes(char const *name, op_set set,
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
  op_arg arg14){

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
  //create aligned pointers for dats
  ALIGNED_float const float * __restrict__ ptr0 = (float *) arg0.data;
  __assume_aligned(ptr0,float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr1 = (float *) arg1.data;
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
  ALIGNED_float const float * __restrict__ ptr8 = (float *) arg8.data;
  __assume_aligned(ptr8,float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr9 = (float *) arg9.data;
  __assume_aligned(ptr9,float_ALIGN);
  ALIGNED_float const float * __restrict__ ptr10 = (float *) arg10.data;
  __assume_aligned(ptr10,float_ALIGN);
  ALIGNED_int const int * __restrict__ ptr11 = (int *) arg11.data;
  __assume_aligned(ptr11,int_ALIGN);
  ALIGNED_float       float * __restrict__ ptr12 = (float *) arg12.data;
  __assume_aligned(ptr12,float_ALIGN);
  ALIGNED_float       float * __restrict__ ptr13 = (float *) arg13.data;
  __assume_aligned(ptr13,float_ALIGN);
  ALIGNED_float       float * __restrict__ ptr14 = (float *) arg14.data;
  __assume_aligned(ptr14,float_ALIGN);

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(7);
  op_timers_core(&cpu_t1, &wall_t1);

  int  ninds   = 4;
  int  inds[15] = {0,0,1,1,-1,-1,2,2,-1,3,3,-1,-1,-1,-1};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: computeFluxes\n");
  }

  #ifdef OP_PART_SIZE_7
    int part_size = OP_PART_SIZE_7;
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
          int map0idx = arg0.map_data[n * arg0.map->dim + 0];
          int map1idx = arg0.map_data[n * arg0.map->dim + 1];

          computeFluxes(
            &(ptr0)[4 * map0idx],
            &(ptr1)[4 * map1idx],
            &(ptr2)[4 * map0idx],
            &(ptr3)[4 * map1idx],
            &(ptr4)[1 * n],
            &(ptr5)[2 * n],
            &(ptr6)[2 * map0idx],
            &(ptr7)[2 * map1idx],
            &(ptr8)[2 * n],
            &(ptr9)[8 * map0idx],
            &(ptr10)[8 * map1idx],
            &(ptr11)[1 * n],
            &(ptr12)[4 * n],
            &(ptr13)[3 * n],
            &(ptr14)[1 * n]);
        }
        #pragma novector
        for ( int n=((offset_b-1)/SIMD_VEC+1)*SIMD_VEC; n<((offset_b+nelem)/SIMD_VEC)*SIMD_VEC; n+=SIMD_VEC ){
          if (n+SIMD_VEC >= set->core_size) {
            op_mpi_wait_all(nargs, args);
          }
          ALIGNED_float float dat0[4][SIMD_VEC];
          ALIGNED_float float dat1[4][SIMD_VEC];
          ALIGNED_float float dat2[4][SIMD_VEC];
          ALIGNED_float float dat3[4][SIMD_VEC];
          ALIGNED_float float dat6[2][SIMD_VEC];
          ALIGNED_float float dat7[2][SIMD_VEC];
          ALIGNED_float float dat9[8][SIMD_VEC];
          ALIGNED_float float dat10[8][SIMD_VEC];
          #pragma omp simd aligned(ptr0,ptr1,ptr2,ptr3,ptr4,ptr5,ptr6,ptr7,ptr8,ptr9,ptr10,ptr11,ptr12,ptr13,ptr14)
          for ( int i=0; i<SIMD_VEC; i++ ){
            int idx0_4 = 4 * arg0.map_data[(n+i) * arg0.map->dim + 0];
            int idx1_4 = 4 * arg0.map_data[(n+i) * arg0.map->dim + 1];
            int idx2_4 = 4 * arg0.map_data[(n+i) * arg0.map->dim + 0];
            int idx3_4 = 4 * arg0.map_data[(n+i) * arg0.map->dim + 1];
            int idx6_2 = 2 * arg0.map_data[(n+i) * arg0.map->dim + 0];
            int idx7_2 = 2 * arg0.map_data[(n+i) * arg0.map->dim + 1];
            int idx9_8 = 8 * arg0.map_data[(n+i) * arg0.map->dim + 0];
            int idx10_8 = 8 * arg0.map_data[(n+i) * arg0.map->dim + 1];

            dat0[0][i] = (ptr0)[idx0_4 + 0];
            dat0[1][i] = (ptr0)[idx0_4 + 1];
            dat0[2][i] = (ptr0)[idx0_4 + 2];
            dat0[3][i] = (ptr0)[idx0_4 + 3];

            dat1[0][i] = (ptr1)[idx1_4 + 0];
            dat1[1][i] = (ptr1)[idx1_4 + 1];
            dat1[2][i] = (ptr1)[idx1_4 + 2];
            dat1[3][i] = (ptr1)[idx1_4 + 3];

            dat2[0][i] = (ptr2)[idx2_4 + 0];
            dat2[1][i] = (ptr2)[idx2_4 + 1];
            dat2[2][i] = (ptr2)[idx2_4 + 2];
            dat2[3][i] = (ptr2)[idx2_4 + 3];

            dat3[0][i] = (ptr3)[idx3_4 + 0];
            dat3[1][i] = (ptr3)[idx3_4 + 1];
            dat3[2][i] = (ptr3)[idx3_4 + 2];
            dat3[3][i] = (ptr3)[idx3_4 + 3];

            dat6[0][i] = (ptr6)[idx6_2 + 0];
            dat6[1][i] = (ptr6)[idx6_2 + 1];

            dat7[0][i] = (ptr7)[idx7_2 + 0];
            dat7[1][i] = (ptr7)[idx7_2 + 1];

            dat9[0][i] = (ptr9)[idx9_8 + 0];
            dat9[1][i] = (ptr9)[idx9_8 + 1];
            dat9[2][i] = (ptr9)[idx9_8 + 2];
            dat9[3][i] = (ptr9)[idx9_8 + 3];
            dat9[4][i] = (ptr9)[idx9_8 + 4];
            dat9[5][i] = (ptr9)[idx9_8 + 5];
            dat9[6][i] = (ptr9)[idx9_8 + 6];
            dat9[7][i] = (ptr9)[idx9_8 + 7];

            dat10[0][i] = (ptr10)[idx10_8 + 0];
            dat10[1][i] = (ptr10)[idx10_8 + 1];
            dat10[2][i] = (ptr10)[idx10_8 + 2];
            dat10[3][i] = (ptr10)[idx10_8 + 3];
            dat10[4][i] = (ptr10)[idx10_8 + 4];
            dat10[5][i] = (ptr10)[idx10_8 + 5];
            dat10[6][i] = (ptr10)[idx10_8 + 6];
            dat10[7][i] = (ptr10)[idx10_8 + 7];

          }
          #pragma omp simd aligned(ptr0,ptr1,ptr2,ptr3,ptr4,ptr5,ptr6,ptr7,ptr8,ptr9,ptr10,ptr11,ptr12,ptr13,ptr14)
          for ( int i=0; i<SIMD_VEC; i++ ){
            computeFluxes_vec(
              dat0,
              dat1,
              dat2,
              dat3,
              &(ptr4)[1 * (n+i)],
              &(ptr5)[2 * (n+i)],
              dat6,
              dat7,
              &(ptr8)[2 * (n+i)],
              dat9,
              dat10,
              &(ptr11)[1 * (n+i)],
              &(ptr12)[4 * (n+i)],
              &(ptr13)[3 * (n+i)],
              &(ptr14)[1 * (n+i)],
              i);
          }
          for ( int i=0; i<SIMD_VEC; i++ ){

          }
        }

        //remainder
        for ( int n=((offset_b+nelem)/SIMD_VEC)*SIMD_VEC; n<offset_b+nelem; n++ ){
        #else
        #pragma omp simd aligned(ptr0,ptr1,ptr2,ptr3,ptr4,ptr5,ptr6,ptr7,ptr8,ptr9,ptr10,ptr11,ptr12,ptr13,ptr14)
        for ( int n=offset_b; n<offset_b+nelem; n++ ){
        #endif
          int map0idx = arg0.map_data[n * arg0.map->dim + 0];
          int map1idx = arg0.map_data[n * arg0.map->dim + 1];

          computeFluxes(
            &(ptr0)[4 * map0idx],
            &(ptr1)[4 * map1idx],
            &(ptr2)[4 * map0idx],
            &(ptr3)[4 * map1idx],
            &(ptr4)[1 * n],
            &(ptr5)[2 * n],
            &(ptr6)[2 * map0idx],
            &(ptr7)[2 * map1idx],
            &(ptr8)[2 * n],
            &(ptr9)[8 * map0idx],
            &(ptr10)[8 * map1idx],
            &(ptr11)[1 * n],
            &(ptr12)[4 * n],
            &(ptr13)[3 * n],
            &(ptr14)[1 * n]);
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
  OP_kernels[7].name      = name;
  OP_kernels[7].count    += 1;
  OP_kernels[7].time     += wall_t2 - wall_t1;
  OP_kernels[7].transfer += (float)set->size * arg0.size;
  OP_kernels[7].transfer += (float)set->size * arg2.size;
  OP_kernels[7].transfer += (float)set->size * arg6.size;
  OP_kernels[7].transfer += (float)set->size * arg9.size;
  OP_kernels[7].transfer += (float)set->size * arg4.size;
  OP_kernels[7].transfer += (float)set->size * arg5.size;
  OP_kernels[7].transfer += (float)set->size * arg8.size;
  OP_kernels[7].transfer += (float)set->size * arg11.size;
  OP_kernels[7].transfer += (float)set->size * arg12.size;
  OP_kernels[7].transfer += (float)set->size * arg13.size;
  OP_kernels[7].transfer += (float)set->size * arg14.size;
  OP_kernels[7].transfer += (float)set->size * arg0.map->dim * 4.0f;
}
#undef VECTORIZE