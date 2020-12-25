#ifndef ENGINE_VAR_H_INCLUDED
#define ENGINE_VAR_H_INCLUDED

#include "common.h"
#include "engine_var.h"
#include "engine.h"
#include "engine_str.h"

#define BUFFER   180

#define OPTION_ERROR      -100
#define FILE_ERROR              -101
#define UNKNOW_ERROR  -102
#define MALLOC_ERROR    -103
#define DIR_ERROR               -104

//GLOBAIS/////////////////////////////////////////////

int valor_variaveis = 0;


////////////////////////////////////////////////////////

extern void exibe_erro (int erro);

variavel *inicializa_var(void);

variavel *reconhece_variaveis(texto str,variavel *banco);

void destroi_var(variavel *banco);

int id_var(variavel *var,char *nome,char *tipo);

variavel *encontra_var (const char *str,const variavel *banco);


variavel *inicializa_var(void)
{
               variavel *p = (variavel *) malloc (sizeof(variavel)*BUFFER);
               if(!p) exibe_erro(MALLOC_ERROR);

               return p;
}

void destroi_var(variavel *banco)
{
               free(banco);
               banco = NULL;
}

variavel *reconhece_variaveis(texto str,variavel *banco)
{
               register int i,tipovar = 0;       //tipovar 0 = char, 1 = int, 2 = float

               for(i = 0; str[i]; i++)
               {
                              if(!strncmp(str[i],"caracter",BUFFER-1))tipovar = 0;
                              if(!strncmp(str[i],"inteiro",BUFFER-1) || !strncmp(str[i],"logico",BUFFER-1))tipovar = 1;
                              if(!strncmp(str[i],"real",BUFFER-1))tipovar = 2;
               }

               if(i == 1)return NULL;

               if(str[i-1]){
                              free(str[i-1]);
               }
               str[i-1] = NULL;

               for(i = 0;str[i];i++)
               {

                              if(encontra_var(str[i],banco)){
                                             printf("<<<< !Variavel %s ja declarada! >>>>\n",str[i]);
                                             interrupt =  1;
                                             return NULL;
                              }
                              else{
                                             if(tipovar == 0){
                                                            id_var(&banco[valor_variaveis],str[i],"str");
                                                            strncat(str[i],"=\"\"; ",BUFFER-1);
                                             }
                                             else{
                                                            if(tipovar == 1){
                                                                           id_var(&banco[valor_variaveis],str[i],"int");
                                                                           strncat(str[i],"=0; ",BUFFER-1);
                                                            }
                                                            else{
                                                                           if(tipovar == 2){
                                                                                          id_var(&banco[valor_variaveis],str[i],"float");
                                                                                          strncat(str[i],"=0.00; ",BUFFER-1);
                                                                           }
                                                            }
                                             }
                              }

               }

               return banco;
}

int id_var(variavel *var,char *nome,char *tipo)
{
               //char temp[BUFFER];

               //snprintf(temp,BUFFER-1,"%c%d",0xf4,valor_variaveis);

               //strncpy(var->identificador,temp,BUFFER-1);

               strncpy(var->nome,nome,BUFFER-1);
               strncpy(var->tipo,tipo,BUFFER-1);

               valor_variaveis++;
               return 1;
}


variavel *encontra_var (const char *str,const variavel *banco)
{
               register int i;
               static variavel saida;
               char nome[BUFFER];
               strncpy(nome,str,BUFFER-1);

               strdel(nome,"(");
               strdel(nome,")");
               strdel(nome, " ");

               for (i = 0; i <valor_variaveis; i++)
               {
                              if(!strncmp(banco[i].nome,nome,BUFFER))break;
                              else continue;
               }

               if(i == valor_variaveis) return NULL;

               saida = banco[i];
               return &saida;

}
#endif // ENGINE_VAR_H_INCLUDED
