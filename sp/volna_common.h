//#ifndef __VOLNA_COMMON_H
//#define __VOLNA_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <hdf5.h>
#include <hdf5_hl.h>
#include "op_lib_cpp.h"

#ifdef SLOPE
#include "inspector.h"
#include "executor.h"
#endif

//
// Define meta data
//
#define N_STATEVAR 4
#define MESH_DIM 2
#define N_NODESPERCELL 3
#define N_CELLSPEREDGE 2

struct LocationData{
  std::vector<std::string> filename;
  int n_points; // Number of point of interest
  float *tmp;
  std::vector<std::vector<float> > time;
  std::vector<std::vector<float> > value;
  std::vector<std::vector<float> > allvalues;
};

extern LocationData locationData;

struct RectangleDomainParams {
  int nx, ny;
  float xmin, xmax, ymin, ymax;
};

struct BoreParams {
  float x0, Hl, ul, vl;
  float S;
};

//these are not const, we just don't want to pass them around
extern double timestamp;
extern int itercount;

//constants
extern float EPS, CFL, g;
extern op_dat currentMaxElevation;
extern op_dat currentMaxSpeed;
extern bool new_format;

struct GaussianLandslideParams {
  float A, v, lx, ly, mesh_xmin;//TODO: mesh_xmin compute
};

struct TimerParams {
  double start, end, step, localTime, t;
  unsigned int istart, iend, istep, localIter, iter;
};

struct EventParams {
  float location_x, location_y;
  int post_update;
  std::string className;
  std::string formula;
  std::string streamName;
  int loc_index;
};

int timer_happens(TimerParams *p);
void read_events_hdf5(hid_t h5file, int num_events, std::vector<TimerParams> *timers, std::vector<EventParams> *events, int *num_outputLocation);
void write_locations_hdf5(float *data, int dim_cont, int dim_stride, const char *filename);
void processEvents(std::vector<TimerParams> *timers, std::vector<EventParams> *events, int firstTime, int updateTimers,
 									 float timeIncrement, int removeFinished, int initPrePost, op_set cells, op_dat values, op_dat cellVolumes,
									 op_dat cellCenters, op_dat nodeCoords, op_map cellsToNodes, op_dat temp_initEta, op_set bathy_nodes, op_set lifted_cells, op_map liftedcellsToBathyNodes, op_map liftedcellsToCells, op_dat bathy_xy, op_dat initial_zb,
                   op_dat* temp_initBathymetry, int n_initBathymetry, BoreParams bore_params, GaussianLandslideParams gaussian_landslide_params, op_map outputLocation_map,
									 op_dat outputLocation_dat, int writeOption);
void processLastSimulation(std::vector<TimerParams> *timers, std::vector<EventParams> *events, op_set cells, op_dat values, op_dat cellVolumes,
									 op_dat nodeCoords, op_map cellsToNodes, int writeOption);

void InitEta(op_set cells, op_dat cellCenters, op_dat values, op_dat initValues, int fromFile);
void InitU(op_set cells, op_dat cellCenters, op_dat values);
void InitV(op_set cells, op_dat cellCenters, op_dat values);
void InitBathymetry(op_set cells, op_dat cellCenters, op_dat values, op_dat initValues, int fromFile, int firstTime, op_set bathy_nodes, op_set lifted_cells, op_map liftedcellsToBathyNodes, op_map liftedcellsToCells, op_dat bathy_xy, op_dat initial_zb);
void InitBore(op_set cells, op_dat cellCenters, op_dat values, BoreParams params);
void InitGaussianLandslide(op_set cells, op_dat cellCenters, op_dat values, GaussianLandslideParams params, int firstTime);

void OutputTime(TimerParams *timer);
void OutputConservedQuantities(op_set cells, op_dat cellVolumes, op_dat values);
void OutputLocation(EventParams *event, int eventid, TimerParams* timer, op_set cells, op_dat nodeCoords, op_map cellsToNodes, op_dat values, op_map outputLocation_map, op_dat outputLocation_dat);
void OutputSimulation(int writeOption, EventParams *event, TimerParams* timer, op_dat nodeCoords, op_map cellsToNodes, op_dat values);
void OutputMaxElevation(int writeOption, EventParams *event, TimerParams* timer, op_dat nodeCoords, op_map cellsToNodes, op_dat values, op_set cells);
void OutputMaxSpeed(int writeOption, EventParams *event, TimerParams* timer, op_dat nodeCoords, op_map cellsToNodes, op_dat values, op_set cells);
float normcomp(op_dat dat, int off);
void dumpme(op_dat dat, int off);

#ifdef SLOPE
void spaceDiscretization(op_dat data_in, op_dat data_out, float *minTimestep,
    op_dat bathySource, op_dat edgeFluxes, op_dat maxEdgeEigenvalues,
    op_dat edgeNormals, op_dat edgeLength, op_dat cellVolumes, op_dat isBoundary,
    op_set cells, op_set edges, op_map edgesToCells, op_map cellsToEdges, 
    op_map cellsToCells, op_dat edgeCenters, op_dat cellCenters, op_dat GradientatCell, op_dat q, op_dat lim, int most,
    inspector_t* insp, executor_t* exec, int nColors);
#else
void spaceDiscretization(op_dat data_in, op_dat data_out, float *minTimestep,
    op_dat bathySource, op_dat edgeFluxes, op_dat maxEdgeEigenvalues,
    op_dat edgeNormals, op_dat edgeLength, op_dat cellVolumes, op_dat isBoundary,
    op_set cells, op_set edges, op_map edgesToCells, op_map cellsToEdges, 
    op_map cellsToCells, op_dat edgeCenters, op_dat cellCenters, op_dat GradientatCell, op_dat q, op_dat lim, int most);
#endif

//
//helper functions
//
#define check_hdf5_error(err) __check_hdf5_error(err, __FILE__, __LINE__)
void __check_hdf5_error(herr_t err, const char *file, const int line);

//#endif
