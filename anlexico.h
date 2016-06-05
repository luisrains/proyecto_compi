/*********** Librerias utilizadas **************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


/************* Definiciones ********************/
#define L_CORCHETE 	      256
#define R_CORCHETE	      257
#define L_LLAVE 	      258
#define R_LLAVE 	      259
#define COMA		      260
#define DOS_PUNTOS	      261
#define STRING            262
#define NUMBER  	      263
#define PR_TRUE		      264
#define PR_FALSE	      265
#define PR_NULL           266

#define VACIO             1
#define TAMAUX            5
#define TAMCONJ           10
#define TAMLEX            50
#define TAMESP            101
#define TAMARRAY          500


/************* Estructuras ********************/
typedef struct {
	int compLex;
        char lexema[TAMLEX];
        int linea;
} token;


/************* Prototipos ********************/
void sigLex();
void agregar_espacio();


/*************Prototipos del parser************/
void json(int setSync[]);
void element();
void array(int setSync[]);
void arrayp(int setSync[]);
void element_list();
void element_listp(int setSync[]);
void object(int setSync[]);
void objectp(int setSync[]);
void atribute_list();
void atribute_listp(int setSync[]);
void atribute();
void atribute_name();
void atribute_value(int setSync[]);
void getToken();
void check_input(int con_primero[], int con_siguiente[]);
void match(int t);
void setsync(int []);
void parser();
void scanto(int setSync1[], int setSync2[]);
