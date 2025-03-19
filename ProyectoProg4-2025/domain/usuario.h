#ifndef DOMAIN_USUARIO_H_
#define DOMAIN_USUARIO_H_

#define ADMINISTRADOR 1
#define CLIENTE 2

typedef struct {
    char nombre[50];
    char dni[10];
    char contrasena[50];
    int rol; // 1 = Administrador, 2 = Cliente
} Usuario;

Usuario registrarUsuario();
void iniciarSesion();

#endif /* DOMAIN_USUARIO_H_ */
