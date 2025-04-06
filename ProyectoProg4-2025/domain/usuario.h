#ifndef USUARIO_H
#define USUARIO_H

#include "../bd/sqlite3.h"

#define ROL_CLIENTE 0
#define ROL_ADMIN 1

// Estructura para mantener los datos del usuario conectado
typedef struct {
    char dni[20];
    int rol;
} Usuario;

int registrar_usuario(sqlite3 *db);
int iniciar_sesion(sqlite3 *db, Usuario *usuario);
void realizar_reserva_cliente(sqlite3 *db, const char *dni);
void cancelar_reserva_cliente(sqlite3 *db, const char *dni);
void ver_habitaciones_disponibles_cliente(sqlite3 *db);
void filtrar_habitaciones_cliente(sqlite3 *db);
void consultar_reservas_cliente(sqlite3 *db, const char *dni);
void visualizar_clientes_admin(sqlite3 *db);
void buscar_cliente_por_dni_admin(sqlite3 *db);
void cancelar_reserva_admin(sqlite3 *db);
void eliminar_cliente_admin(sqlite3 *db);
void cerrar_sesion_admin();


#endif
