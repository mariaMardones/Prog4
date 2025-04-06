#ifndef USUARIO_H
#define USUARIO_H

typedef struct {
    char nombre[50];
    char dni[10];
    char contrasena[50];
    int rol; // 0 = Cliente, 1 = Admin
} Usuario;

int iniciar_sesion(sqlite3 *db, const char *dni, const char *contrasena, int *rol);
int registrar_usuario(sqlite3 *db, const char *nombre, const char *dni, const char *contrasena, int rol);

#endif
