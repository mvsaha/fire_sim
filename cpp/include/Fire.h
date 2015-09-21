#pragma once
#include "Grid.h"
#include <vector>

/// Object representing a propagating fire front
// The fire class contains references to Grids that it does not own,
// it is only meant to be a component of the Simulation class, which
// ensures that it does not contain invalid references.
class Fire
{
public:
   //===========================================//
   //               CONSTRUCTORS                //
   //===========================================//
   
   // Construct a Fire with ignition points
   Fire(
      Grid<char>& landtype,
      Grid<double>& activation_energy,
      Grid<double>& released_energy,
      Grid<double>& accumulated_energy,
      std::vector<long>&& ignitions,
      int max_radius);
   
   
   //===========================================//
   //               PROPERTIES                  //
   //===========================================//
   
   // Spatial grids over which the fire is burning
   Grid<char>& landtype; // Type of land cover (0,1,2)
   Grid<double>& AE; // Energy requirement for burning
   Grid<double>& RE; // Energy released upon burning
   Grid<double>& E;  // Current energy tallies
   
   // Points that are currently burning
   std::vector<long> active_burns;
   
   // Total number of pixels that have burned.
   long n_fires;
   
   // Maximum radius that energy transfer is truncated to.
   const int max_radius;
   
   
   //===========================================//
   //               METHODS                     //
   //===========================================//
   
   // Apply energy from currently burning pixels and build a new list.
   bool step();
   
   // Burn a single pixel and release energy to neighboring pixels.
   inline std::vector<long> burn_pixel(long pixel);
   
   // Number of pixels that are currently on fire
   inline long n_active_fires() const {return active_burns.size();}
};

