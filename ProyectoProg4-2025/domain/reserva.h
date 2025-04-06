#ifndef RESERVA_H
#define RESERVA_H
#include "habitacion.h"

typedef struct {
    int codigo;
    int num_habitacion;
    char dni_usuario[10];
} Reserva;

void inicializarReserva(Reserva *r, int codigo, int num_habitacion, const char *dni_usuario);
void mostrarReserva(const Reserva *r);
#endif
