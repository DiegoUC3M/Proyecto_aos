//
// Created by diego on 22/10/21.
//

#ifndef PROYECTO_SOA_CALCULATIONS_HPP
#define PROYECTO_SOA_CALCULATIONS_HPP
#include <fstream>

extern ofstream forcesFile;
extern ofstream accelarationFile;
extern ofstream velocitiesFile;
extern ofstream positionsFile;

vec* calcForces(double num_objetos, object* objetos, vec* forces, int iteration);

vec* calcAccelerations(double num_objetos,object* objetos, vec* forces, vec* accelerations, int iteration);

void calcVelocities (double num_objetos, object* objetos, vec* accelerations, int iteration, double incr_tiempo);

void calcPositions (double num_objetos, object* objetos, int iteration, double incr_tiempo, double lado);


#endif //PROYECTO_SOA_CALCULATIONS_HPP