/*
 *	Curso: Compiladores y Lenguajes de Bajo de Nivel
 *	Descripcion: Implementacion de un analizador lexico que reconoce el BNF para un lenguaje JSON
 *	
 */


/*********** Inclusion de cabecera **************/
#include "anlexico.h"


/************* Variables globales **************/
token t; // Token global para recibir componentes del analizador lexico.


/***** Variables para el analizador lexico *****/
FILE *archivo; // Fuente pascal.
token array_tokens[TAMARRAY];
int numLinea = 1; // Numero de Linea.
int con = -1; // Variale que indica la cantidad de espacio que ya se ha cargado en 
int tamano_actual = 0;
char id[TAMLEX]; // Utilizado por el analizador lexico.
char cont_esp[TAMESP]; // Variabe para contar la cantidad de espacio.
char msg1[41];


/**************** Funciones *********************/
// Rutinas del analizador lexico

void error(const char* mensaje) 
{
    printf(" Lin %d: Error Lexico. %s", numLinea, mensaje);
}

void sigLex() 
{
    int i = 0;
    int ban = 0;
    int acepto = 0;
    int estado = 0;
    char c = 0;
    char msg[41];
    char aux[TAMAUX] = " "; // Vector auxiliar para leer false, null o true.
    con = -1;
    token e;

    while ((c = fgetc(archivo)) != EOF) 
    {
        if (c == '\n') {
            // Incrementar el numero de linea.
            numLinea++;
            continue;
        } else if (c == ' ') 
        {
            do 
            {
                // Se encarga de cargar los espacios leidos para luego imprimirlos.
                con++;
                cont_esp[con] = ' ';
                c = fgetc(archivo);
            } while (c == ' ');
            c = ungetc(c, archivo);
        } else if (c == '\t') 
        {
            // Si es un tabulador que guarde los 4 espacios correspondientes.
            while (c == '\t') 
            {
                printf("%c", c);
                c = fgetc(archivo);
            }
        } else if (isdigit(c)) 
        {
            // Si es un numero.
            i = 0;
            estado = 0;
            acepto = 0;
            id[i] = c;
            while (!acepto) 
            {
                switch (estado) 
                {
                    case 0: // Una secuencia netamente de digitos, puede ocurrir . o e
                        c = fgetc(archivo);
                        if (isdigit(c)) 
                        {
                            id[++i] = c;
                            estado = 0;
                        } else if (c == '.') 
                        {
                            id[++i] = c;
                            estado = 1;
                        } else if (tolower(c) == 'e') 
                        {
                            id[++i] = c;
                            estado = 3;
                        } else
                            estado = 6;
                        break;
                    case 1: // Un punto, debe seguir un digito. 
                        c = fgetc(archivo);
                        if (isdigit(c)) 
                        {
                            id[++i] = c;
                            estado = 2;
                        } else 
                        {
                            sprintf(msg, "No se esperaba '%c' despues del . ", c);
                            estado = -1;
                        }
                        break;
                    case 2: // La fraccion decimal, pueden seguir los digitos o e.
                        c = fgetc(archivo);
                        if (isdigit(c)) 
                        {
                            id[++i] = c;
                            estado = 2;
                        } else if (tolower(c) == 'e') 
                        {
                            id[++i] = c;
                            estado = 3;
                        } else
                            estado = 6;
                        break;
                    case 3: // Una e, puede seguir +, - o una secuencia de digitos.
                        c = fgetc(archivo);
                        if (c == '+' || c == '-') 
                        {
                            id[++i] = c;
                            estado = 4;
                        } else if (isdigit(c)) 
                        {
                            id[++i] = c;
                            estado = 5;
                        } else 
                        {
                            sprintf(msg, "Se esperaba signo o digitos despues del exponente");
                            estado = -1;
                        }
                        break;
                    case 4:// Necesariamente debe venir por lo menos un digito.
                        c = fgetc(archivo);
                        if (isdigit(c)) 
                        {
                            id[++i] = c;
                            estado = 5;
                        } else 
                        {
                            sprintf(msg, "No se esperaba '%c' despues del signo", c);
                            estado = -1;
                        }
                        break;
                    case 5:// Una secuencia de digitos correspondiente al exponente.
                        c = fgetc(archivo);
                        if (isdigit(c)) 
                        {
                            id[++i] = c;
                            estado = 5;
                        } else
                            estado = 6;
                        break;
                    case 6: // Estado de aceptacion, devolver el caracter correspondiente a otro componente lexico.
                        if (c != EOF)
                            ungetc(c, archivo);
                        else
                            c = 0;
                        id[++i] = '\0';
                        acepto = 1;
                        t.compLex = NUMBER;
                        t.linea=numLinea;
                        strcpy(t.lexema, id);
                        array_tokens[tamano_actual] = t;
                        tamano_actual++;
                        break;
                    case -1:
                        if (c == EOF)
                            error("No se esperaba el fin de archivo\n");
                        else
                            error(msg);
                        acepto = 1;
                        t.compLex = VACIO;
                        t.linea=numLinea;
                        while (c != '\n')
                            c = fgetc(archivo);
                        ungetc(c, archivo);
                        break;
                }
            }
            break;
        } else if (c == '\"') 
        {
            // Si es un caracter o una cadena de caracteres.
            i = 0;
            id[i] = c;
            i++;
            do 
            {
                c = fgetc(archivo);
                if (c == '\"') 
                {
                    id[i] = c;
                    i++;
                    ban = 1;
                    break;
                } else if (c == EOF || c == ',' || c == '\n' || c == ':') 
                {
                    while (c != '\n') 
                    {
                        c = fgetc(archivo);
                        if (c == '\"') 
                        {
                            ban = 1;
                        }
                    }
                    if (ban == 0) 
                    {
                        sprintf(msg, "Se esperaba que finalize el literal");
                        error(msg);
                    }

                    ungetc(c, archivo);
                    con = -1;
                    break;
                } else 
                {
                    id[i] = c;
                    i++;
                }
            } while (isascii(c) || ban == 0);
            id[i] = '\0';
            strcpy(t.lexema, id);
            t.compLex = STRING;
            t.linea=numLinea;
            array_tokens[tamano_actual] = t;
            tamano_actual++;
            break;
        } else if (c == ':') 
        {
            // Si es un :
            t.compLex = DOS_PUNTOS;
            t.linea=numLinea;
            strcpy(t.lexema, ":");
            array_tokens[tamano_actual] = t;
            tamano_actual++;
            break;
        } else if (c == ',') 
        {
            t.compLex = COMA;
            t.linea=numLinea;
            strcpy(t.lexema, ",");
            array_tokens[tamano_actual] = t;
            tamano_actual++;
            break;
        } else if (c == '[') 
        {
            t.compLex = L_CORCHETE;
            t.linea=numLinea;
            strcpy(t.lexema, "[");
            array_tokens[tamano_actual] = t;
            tamano_actual++;
            break;
        } else if (c == ']') 
        {
            t.compLex = R_CORCHETE;
            t.linea=numLinea;
            strcpy(t.lexema, "]");
            array_tokens[tamano_actual] = t;
            tamano_actual++;
            break;
        } else if (c == '{') 
        {
            t.compLex = L_LLAVE;
            t.linea=numLinea;
            strcpy(t.lexema, "{");
            array_tokens[tamano_actual] = t;
            tamano_actual++;
            break;
        } else if (c == '}') 
        {
            t.compLex = R_LLAVE;
            t.linea=numLinea;
            strcpy(t.lexema, "}");
            array_tokens[tamano_actual] = t;
            tamano_actual++;
            break;
        } else if (c == 'n' || c == 'N') 
        {
            ungetc(c, archivo);
            fgets(aux, 5, archivo); //ver si es null
            if (strcmp(aux, "null") == 0 || strcmp(aux, "NULL") == 0) 
            {
                t.compLex = PR_NULL;
                t.linea=numLinea;
                strcpy(t.lexema, aux);
                array_tokens[tamano_actual] = t;
                tamano_actual++;
            } else 
            {
                sprintf(msg, "%c no esperado", c);
                error(msg);

                while (c != '\n')
                    c = fgetc(archivo);
                t.compLex = VACIO;
                t.linea=numLinea;
                ungetc(c, archivo);
            }
            break;
        }
        else if (c == 'f' || c == 'F') 
        {
            ungetc(c, archivo);
            fgets(aux, 6, archivo); //ver si es null
            if (strcmp(aux, "false") == 0 || strcmp(aux, "FALSE") == 0) 
            {
                t.compLex = PR_FALSE;
                t.linea=numLinea;
                strcpy(t.lexema, aux);
                array_tokens[tamano_actual] = t;
            tamano_actual++;
            } else 
            {
                sprintf(msg, "%c no esperado", c);
                error(msg);

                while (c != '\n')
                    c = fgetc(archivo);
                t.compLex = VACIO;
                t.linea=numLinea;
                ungetc(c, archivo);
            }
            break;
        }
        else if (c == 't' || c == 'T') 
        {
            ungetc(c, archivo);
            fgets(aux, 5, archivo); //ver si es null
            if (strcmp(aux, "true") == 0 || strcmp(aux, "TRUE") == 0) 
            {
                t.compLex = PR_TRUE;
                t.linea=numLinea;
                strcpy(t.lexema, aux);
                array_tokens[tamano_actual] = t;
                tamano_actual++;
            } else 
            {
                sprintf(msg, "%c no esperado", c);
                error(msg);

                while (c != '\n')
                    c = fgetc(archivo);
                t.compLex = VACIO;
                t.linea=numLinea;
                ungetc(c, archivo);
            }
            break;
        } else if (c != EOF) 
        {
            sprintf(msg, "%c no esperado", c);
            error(msg);
            while (c != '\n')
                c = fgetc(archivo);
            strcpy(cont_esp, " ");
            con = -1; // Variable que controla los espacios que se imprimen.
            ungetc(c, archivo);
        }
    }
    if (c == EOF) 
    {
        t.compLex = EOF;
        t.linea=numLinea;
        strcpy(t.lexema, "EOF");
        sprintf(t.lexema, "EOF");
        array_tokens[tamano_actual] = t;
        tamano_actual++;
    }
}


/******************************************************* P A R S E R *******************************************************************/

token token1;
int setSync[TAMCONJ];
int posicion=-1;
int errores= 0;

void error_sintac(const char* mensaje) 
{
    errores=1; 
   printf(" Error sintactico. %s",  mensaje);

}

void match(int t)
{
    if(t == token1.compLex)
    {
        getToken();
    }else
    {
        sprintf(msg1, "En la linea %d No se esperaba  %s\n", token1.linea, token1.lexema );
        error_sintac(msg1);
    }
}

void getToken()
{
    posicion++;
    token1 = array_tokens[posicion];
}

void parser()
{
    getToken();
    int con_sig []={EOF,0,0,0,0,0,0,0};
    json(con_sig);
}

void json(int setSync[])
{
    if(token1.compLex == L_LLAVE || token1.compLex == L_CORCHETE){
        element();
    }else
    {
        sprintf(msg1, "En la linea %d Se esperaba L_LLAVE o L_CORCHETE\n" , token1.linea);
        error_sintac(msg1);
    }
}

void element()
{ 
    int con_primero[] = {L_LLAVE, L_CORCHETE,0,0,0,0,0,0};
    int con_sig []={EOF,COMA, R_CORCHETE, R_LLAVE,0,0,0,0};
    check_input(con_primero, con_sig);
    switch(token1.compLex)
    {
        case L_LLAVE:
            object(con_sig);
            break;
        case L_CORCHETE:
            array(con_sig);
            break;
        default:
            sprintf(msg1, "En la linea %d Se esperaba L_LLAVE o L_CORCHETE\n" , token1.linea);
            error_sintac(msg1);
            break;
    }
    check_input(con_sig,con_primero);
}

void object(int setSync[])
{
    int con_primero[]={L_LLAVE,0,0,0,0,0,0,0};
    int con_sig[]={EOF,COMA, R_CORCHETE, R_LLAVE,0,0,0,0};
    check_input(con_primero,con_sig);
    switch (token1.compLex)
    {
        case L_LLAVE:
            match(L_LLAVE);        
            objectp(setSync);
            break;
        default:
            sprintf(msg1, "En la linea %d Se esperaba L_LLAVE\n" , token1.linea);
            error_sintac(msg1);
            break;
    }   
    check_input(con_sig,con_primero);
}

void array(int setSync[])
{
    int con_primero[]={L_CORCHETE,0,0,0,0,0,0,0};
    int con_si[] = {EOF,COMA, R_CORCHETE, R_LLAVE,0,0,0,0};
    check_input(con_primero,con_si);
    switch (token1.compLex)
    {
        case L_CORCHETE:
            match(L_CORCHETE);        
            arrayp(setSync);
            break;
        default:
            sprintf(msg1, "En la linea %d Se esperaba L_CORCHETE\n" , token1.linea);
            error_sintac(msg1);
            break;
    }
    check_input(con_si,con_primero);
}

void objectp(int setSync[])
{
    int con_primero[]={STRING,R_LLAVE,0,0,0,0,0,0};
    int con_si[] = {EOF,COMA, R_CORCHETE, R_LLAVE,0,0,0,0};
    check_input(con_primero,con_si);
    switch (token1.compLex)
    {
        case STRING:
            atribute_list();
            match(R_LLAVE);        
            break;
        case R_LLAVE:
            match(R_LLAVE);
            break;
        default:
            sprintf(msg1, "En la linea %d Se esperaba STRING o R_LLAVE\n" , token1.linea);
            error_sintac(msg1);

            break;
    }
    check_input(con_si,con_primero);
}

void arrayp(int setSync[])
{
    int con_primero[]={L_LLAVE, L_CORCHETE, R_CORCHETE,0,0,0,0,0};
    int con_si[] = {EOF,COMA, R_CORCHETE, R_LLAVE,0,0,0,0};
    check_input(con_primero,con_si);
    switch (token1.compLex)
    {
        case L_LLAVE:
            element_list();
            match(R_CORCHETE);        
            break;
        case L_CORCHETE:
            element_list();
            match(R_CORCHETE);
            break;
        case R_CORCHETE:
            match(R_CORCHETE);
            break;
        default:
            sprintf(msg1, "En la linea %d Se esperaba L_LLAVE, L_CORCHETE o R_CORCHETE\n" , token1.linea);
            error_sintac(msg1);
            break;
    }
    check_input(con_si,con_primero);
}

void element_list()
{
    int con_primero[]={L_LLAVE,L_CORCHETE,0,0,0,0,0,0};
    int con_siguient[]={R_CORCHETE,0,0,0,0,0,0,0};
    check_input(con_primero,con_siguient);
    if(token1.compLex ==L_LLAVE || token1.compLex == L_CORCHETE){
        element();
        element_listp(con_siguient);
    }else{
        sprintf(msg1, "En la linea %d Se esperaba L_LLAVE, L_CORCHETE\n" , token1.linea);
        error_sintac(msg1);
        check_input(con_siguient,con_primero);
    }
}


void element_listp(int setSync[])
{
    int con_primero[]={COMA,0,0,0,0,0,0,0};
    int con_sig[]={R_CORCHETE};
    if(token1.compLex==R_CORCHETE){
        check_input(con_sig,con_primero);
    }else{
        check_input(con_primero,con_sig);
    }
    switch(token1.compLex)
    {
        case COMA:
            match(COMA);
            element();
            element_listp(con_sig);
            break;
    }
   check_input(con_sig,con_primero);
}

void atribute()
{
    int con_primero[]={STRING,0,0,0,0,0,0,0};
    int con_siguien[]={COMA,R_LLAVE,0,0,0,0,0,0};
    check_input(con_primero,con_siguien);
    switch(token1.compLex)
    {
        case STRING:
            atribute_name();
            match(DOS_PUNTOS);
            atribute_value(con_siguien);
            break;
        default:
            sprintf(msg1, "En la linea %d Se esperaba STRING\n" , token1.linea);
            error_sintac(msg1);
            break;
    }
}

void atribute_list()
{
    int con_primero[]={STRING,0,0,0,0,0,0,0};
    int con_siguie[]={R_LLAVE,0,0,0,0,0,0,0};
    check_input(con_primero,con_siguie);
    if(token1.compLex==STRING){
        atribute();
        atribute_listp(con_siguie);
    }else{
        sprintf(msg1, "En la linea %d Se esperaba STRING\n" , token1.linea);
        error_sintac(msg1);
    }
    check_input(con_siguie,con_primero);
}

void atribute_listp(int setSync[])
{
    int con_primero[]={COMA,0,0,0,0,0,0,0};
    int con_s[]={R_LLAVE,0,0,0,0,0,0,0};
    
    if(token1.compLex == R_LLAVE){
        check_input(con_s,con_primero);
    }else{        
        check_input(con_primero,con_s);
    }
    switch(token1.compLex)
    {
        case COMA:
            match(COMA);
            atribute();
            atribute_listp(con_s);
            break;
    }
    check_input(con_s,con_primero);
}

void atribute_name()
{
    int con_primero[]={STRING,0,0,0,0,0,0,0};
    int con_sigui[]={DOS_PUNTOS,0,0,0,0,0,0,0};
    check_input(con_primero,con_sigui);
    switch(token1.compLex)
    {
        case STRING:
            match(STRING);
            break;
        default:
            sprintf(msg1, "En la linea %d Se esperaba STRING\n" , token1.linea);
            error_sintac(msg1);
            break;
    }
}

void atribute_value(int setSync[])
{
    int con_primero[]={L_LLAVE, L_CORCHETE, STRING, NUMBER, PR_TRUE, PR_FALSE,PR_NULL,0};
    int con_siv[]={COMA,R_LLAVE,0,0,0,0,0,0};
    check_input(con_primero,con_siv);
    switch(token1.compLex)
    {
        case L_LLAVE:
            element();
            break;
        case L_CORCHETE:
            element();
            break;
        case STRING:
            match(STRING);
            break;
        case NUMBER:
            match(NUMBER);
            break;
        case PR_TRUE:
            match(PR_TRUE);
            break;
        case PR_FALSE:
            match(PR_FALSE);
            break;
        case PR_NULL:
            match(PR_NULL);
            break;
        default:
            error_sintac("Se esperaba L_LLAVE, L_CORCHETE, STRING, NUMBER, TRUE,FALSE O NULL\n");
            break;
    }
}

/***********Funciones para el panic mode***********/

void scanto(int setSync1[], int setSync2[])
{
    setSync[16];
    int i=0;
    int ban=0;
    int tam= 8;
    int tam1 = 16;
    int j;   
    /***volcado de los dos array de syncronizacion*/
    for(j=0; j<8;j++)
    {
        setSync[j] = setSync1[j];
    }
    int k;
    int y=0;
    for(k=8; k<16;k++){
        setSync[k] = setSync2[y];
        y++;
    }
    do{
        while(i<tam1)
        { 
            if(token1.compLex != setSync[i] && token1.compLex != EOF)
            {
                // Poner bandera y si no esta en el set de sincronizaciones entonces hacer get y volver a empezar el for o do while.
                ban = 1;
                i++;
            }else{
                ban=0;
                i=16;
                break;
            }
       }
       if(ban==1)
       {
           getToken();
           i=0;
       }
    }while(i<tam1);
}

void check_input(int con_primero[], int con_siguiente[])
{
    int tam= 8;
    int i;
    int ban1= 0; // Si es cero si esta en el conjunto primero.    
    for (i = 0; i < tam; i++) 
    {
        if(token1.compLex == con_primero[i])
        {
            ban1=0;
            break;
        }else{
            ban1 = 1; // Si se prende es porque el token no esta en el conjuntoprimero y debe lanzar un error.
        }
    }    
    if(ban1==1)
    {
        sprintf(msg1, "En la linea %d No se esperaba  %s\n", token1.linea, token1.lexema);
        error_sintac(msg1);
        scanto(con_primero, con_siguiente);
    }
}
 
 /********************************************************* M A I N **********************************************************************/

int main(int argc, char* args[]) 
{
    // Imprimir un \n.
    if (argc > 1) 
    {
        if (!(archivo = fopen(args[1], "rt"))) 
        {
            printf("Archivo no encontrado.\n");
            exit(1);
        }
        while (t.compLex != EOF) 
        {
            sigLex();   
        }   
        fclose(archivo);
        parser();
        if(errores == 0)
        {
            printf("Es Correctamente Sintactico\n");
        }else{
            printf("No es correctamente sintactico\n");
        }
    } else {
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }
    return 0;
}

