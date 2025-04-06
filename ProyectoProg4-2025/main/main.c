#include <stdio.h>
#include <stdlib.h>
#include "menus.h"
#include "bd.h"
#include "usuario.h"

int main() {
    sqlite3 *db;
    Usuario usuario;

    if (!conectar_bd(&db)) {
        return 1;
    }

    crear_tablas(db);

    int opcion;
    int sesion_iniciada = 0;

    do {
        mostrar_menu_principal();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                if (iniciar_sesion(db, &usuario)) {
                    sesion_iniciada = 1;

                    int subopcion;
                    if (usuario.rol == ROL_CLIENTE) {
                        do {
                            subopcion = mostrar_menu_cliente();
                            // Aquí se llamarán funciones específicas para cliente
                            printf("Elegiste opción %d como cliente (a implementar)\n", subopcion);
                        } while (subopcion != 6);
                    } else if (usuario.rol == ROL_ADMIN) {
                        do {
                            subopcion = mostrar_menu_admin();
                            // Aquí se llamarán funciones específicas para admin
                            printf("Elegiste opción %d como admin (a implementar)\n", subopcion);
                        } while (subopcion != 5);
                    }

                    sesion_iniciada = 0;
                }
                break;
            case 2:
                registrar_usuario(db);
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
