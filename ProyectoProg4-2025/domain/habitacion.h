#ifndef HABITACION_H
#define HABITACION_H

#include "../bd/sqlite3.h"
typedef struct {
    int numero;
    float precio;
    int reservado; // 0 = no, 1 = sÃ­
} Habitacion;

void mostrarHabitacion(const Habitacion *h);
void mostrar_habitaciones_disponibles(sqlite3 *db);
extern void filtrar_habitaciones_por_precio(sqlite3 *db, float max_precio);


#endif
