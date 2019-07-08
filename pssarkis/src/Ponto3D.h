#ifndef __PONTO3D_H__
#define __PONTO3D_H__

struct Ponto3D{
    double x;
    double y;
    double z;
    Ponto3D(){
        x = 0;
        y = 0;
        z = 0;
    }
    Ponto3D(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
};



#endif
