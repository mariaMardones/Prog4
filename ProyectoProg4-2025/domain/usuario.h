#ifndef USUARIO_H
#define USUARIO_H

#include <sqlite3.h>

#define ROL_CLIENTE 0
#define ROL_ADMIN 1

// Estructura para mantener los datos del usuario conectado
typedef struct {
    char dni[20];
    int rol;
} Usuario;

int registrar_usuario(sqlite3 *db);
int iniciar_sesion(sqlite3 *db, Usuario *usuario);

#endif
