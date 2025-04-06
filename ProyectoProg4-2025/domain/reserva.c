#include "reserva.h"
#include <stdio.h>
#include <string.h>

void inicializarReserva(Reserva *r, int codigo, int num_habitacion, const char *dni_usuario) {
    r->codigo = codigo;
    r->num_habitacion = num_habitacion;
    strncpy(r->dni_usuario, dni_usuario, 9);
    r->dni_usuario[9] = '\0';
}

void mostrarReserva(const Reserva *r) {
    printf("Reserva %d - HabitaciÃ³n: %d - DNI: %s\n", r->codigo, r->num_habitacion, r->dni_usuario);
}
