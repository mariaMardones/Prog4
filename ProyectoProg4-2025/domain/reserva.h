#ifndef RESERVA_H
#define RESERVA_H
#include "../bd/sqlite3.h"

typedef struct {
    int id;
    int num_habitacion;
    char dni_usuario[10];
    char fecha[20];
} Reserva;

extern void hacer_reserva(sqlite3 *db, const char *dni, int habitacion, const char *fecha);
extern void cancelar_reserva(sqlite3 *db, const char *dni, int habitacion);
void ver_reservas_activas(sqlite3 *db, const char *dni);

#endif
