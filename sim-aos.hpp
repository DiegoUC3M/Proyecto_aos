#ifndef PROYECTO_SIM_AOS_HPP
#define PROYECTO_SIM_AOS_HPP

using namespace std;

const double G = 6.674*10E-12;

struct object {//parametros del objeto
    double position_x;
    double position_y;
    double position_z;
    double speed_x;
    double speed_y;
    double speed_z;
    double masa;
};

struct vec {//parametros del vector
    double x;
    double y;
    double z;
};

//FUNCIONES UTILIZADAS EN SIM-SOA.CPP:*********************/
void comprobacionColisiones (int& num_objetos, object* objetos);

#endif //PROYECTO_SIM_AOS_HPP
