#include <stdio.h>
#include "../bd/bd.h"
#include "../domain/menus.h"

int main() {
	printf("programa ejecutandose");
    sqlite3 *db;
    if (inicializar_bd(&db) == 0) {
        printf("Base de datos creada correctamente.\n");
    } else {
        printf("Error al inicializar la base de datos.\n");
        return 1;
    }

    int opcion;
    char dni[20], contrasena[100], nombre[100];
    int rol;

    while (1) {
        mostrar_menu();  // Muestra el menú principal
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:  // Registrar
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

                if (iniciar_sesion(db, dni, contrasena, &rol) == 0) {  // Iniciar sesión
                    printf("Bienvenido, %s\n", dni);
                    if (rol == 1) {
                        menu_administrador();  // Si es admin, muestra menú administrador
                    } else {
                        menu_cliente();  // Si es cliente, muestra menú cliente
                    }
                } else {
                    printf("Credenciales incorrectas.\n");
                }
                break;

            case 3:  // Salir
                cerrar_bd(db);  // Cierra la base de datos
                return 0;

            default:
                printf("Opción no válida.\n");
        }
    }

    return 0;
}
