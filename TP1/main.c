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

#define MAX_MATRIX 30

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

int main()
{

    printf("\nTEST BORRABLE BEGIN \n\n");

    char * argv3[MAX_MATRIX][MAX_MATRIX] = {{"./myProgram","-clave1", "3", NULL},
                          {"./myProgram","-clave2", "3", "param1", "param2", "param3", "-clave1", "2", "-clave3", "-4", "-clave2", "2", "param3", NULL  },
                          {"./myProgram","-clave2", "3", NULL},
                          {"./myProgram","-clave1", "3", "-", NULL},
                          {"./myProgram", "-clave2", NULL} ,
                          {"./myProgram", "-", "7", NULL},
                          {"./myProgram", NULL},
                          {"./myProgram", "param1", "mal_escrito", NULL},
                          {NULL}  };

    int fils = 0;
    while(argv3[fils][0] != NULL && fils < (MAX_MATRIX - 1))    // -1 porque la ultima fila se reserva para la NULL
        fils++;

    if(fils == (MAX_MATRIX - 1) )
    {
        printf("ERROR DEL TESTER, demasiados tests ingresados\n");
    }
    else
    {
        for (int j = 0; j < fils; j++)
        {
            printf("\n\n#####TEST NRO %d ha comenenzado#####\n\n", j + 1);
    
            int argc3 = 0;
    
            while(argv3[j][argc3] != NULL && argc3 < MAX_MATRIX)
                argc3++;
    
            userDataInput_t * myPtr = malloc( argc3 * sizeof(userDataInput_t) );


            if(argc3 < MAX_MATRIX && argv3[j][argc3] == NULL)
            {
                for (int i = 0; i < argc3 ; i++)
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
}


int parseCallback(char * key, char * value, void * userData)
{
    userDataInput_t * pUserData = userData;
    int error = 0;

    int i = 0;
    while(pUserData[i].tipoDeArgumento != VOID )
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