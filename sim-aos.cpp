#include <iostream>
#include <random>
#include <fstream>
#include <iomanip> /*Libreria para ajustar la precision*/
#include "sim-aos.hpp"
#include "calculations.hpp"

using namespace std;





// HACEMOS PASO POR REFERENCIA EN EL MÉTODO COMPROBACIONCOLISIONES PARA QUE CAMBIE EL VALOR DE NUM_OBJETOS TAMBIÉN EN EL MAIN
void comprobacionColisiones (int& num_objetos, object* objetos) {

    int num_eliminados; //Para ir viendo el numero de eliminados
    double norma;

    //COMPROBAMOS EL OBJETO ACTUAL CON EL RESTO DE OBJETOS. PARA ELLO SE QUEDA EN UNA POSICION J Y COMPRUEBA CON EL RESTO DE OBJETOS EN EL SIGUIENTE
    //FOR ITERANDO EN LA POSICION K
    for (int j = 0 ; j < num_objetos; j++){

        num_eliminados=0;
        for (int k = j+1; k < num_objetos+1; k++) {

            //ESTO SE HACE PARA QUE EL "COMPROBADOR DE COLISIONES" PRUEBE LA POSICIÓN NUEVA A LA QUE SE HAN MOVIDO LOS ELEMENTOS QUE NO
            // ERAN "NULOS" (masa == -1)
            if(j!=k-1) {
                norma = std::sqrt(((objetos[k-1].position_x - objetos[j].position_x) * (objetos[k-1].position_x - objetos[j].position_x)) + ((objetos[k-1].position_y - objetos[j].position_y) * (objetos[k-1].position_y - objetos[j].position_y)) + ((objetos[k-1].position_z - objetos[j].position_z) * (objetos[k-1].position_z - objetos[j].position_z)));

                if (norma<1) {
                    k=k-1;
                }
            }
            if (objetos[k].masa==0) {
                break;
            }


            norma = std::sqrt(((objetos[k].position_x - objetos[j].position_x) * (objetos[k].position_x - objetos[j].position_x)) + ((objetos[k].position_y - objetos[j].position_y) * (objetos[k].position_y - objetos[j].position_y)) + ((objetos[k].position_z - objetos[j].position_z) * (objetos[k].position_z - objetos[j].position_z)));

            if (norma < 1) {//si su distancia es menor que la unidad

                //ESCRIBIMOS LAS COLISIONES EN EL TXT DE POSITIONS
                positionsFile << "Collapsing objects " << j << " and " << k+num_eliminados<< endl;
                positionsFile << "Body " << j << ": " << objetos[j].position_x << " " << objetos[j].position_y << " " << objetos[j].position_z << " " << objetos[j].speed_x << " " << objetos[j].speed_y << " " << objetos[j].speed_z << " " << objetos[j].masa << endl;
                positionsFile << "Body " << k+num_eliminados << ": " << objetos[k].position_x << " " << objetos[k].position_y << " " << objetos[k].position_z << " " << objetos[k].speed_x << " " << objetos[k].speed_y << " " << objetos[k].speed_z << " " << objetos[k].masa << endl;

                //SUMAMOS LAS MASAS Y VELOCIDADES DE LOS OBJETOS J Y K Y ELIMINAMOS EL SEGUNDO OBJETO (ASIGNANDO VALORES -1)
                objetos[j].masa = objetos[j].masa + objetos[k].masa;
                objetos[j].speed_x = objetos[j].speed_x + objetos[k].speed_x;
                objetos[j].speed_y = objetos[j].speed_y + objetos[k].speed_y;
                objetos[j].speed_z = objetos[j].speed_z + objetos[k].speed_z;
                objetos[k].masa = -1;
                objetos[k].speed_x = -1;
                objetos[k].speed_y = -1;

                objetos[k].speed_z = -1;
                objetos[k].position_x = -1;
                objetos[k].position_y = -1;
                objetos[k].position_z = -1;

                positionsFile << "Body " << k+num_eliminados << " removed" <<endl;


                //ELIMINAMOS LOS OBJETOS MOVIÉNDOLOS AL FINAL DEL ARRAY, PARA QUE AL PRINCIPIO SOLO ESTÉN LOS NO COLISIONADOS
                //PARA ELLO, COMPROBAMOS EL OBJETO ELIMINADO CON EL SIGUIENTE. SI EL SIGUIENTE TIENE MASA DISTINTA A -1 INTERCAMBIAN SUS POSICIONES.
                //ASI HASTA QUE LLEGA A LA ULTIMA POSICION, QUE SERÁ CUANDO EL SIGUIENTE OBJETO TAMBIÉN TENGA MASA -1 O HAYA LLEGADO A NUM_OBJETOS

                for (int a = k; a < num_objetos; a++) {
                    if(objetos[a].masa == -1) {
                        if (objetos[a + 1].masa != -1) {

                            objetos[a].masa = objetos[a + 1].masa;
                            objetos[a].speed_x = objetos[a + 1].speed_x;
                            objetos[a].speed_y = objetos[a + 1].speed_y;
                            objetos[a].speed_z = objetos[a + 1].speed_z;
                            objetos[a].position_x = objetos[a + 1].position_x;
                            objetos[a].position_y = objetos[a + 1].position_y;
                            objetos[a].position_z = objetos[a + 1].position_z;
                            objetos[a + 1].masa = -1;
                            objetos[a + 1].speed_x = -1;
                            objetos[a + 1].speed_y = -1;
                            objetos[a + 1].speed_z = -1;
                            objetos[a + 1].position_x = -1;
                            objetos[a + 1].position_y = -1;
                            objetos[a + 1].position_z = -1;

                        }
                    }
                }

                num_eliminados++;
                num_objetos = num_objetos - 1;


            }



            positionsFile << "Body " << j << " after collapse" <<endl;
            positionsFile << "Body " << j << ": " << objetos[j].position_x << " " << objetos[j].position_y << " " << objetos[j].position_z << " " << objetos[j].speed_x << " " << objetos[j].speed_y << " " << objetos[j].speed_z << " " << objetos[j].masa << endl << endl;





        }

    }

}





int main(int argc, char *argv[]) {


    mt19937_64 gen64_aos;


    //COMPROBAMOS EL NUMERO DE PARÁMETROS
    if (argc != 6) {
        cerr << "El número de parámetros no es correcto" << "\nArgumentos:\n Num_objects:" << argv[1] << "\nNum_iterations:" << argv[2] << "\nRandom_seed:" << argv[3] << "\nSize_enclosure:" << argv[4] << "\nTime_step:" << argv[5];
        return -1;
    }

    int num_objetos = stoi(argv[1]);
    int num_iteraciones = stoi(argv[2]);
    int semilla = stoi(argv[3]);
    double lado = stod(argv[4]);
    double incr_tiempo = stod(argv[5]);


    //COMPROBAMOS QUE EL NUMERO DE PARÁMETROS ES CORRECTO
    if (num_objetos <= 0 || num_iteraciones <= 0 || semilla <= 0 || lado <= 0 || incr_tiempo <= 0) {
        cerr << "El valor de alguno de los parámetros no es correcto\n" << "Argumentos:\nNum_objects:" << num_objetos << "\nNum_iterations:" << num_iteraciones << "\nRandom_seed:" << semilla << "\nSize_enclosure:" << lado << "\nTime_step:" << incr_tiempo;
        return -2;
    }

    cout << "Creating simulation:\n" << "  num_objects: " << num_objetos << "\n  num_iterations: " << num_iteraciones << "\n  random_seed: " << semilla << "\n  size_enclosure: " << lado << "\n  time_step: " << incr_tiempo << "\n";






    //CREAMOS OBJETO E INICIALIZAMOS CON MALLOC LOS ARRAYS CON SIZE = NUM_OBJETOS
    object* objetos;
    objetos= (object*) malloc (num_objetos* sizeof(object));

    vec* fuerza;
    fuerza = (vec*) malloc(num_objetos* sizeof(vec));

    vec* aceleracion;
    aceleracion = (vec*) malloc(num_objetos* sizeof(vec));




    //INICIALIZACIÓN PARA LOS DATOS ALEATORIOS
    gen64_aos.seed(semilla);
    uniform_real_distribution<> distUniforme(0.0, lado);
    normal_distribution <>distNormal{10E20, 10E14};

    //INICIALIZO EL ARCHIVO: FILE_INIT
    ofstream file_init;
    file_init.open("init_config.txt");
    file_init << fixed << setprecision(3) << lado << " " << incr_tiempo << " " << num_objetos;

    //INICIALIZAMOS TODOS LOS OBJETOS
    for(int i=0; i<num_objetos; i++){
        objetos[i].position_x=distUniforme(gen64_aos);
        objetos[i].position_y=distUniforme(gen64_aos);
        objetos[i].position_z=distUniforme(gen64_aos);
        objetos[i].speed_x=0;
        objetos[i].speed_y=0;
        objetos[i].speed_z=0;
        objetos[i].masa=distNormal(gen64_aos);
        file_init << fixed << setprecision(3) << "\n" << objetos[i].position_x << " " << objetos[i].position_y << " " << objetos[i].position_z << " " << objetos[i].speed_x << " " << objetos[i].speed_y << " " << objetos[i].speed_z << " " << objetos[i].masa;
    }
    file_init.close();//cierro el archivo




    //COMPROBACIÓN DE COLISIONES INICIAL
    comprobacionColisiones(num_objetos,objetos);



    //ABRIMOS TODOS LOS ARCHIVOS TXT PARA PROCEDER A MODIFICARLOS
    forcesFile.open("forces.txt");
    accelarationFile.open("accelaration.txt");
    velocitiesFile.open("velocities.txt");
    positionsFile.open("positions.txt");







    //********CALCULAMOS LAS FUERZAS, ACELERACIONES, VELOCIDADES Y POSICIONES DE TODOS LOS CUERPOS***********

    for (int i = 0; i < num_iteraciones; i++) {
        //ES NECESARIO INICIALIZAR LAS FUERZAS A CERO EN CADA NUEVA ITERACION, PARA QUE NO SE SUMEN LAS FUERZAS DE LAS ITERACIONES ANTERIORES EN NUEVAS ITERACIONES
        for (int j=0; j < num_objetos; j++){
            fuerza[j].x = 0;
            fuerza[j].y = 0;
            fuerza[j].z = 0;
        }

        //REALIZAMOS LOS CÁLCULOS Y LA COMPROBACIÓN DE COLISIONES LLAMANDO A LAS FUNCIONES
        fuerza = calcForces(num_objetos,objetos, fuerza, i);
        aceleracion = calcAccelerations(num_objetos,objetos,fuerza,aceleracion, i);
        calcVelocities(num_objetos,objetos,aceleracion,i, incr_tiempo);
        calcPositions(num_objetos,objetos,i,incr_tiempo,lado);

        comprobacionColisiones(num_objetos,objetos);


    }





    //CERRAMOS TODOS LOS ARCHIVOS TXT
    forcesFile.close();
    accelarationFile.close();
    velocitiesFile.close();
    positionsFile.close();


    //ESCRIBIMOS EL ARCHIVO FILE_FINAL, QUE TIENE LAS POSICIONES, VELOCIDADES Y MASAS DE LOS OBJETOS QUE QUEDAN AL FINAL AL NO HABER COLISIONADO

    ofstream file_final;
    file_final.open("final_config.txt");
    file_final << fixed << setprecision(3) << lado << " " << incr_tiempo << " " << num_objetos;
    for(int j=0; j<num_objetos; j++){
        file_final << fixed << setprecision(3) << "\n" << objetos[j].position_x << " " << objetos[j].position_y << " " << objetos[j].position_z << " " << objetos[j].speed_x << " " << objetos[j].speed_y << " " << objetos[j].speed_z << " " << objetos[j].masa;
    }
    file_final.close();//cierro el archivo





    return 0;
}

