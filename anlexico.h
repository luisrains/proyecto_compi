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
#define TAMCONJ               10
#define TAMARRAY              500
#define TAMTRA                1000



/************* Estructuras ********************/

typedef struct {
	int compLex;
        char lexema[TAMLEX];
        int linea;
} token;

typedef struct {
    char lexematrad[TAMLEX];
} traducido;

/************* Prototipos ********************/
void sigLex();
void agregar_espacio();
/*************Prototipos del parser************/

void json(int setSync[]);
void element(char elem[]);
void array(int setSync[], char arr[]);
void arrayp(int setSync[], char arrp[]);
void element_list(char elemtl[]);
void element_listp(int setSync[],char elemtlp[]);
void object(int setSync[],char obj[]);
void objectp(int setSync[], char objp[]);
void atribute_list(char attl[]);
void atribute_listp(int setSync[],char attlp[]);
void atribute(char att[]);
void atribute_name(char att_name[]);
void atribute_value(int setSync[], char att_value[]);
void getToken();
void check_input(int con_primero[], int con_siguiente[]);
void match(int t);
void setsync(int []);
void parser();
void scanto(int setSync1[], int setSync2[]);
void sin_comilla(char []);