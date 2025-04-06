#include <stdio.h>
#include <string.h>
#include "usuario.h"
#include "bd.h"

int registrar_usuario(sqlite3 *db) {
    char nombre[100], dni[20], contrasena[50];
    int rol;

    printf("\n--- Registro de Usuario ---\n");
    printf("Nombre: ");
    scanf(" %[^\n]", nombre);

    printf("DNI: ");
    scanf(" %s", dni);

    printf("Contraseña: ");
    scanf(" %s", contrasena);

    printf("Rol (0 = Cliente, 1 = Administrador): ");
    scanf("%d", &rol);

    if (insertar_usuario(db, nombre, dni, contrasena, rol)) {
        printf("Registro exitoso.\n");
        return 1;
    } else {
        printf("Error en el registro.\n");
        return 0;
    }
}

int iniciar_sesion(sqlite3 *db, Usuario *usuario) {
    char dni[20], contrasena[50];
    int rol;

    printf("\n--- Iniciar Sesión ---\n");
    printf("DNI: ");
    scanf(" %s", dni);

    printf("Contraseña: ");
    scanf(" %s", contrasena);

    if (verificar_login(db, dni, contrasena, &rol)) {
        strcpy(usuario->dni, dni);
        usuario->rol = rol;
        printf("Inicio de sesión exitoso. Rol: %s\n", rol == ROL_ADMIN ? "Administrador" : "Cliente");
        return 1;
    } else {
        printf("Credenciales incorrectas.\n");
        return 0;
    }
}
