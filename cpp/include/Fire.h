#pragma once
#include "Grid.h"
#include <vector>

class Fire
{
public:
   //===========================================//
   //               CONSTRUCTORS                //
   //===========================================//
   
   // Construct a Fire with ignition points
   Fire(Grid<char> landtype,Grid<double> accum_energy,std::vector<long>&& ignitions,int max_radius);
   
   
   //===========================================//
   //               PROPERTIES                  //
   //===========================================//
   
   // The landscape over which this fire is burning
   Grid<char> landtype;
   Grid<double> accum_energy;
   
   // Points that are burning at the beginning of the simulation
   std::vector<long> active_burns;
   
   // Total number of pixels that have bu;rned.
   long n_fires;
   
   // Maximum radius that energy transfer is truncated to.
   const int max_radius;
   
   
   //===========================================//
   //               METHODS                     //
   //===========================================//
   
   // Apply energy from currently burning pixels and build a new list.
   bool iterate();
   
   // Burn a single pixel and release energy to neighboring pixels.
   std::vector<long> burn_pixel(long pixel,double energy_released);
   
   // Number of pixels that are currently on fire
   inline long n_active_fires() const {return active_burns.size();}
};

