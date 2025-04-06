#ifndef BD_H
#define BD_H

#include <sqlite3.h>

int conectar_bd(sqlite3 **db);
int crear_tablas(sqlite3 *db);
int insertar_usuario(sqlite3 *db, const char *nombre, const char *dni, const char *contrasena, int rol);
int verificar_login(sqlite3 *db, const char *dni, const char *contrasena, int *rol);
int insertar_habitacion(sqlite3 *db, int numero, float precio);
int listar_habitaciones(sqlite3 *db);
int filtrar_habitaciones_por_precio(sqlite3 *db, float max_precio);
int hacer_reserva(sqlite3 *db, int num_habitacion, const char *dni_usuario);
int cancelar_reserva(sqlite3 *db, int codigo_reserva);
int listar_reservas_usuario(sqlite3 *db, const char *dni_usuario);

int consultar_usuarios(sqlite3 *db);
int cerrar_bd(sqlite3 *db);

#endif // BD_H
