#include <stdio.h>
#include <stdlib.h>
#include "menus.h"
#include "bd.h"

int main() {
    sqlite3 *db;

    if (!conectar_bd(&db)) {
        return 1;
    }

    crear_tablas(db); // solo crea "usuarios" de momento

    int opcion;
    do {
        mostrar_menu_principal();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Iniciar sesión (a implementar)...\n");
                // Aquí iría validación y mostrar menú según rol
                break;
            case 2:
                printf("Registro de usuario (a implementar)...\n");
                break;
            case 3:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 3);

    cerrar_bd(db);
    return 0;
}
