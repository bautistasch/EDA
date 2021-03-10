/***************************************************************************//**
  @file     main.c
  @author   Grupo7
 ******************************************************************************/

 /*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseArg.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MAX_FIL 15
#define MAX_COL 30

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


typedef enum{NO_ERROR, WRONG_ARG} errorStates_t;    // calback variables 
typedef enum{VOID, PARAMETRO, OPCION} tipoDeArgumento_t; 


typedef struct 
{
    tipoDeArgumento_t tipoDeArgumento;
    errorStates_t errorState;
    union{
        char * parametro;
        char * clave;
    };
    int valor;
}userDataInput_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static int parseCallback(char * key, char * value, void * userData);

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

int main(int argc, char * argv[])
{
    printf("\nTEST BEGIN\n"); 
                                               
    char * argv3[MAX_FIL][MAX_COL] = {
/*1*/   {"./myProgram","-clave1", "3", NULL},   // OK. 1 OPCION
/*2*/   {"./myProgram","-clave2", "3", "param1", "param2", "param3", "-clave1", "2", "-clave3", "-4", "-clave2", "2", "param3", NULL  },// OK 4 OPCIONES, 4 PARAMS 
/*3*/   {"./myProgram","-clave1", "3", "-", NULL}, // ERROR TIPO 1, (OPCION SIN CLAVE)
/*4*/   {"./myProgram", "-clave2", NULL} , // ERROR TIPO 2, (NO VALOR)
/*5*/   {"./myProgram", "-", "7", NULL},// ERROR TIPO 1
/*6*/   {"./myProgram", NULL}, //OK. 0 ARGUMENTOS INGRESADOS
/*7*/   {"./myProgram", "param1", "mal_escrito", NULL},// ERROR argumento no registrado por la callBack
/*8*/   {"./myProgram", "-NO_SOY_LA_CLAVE", "mal_escrito", NULL}, // ERROR clave no registrada por la callback
/*9*/   {"./myProgram", "-clave1", "-1","-clave1","-5", NULL}, // OK. 2 OPCIONES
        {NULL}  // terminador usado para el test
        };

    int fils = 1;
    while(argv3[fils - 1][0] != NULL && fils < MAX_FIL)   
        fils++;

    if(fils == MAX_FIL && argv3[fils - 1][0] != NULL)
    {
        printf("ERROR DEL TESTER, no agregaste NULL final\n");
    }
    else
    {
        for (int j = 0; j < (fils - 1); j++)
        {
            printf("\n\n#####TEST NRO %d ha comenenzado#####\n\n", j + 1);
    
            int argc3 = 0;
    
            while(argv3[j][argc3] != NULL && argc3 < MAX_FIL)
                argc3++;
    
            userDataInput_t * myPtr = malloc( argc3 * sizeof(userDataInput_t) );
            if(argc3 < MAX_FIL && argv3[j][argc3] == NULL)
            {   

                for (int i = 0; i < argc3 ; i++)       // INICIALIZACION DE LA VARIABLE
                {
                    myPtr[i].tipoDeArgumento = VOID;
                    myPtr[i].errorState = NO_ERROR;
                }    

                int state = parseCmdLine(argc3, argv3[j], &parseCallback, myPtr );
                if( state != -1)
                {
                    printf("La cantidad de opciones y parametros ingresada fue de = %d\n", state);
    
                    for (int i = 0; i < argc3 && myPtr[i].parametro != VOID; i++)
                    {
                        if(myPtr[i].tipoDeArgumento == OPCION)
                        {
                            printf("CLAVE = %s\n", myPtr[i].clave);
                            printf("VALOR = %d\n\n", myPtr[i].valor);
                        }
                        else if(myPtr[i].tipoDeArgumento == PARAMETRO)
                        {
                            printf("PARAMETRO = %s\n\n", myPtr[i].parametro);
                        }
                    }
                }
                else
                {
                    if(myPtr[0].errorState == WRONG_ARG )
                        printf("WRONG ARGUMENT\n");
                }
            }
            else
            {
                printf("ERROR SISTEMA DE TESTEO\n");
            }

            free(myPtr);
        }
    }    

//##################################
//#######TEST DESDE CONSOLA#########
//##################################

    printf("TEST FROM CONSOLE IS ABOUT TO START\n\n");

    userDataInput_t * myPtr2 = malloc( argc * sizeof(userDataInput_t) );

    for (int i = 0; i < argc ; i++)       // INICIALIZACION DE LA VARIABLE
    {
        myPtr2[i].tipoDeArgumento = VOID;
        myPtr2[i].errorState = NO_ERROR;
    }  
    int state2 = parseCmdLine(argc, argv, &parseCallback, myPtr2 );
    if( state2 != -1)
    {
        printf("La cantidad de opciones y parametros ingresada fue de = %d\n", state2);
        for (int i = 0; i < argc && myPtr2[i].parametro != VOID; i++)
        {
            if(myPtr2[i].tipoDeArgumento == OPCION)
            {
                printf("CLAVE = %s\n", myPtr2[i].clave);
                printf("VALOR = %d\n\n", myPtr2[i].valor);
            }
            else if(myPtr2[i].tipoDeArgumento == PARAMETRO)
            {
                printf("PARAMETRO = %s\n\n", myPtr2[i].parametro);
            }
        }
    }
    else
    {
        if(myPtr2[0].errorState == WRONG_ARG )
            printf("WRONG ARGUMENT\n");
    }

    free(myPtr2);

}


int parseCallback(char * key, char * value, void * userData)
{
    userDataInput_t * pUserData = userData;
    int error = 0;

    int i = 0;
    while(pUserData[i].tipoDeArgumento != VOID )            // Busco hasta encontrar una variable aun no usada
        i++;

    if(key == NULL)
    {
        pUserData[i].tipoDeArgumento = PARAMETRO;
        if(strcmp(value, "param1") == 0)
        {
            pUserData[i].parametro = "Hola soy param1";
        }
        else if(strcmp(value, "param2") == 0)
        {
            pUserData[i].parametro = "Hola soy param2";
        }
        else if(strcmp(value, "param3") == 0)
        {
            pUserData[i].parametro = "Hola soy param3";
        }
        else if(strcmp(value, "param4") == 0)
        {
            pUserData[i].parametro = "Hola soy param4";
        }
        else
        {
            pUserData[0].errorState = WRONG_ARG; 
            error = 1;
        }
    }
    else if( strcmp(key, "-clave1") == 0)
    {
        pUserData[i].tipoDeArgumento = OPCION;
        pUserData[i].clave = "Hola soy la clave 1";
        pUserData[i].valor = atof(value);
    }
    else if( strcmp(key, "-clave2") == 0)
    {
        pUserData[i].tipoDeArgumento = OPCION;
        pUserData[i].clave = "Hola soy la clave 2";
        pUserData[i].valor = atof(value);
    }
    else if( strcmp(key, "-clave3") == 0)
    {
        pUserData[i].tipoDeArgumento = OPCION;
        pUserData[i].clave = "Hola soy la clave 3";
        pUserData[i].valor = atof(value);     
    }
    else
    {
        pUserData[0].errorState = WRONG_ARG;
        error = 1;
    }
    
    return !error;   
}