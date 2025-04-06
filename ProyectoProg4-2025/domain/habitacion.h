#ifndef HABITACION_H
#define HABITACION_H

typedef struct {
    int numero;
    int reservado; // 0 = no, 1 = sÃ­
} Habitacion;

void inicializarHabitacion(Habitacion *h, int numero);
void mostrarHabitacion(const Habitacion *h);
#endif
