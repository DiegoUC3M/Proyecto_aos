//
// Created by diego on 22/10/21.
//
#include <iostream>
#include <cmath>
#include <iomanip>
#include <omp.h>
#include "sim-aos.hpp"
#include "calculations.hpp"


//DECLARAMOS LAS VARIABLES FILE
ofstream forcesFile;
ofstream accelarationFile;
ofstream velocitiesFile;
ofstream positionsFile;







//HACEMOS EL CALCULO DE FUERZAS
vec* calcForces(int num_objetos, object* objetos, vec* forces, int iteration) {

    forcesFile << "iteración " << iteration << ":" << endl; //Escribimos en el txt en que iteracion esta antes de entrar en el bucle

    //EN ESTAS VARIABLES SE ALMACENAN LOS CALCULOS DE LA FUERZA GRAVITATORIA ENTRE DOS OBJETOS. LOS INICIALIZAMOS
    double fx = 0;
    double fy = 0;
    double fz = 0;

    //HACE EL CALCULO DE UN SOLO OBJETO CON EL DEL RESTO DE OBJETOS. POR ELLO SE QUEDA EN UN OBJETO CON EL PRIMER BUCLE (POSICION J) E ITERA
    //SOBRE EL RESTO DE OBJETOS (POSICION K)
    for (int j = 0; j < num_objetos; j++) {

        for (int k = j + 1; k < num_objetos; k++) {

            //FUNCIÓN FUERZA
            double norma;
            norma = std::sqrt(((objetos[k].position_x - objetos[j].position_x) *
                               (objetos[k].position_x - objetos[j].position_x)) +
                              ((objetos[k].position_y - objetos[j].position_y) *
                               (objetos[k].position_y - objetos[j].position_y)) +
                              ((objetos[k].position_z - objetos[j].position_z) *
                               (objetos[k].position_z - objetos[j].position_z)));


            fx = (((G * objetos[k].masa) * objetos[j].masa) * (objetos[k].position_x - objetos[j].position_x)) /
                 (norma * norma * norma);//fuerza eje x
            fy = (((G * objetos[k].masa) * objetos[j].masa) * (objetos[k].position_y - objetos[j].position_y)) /
                 (norma * norma * norma);//fuerza eje y
            fz = (((G * objetos[k].masa) * objetos[j].masa) * (objetos[k].position_z - objetos[j].position_z)) /
                 (norma * norma * norma);//fuerza eje z

            //COMO SOBRE UN OBJETO INFLUYE LA FUERZA DEL RESTO DE OBJETOS, VAMOS SUMANDO CONTINUAMENTE LAS FUERZAS QUE SE EJERCEN SOBRE EL OBJETO QUE
            //ESTA SIENDO ITERADO (J), POR ESO SE SUMA EL ANTIGUO VALOR DE LA VARIABLE.
            forces[j].x = forces[j].x + fx;
            forces[j].y = forces[j].y + fy;
            forces[j].z = forces[j].z + fz;
            forces[k].x = forces[k].x - fx;
            forces[k].y = forces[k].y - fy;
            forces[k].z = forces[k].z - fz;

        }


        forcesFile << "fuerza[" << j << "]: " << forces[j].x << " "  << forces[j].y << " "  << forces[j].z <<endl;

    }

    return forces;
}






//HACEMOS EL CALCULO DE LAS ACELERACIONES
vec* calcAccelerations(int num_objetos,object* objetos, vec* forces, vec* accelerations, int iteration) {

    accelarationFile << "iteración " << iteration << ":" << endl;

    //CALCULAMOS TODAS LAS ACELERACION ITERANDO SOBRE TODOS LOS OBJETOS
#pragma omp parallel for
    for (int j = 0; j < num_objetos; j++) {

        accelerations[j].x = forces[j].x / objetos[j].masa;//aceleración eje x
        accelerations[j].y = forces[j].y / objetos[j].masa;//aceleración eje y
        accelerations[j].z = forces[j].z / objetos[j].masa;//aceleración eje z

#pragma omp critical
        accelarationFile << "accelaration[" << j << "]: " << accelerations[j].x << " "  << accelerations[j].y << " "  << accelerations[j].z <<endl;


    }

    return accelerations;
}






//HACEMOS EL CALCULO DE LAS VELOCIDADES
void calcVelocities (int num_objetos, object* objetos, vec* accelerations, int iteration, double incr_tiempo) {

    velocitiesFile << "iteración " << iteration << ":" << endl;

    //CALCULAMOS TODAS LAS VELOCIDADES ITERANDO SOBRE TODOS LOS OBJETOS
#pragma omp parallel for
    for (int j = 0; j < num_objetos; j++) {

        objetos[j].speed_x = objetos[j].speed_x + (accelerations[j].x * (incr_tiempo));
        objetos[j].speed_y = objetos[j].speed_y + (accelerations[j].y * (incr_tiempo));
        objetos[j].speed_z = objetos[j].speed_z + (accelerations[j].z * (incr_tiempo));

#pragma omp critical
        velocitiesFile << "speed[" << j << "]: " << objetos[j].speed_x << " "  << objetos[j].speed_y  << " "  << objetos[j].speed_z <<endl;


    }

}




//HACEMOS EL CALCULO DE LAS POSICIONES
void calcPositions (int num_objetos, object* objetos, int iteration, double incr_tiempo, double lado) {

    positionsFile << "iteración " << iteration << ":" << endl;

#pragma omp parallel for
    for (int j=0;j<num_objetos;j++) {
        objetos[j].position_x = objetos[j].position_x + (objetos[j].speed_x * (incr_tiempo));
        objetos[j].position_y = objetos[j].position_y + (objetos[j].speed_y * (incr_tiempo));
        objetos[j].position_z = objetos[j].position_z + (objetos[j].speed_z * (incr_tiempo));



        //SI SE SALE UN OBJETO POR EL EJE X
        if (objetos[j].position_x <= 0) {
            objetos[j].speed_x = -objetos[j].speed_x; //CAMBIAMOS LA DIRECCION DEL VECTOR VELOCIDAD
            objetos[j].position_x = 0;
        } else if (objetos[j].position_x >= lado) {
            objetos[j].speed_x = -objetos[j].speed_x;
            objetos[j].position_x = lado;
        }


        //SI SE SALE UN OBJETO POR EL EJE Y
        if (objetos[j].position_y <= 0) {
            objetos[j].speed_y = -objetos[j].speed_y;
            objetos[j].position_y = 0;
        } else if (objetos[j].position_y >= lado) {
            objetos[j].speed_y = -objetos[j].speed_y;
            objetos[j].position_y = lado;
        }


        //SI SE SALE UN OBJETO POR EL EJE Z
        if (objetos[j].position_z <= 0) {
            objetos[j].speed_z = -objetos[j].speed_z;
            objetos[j].position_z = 0;
        } else if (objetos[j].position_z >= lado) {
            objetos[j].speed_z = -objetos[j].speed_z;
            objetos[j].position_z = lado;
        }

#pragma omp critical
        positionsFile << "positions[" << j << "]: " << objetos[j].position_x << " "  << objetos[j].position_y  << " "  << objetos[j].position_z <<endl;


    }

}
