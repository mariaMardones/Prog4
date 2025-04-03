#include <stdio.h>
#include "../bd/bd.h"

int main() {
    sqlite3 *db;
    if (inicializar_bd(&db) == 0) {
        printf("Base de datos creada correctamente.\n");
    }
    cerrar_bd(db);
    return 0;
}
