//
// Created by diego on 22/10/21.
//

#ifndef PROYECTO_SOA_CALCULATIONS_HPP
#define PROYECTO_SOA_CALCULATIONS_HPP
#include <fstream>

vec* calcForces(int num_objetos, object* objetos, vec* forces);

vec* calcAccelerations(int num_objetos,object* objetos, vec* forces, vec* accelerations);

void calcVelocities (int num_objetos, object* objetos, vec* accelerations, double incr_tiempo);

void calcPositions (int num_objetos, object* objetos, double incr_tiempo, double lado);


#endif //PROYECTO_SOA_CALCULATIONS_HPP