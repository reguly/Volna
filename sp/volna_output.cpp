#include "volna_writeVTK.h"
#include "volna_common.h"
#include "getTotalVol.h"
#include "getMaxElevation.h"
#include "gatherLocations.h"
#include "op_lib_cpp.h"

//
// mpi header file - included by user for user level mpi
//

//#include <mpi.h>

int outputLocation_lastupdate = -1;

void OutputTime(TimerParams *timer) {
  op_printf("Iteration: %d, time: %lf \n", (*timer).iter, (*timer).t);
}

void OutputConservedQuantities(op_set cells, op_dat cellVolumes, op_dat values) {
  float totalVol = 0.0;
  op_par_loop(getTotalVol, "getTotalVol", cells,
      op_arg_dat(cellVolumes, -1, OP_ID, 1, "float", OP_READ),
      op_arg_dat(values, -1, OP_ID, 4, "float", OP_READ),
      op_arg_gbl(&totalVol, 1, "float", OP_INC));

  op_printf("mass(volume): %lf \n", totalVol);
}

void OutputMaxElevation(int writeOption, EventParams *event, TimerParams* timer, op_dat nodeCoords, op_map cellsToNodes, op_dat values, op_set cells) {
// Warning: The function only finds the maximum of every
// "timer.istep"-th step. Therefore intermediate maximums might be neglected.
//  op_fetch_data(values);

  // first time the event is executed
  float *temp = NULL;
  if (timer->iter == timer->istart)
    currentMaxElevation = op_decl_dat_temp(cells, 1, "float",
        temp,
        "maxElevation");
  // Get the max elevation
  op_par_loop(getMaxElevation, "getMaxElevation", cells,
      op_arg_dat(values, -1, OP_ID, 4, "float", OP_READ),
      op_arg_dat(currentMaxElevation, -1, OP_ID, 1, "float", OP_RW));

  int nnode = cellsToNodes->to->size;
  int ncell = cellsToNodes->from->size;
  char filename[255];
  strcpy(filename, event->streamName.c_str());
  const char* substituteIndexPattern = "%i";
  char* pos;
  pos = strstr(filename, substituteIndexPattern);
  char substituteIndex[255];
  // 0 - write to HDF5 file
  if(writeOption == 0) {
    sprintf(substituteIndex, "%04d.h5", timer->iter);
    strcpy(pos, substituteIndex);
    op_printf("Writing OutputMaxElevation to HDF5 file: %s \n",filename);
    op_fetch_data_hdf5_file(values, filename);
  }
  else if(writeOption > 0) {
    sprintf(substituteIndex, "%04d.vtk", timer->iter);
    strcpy(pos, substituteIndex);
    switch(writeOption) {
    // 1 - write to ASCII VTK file
    case 1:
      WriteMeshToVTKAscii(filename, nodeCoords, nnode, cellsToNodes, ncell, currentMaxElevation);
      break;
      // 1 - write to Binary VTK file
    case 2:
      WriteMeshToVTKBinary(filename, nodeCoords, nnode, cellsToNodes, ncell, currentMaxElevation);
      break;
    }
  }
}

/*
 * Write H + Zb on the given location (x,y) to ASCII file
 */
void OutputLocation(EventParams *event, int eventid, TimerParams* timer, op_set cells, op_dat nodeCoords, op_map cellsToNodes, op_dat values, op_map outputLocation_map, op_dat outputLocation_dat) {
  if (outputLocation_lastupdate == -1 || timer->iter != (unsigned int)outputLocation_lastupdate) {
    op_par_loop(gatherLocations, "gatherLocations", outputLocation_map->from,
        op_arg_dat(values, 0, outputLocation_map, 4, "float", OP_READ),
        op_arg_dat(outputLocation_dat, -1, OP_ID, 1, "float", OP_WRITE));
    // Fetch data on every node
    op_fetch_data_hdf5(outputLocation_dat, locationData.tmp, 0, locationData.n_points-1);
    outputLocation_lastupdate = timer->iter;
  }

  //op_printf("OutputLocation: time = %f  eventid = %d   H = %10.20f\n", timer->t, eventid, locationData.tmp[0]);
  // Write location data to std vectors
  if(op_is_root()) {
    locationData.time[event->loc_index].push_back(timer->t);
    locationData.value[event->loc_index].push_back(locationData.tmp[event->loc_index]);
    /*for (int j = 0; j < locationData.n_points; j++) {
      if (locationData.filename[j] == event->streamName) {
        locationData.time[j].push_back(timer->t);
        locationData.value[j].push_back(locationData.tmp[j]);
        break;
      }
    }*/
  }
}

/*
 * Write output simulation either to binary or ASCII file
 */
void OutputSimulation(int writeOption, EventParams *event, TimerParams* timer, op_dat nodeCoords, op_map cellsToNodes, op_dat values) {
  char filename[255];
  strcpy(filename, event->streamName.c_str());
  int nnode = nodeCoords->set->size;
  int ncell = cellsToNodes->from->size;
  const char* substituteIndexPattern = "%i";
  char* pos;
  pos = strstr(filename, substituteIndexPattern);
  char substituteIndex[255];

  // 0 - write to HDF5 file
  if(writeOption == 0) {
    sprintf(substituteIndex, "%04d.h5", timer->iter);
    strcpy(pos, substituteIndex);
    op_printf("Writing OutputSimulation to HDF5 file: %s \n",filename);
    op_fetch_data_hdf5_file(values, filename);
  }
  else if(writeOption > 0) {
    sprintf(substituteIndex, "%04d.vtk", timer->iter);
    strcpy(pos, substituteIndex);
    switch(writeOption) {
    // 1 - write to ASCII VTK file
    case 1:
      WriteMeshToVTKAscii(filename, nodeCoords, nnode, cellsToNodes, ncell, values);
      break;
      // 1 - write to Binary VTK file
    case 2:
      WriteMeshToVTKBinary(filename, nodeCoords, nnode, cellsToNodes, ncell, values);
      break;
    }
  }
}

float normcomp(op_dat dat, int off) {
  int dim = dat->dim;
  float *data = (float *)(dat->data);
  float norm = 0.0;
  for (int i = 0; i < dat->set->size; i++) {
    norm += data[dim*i + off]*data[dim*i + off];
  }
  return sqrt(norm);
}

void dumpme(op_dat dat, int off) {
  int dim = dat->dim;
  float *data = (float *)(dat->data);
  for (int i = 0; i < dat->set->size; i++) {
    printf("%g\n",data[dim*i + off]);
  }
}
