#include "habitacion.h"
#include <stdio.h>

void inicializarHabitacion(Habitacion *h, int numero) {
    h->numero = numero;
    h->reservado = 0;
}

void mostrarHabitacion(const Habitacion *h) {
    printf("HabitaciÃ³n %d - Reservado: %s\n", h->numero, h->reservado ? "SÃ­" : "No");
}
