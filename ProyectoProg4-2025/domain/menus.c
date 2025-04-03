#include "../domain/menus.h"
#include <stdio.h>
#include "../bd/bd.h"

void menu_administrador() {
    printf("\n--- Menú Administrador ---\n");
    printf("1. Visualizar clientes\n");
    printf("2. Buscar cliente por DNI\n");
    printf("3. Eliminar cliente\n");
    printf("4. Gestionar reservas\n");
    printf("5. Salir\n");
    printf("Selecciona una opción: ");
}

void menu_cliente() {
    printf("\n--- Menú Cliente ---\n");
    printf("1. Hacer reserva\n");
    printf("2. Cancelar reserva\n");
    printf("3. Ver reservas activas\n");
    printf("4. Salir\n");
    printf("Selecciona una opción: ");
}

void mostrar_menu() {
    printf("\n--- Menú Principal ---\n");
    printf("1. Registrar usuario\n");
    printf("2. Iniciar sesión\n");
    printf("3. Salir\n");
    printf("Selecciona una opción: ");
}

void manejar_menu_principal(sqlite3 *db) {
    int opcion;
    char nombre[100], dni[20], contrasena[50];
    int rol;

    while (1) {
        mostrar_menu();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:  // Registrar usuario
                printf("Introduce nombre: ");
                scanf("%s", nombre);
                printf("Introduce DNI: ");
                scanf("%s", dni);
                printf("Introduce contraseña: ");
                scanf("%s", contrasena);
                printf("Introduce rol (0 = Cliente, 1 = Administrador): ");
                scanf("%d", &rol);

                if (registrar_usuario(db, nombre, dni, contrasena, rol) == 0) {
                    printf("Usuario registrado con éxito.\n");
                } else {
                    printf("Error al registrar el usuario.\n");
                }
                break;

            case 2:  // Iniciar sesión
                printf("Introduce DNI: ");
                scanf("%s", dni);
                printf("Introduce contraseña: ");
                scanf("%s", contrasena);

                if (iniciar_sesion(db, dni, contrasena, &rol) == 0) {
                    printf("Inicio de sesión exitoso.\n");
                    if (rol == 0) {
                        menu_cliente();  // Menú cliente
                    } else if (rol == 1) {
                        menu_administrador();  // Menú administrador
                    }
                } else {
                    printf("Error al iniciar sesión.\n");
                }
                break;

            case 3:  // Salir
                printf("Saliendo...\n");
                return;  // Termina el programa

            default:
                printf("Opción inválida. Intenta de nuevo.\n");
        }
    }
}
