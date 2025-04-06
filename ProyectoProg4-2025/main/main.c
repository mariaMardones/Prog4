#include "../bd/bd.h"
#include "../domain/menus.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    sqlite3 *db;
    if (inicializar_bd(&db) != 0) {
        return 1;
    }

    manejar_menu_principal(db);

    cerrar_bd(db);
    return 0;
}
