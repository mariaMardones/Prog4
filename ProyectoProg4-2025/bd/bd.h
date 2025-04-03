#ifndef DB_DB_H_
#define DB_DB_H_

#include "sqlite3.h"

int inicializar_bd(sqlite3 **db);

void cerrar_bd(sqlite3 *db);

int registrar_usuario(sqlite3 *db, const char *nombre, const char *dni, const char *contrasena, int rol);

int iniciar_sesion(sqlite3 *db, const char *dni, const char *contrasena, int *rol);

int hacer_reserva(sqlite3 *db, int usuario_id, int habitacion, const char *fecha);

int cancelar_reserva(sqlite3 *db, int reserva_id);

#endif /* DB_DB_H_ */
