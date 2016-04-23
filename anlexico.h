/*********** Librerias utilizadas **************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/************* Definiciones ********************/

//Codigos
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
// Fin Codigos
#define TAMAUX                5
#define TAMLEX                50
#define TAMESP               101
#define VACIO                 1


/************* Estructuras ********************/

typedef struct {
	int compLex;
        char lexema[TAMLEX];
} token;

/************* Prototipos ********************/
void sigLex();
void agregar_espacio();
