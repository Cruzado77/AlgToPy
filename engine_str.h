#ifndef ENGINE_STR_H_INCLUDED
#define ENGINE_STR_H_INCLUDED

#include "common.h"
#include "engine_var.h"
#include "engine.h"
#include "engine_str.h"


#define BUFFER  180

#define OPTION_ERROR      -100
#define FILE_ERROR              -101
#define UNKNOW_ERROR  -102
#define MALLOC_ERROR    -103
#define DIR_ERROR               -104

extern int strdel(char *str,const char *del);
extern int strtroca(char *str, const char *anterior, const char *nova,int ocorrencias);
extern void exibe_erro (int erro);




char id_str[BUFFER] = "?0?";

typedef struct {
               char identificador[BUFFER];
               char conteudo[BUFFER];
               int num;
}strings;


char *strtolower (char *str);

strings *init_banco_str (void);

strings *resize_banco_str (strings *banco);

void destroi_banco_str (strings *banco);

int tam_banco_str (void);

char *reconhece_string (char *entrada, strings *banco_strings); //capaz de reconhecer uma string na entrada e retorná-la ou NULL;
                                                                                                                        //ATENÇÃO!!! PONTEIRO DEVE SER INICIALIZADO POR ALOCAÇÃO DINAMICA!!!

void identstr (strings *str,const char *conteudo);              //uma instancia de strings como argumento

char *restaura_strings (char *entrada,strings *banco_strings);



char *reconhece_string (char *entrada, strings *banco_strings)
{
               static char saida[BUFFER];

               int i,j, iInicio,iFim, contador = 0;

               strncpy(saida,entrada,BUFFER-1);

               //printf("entrada (%s)\n",entrada);
               for(i = 0; i< strlen(entrada); i++)
               {
                              //printf("%c\n",entrada[i]);
                              if(entrada[i] == 0x22 && entrada[i-1] != '\\')
                              {
                                             if(contador == 1) {
                                                            iFim = i +1;
                                                            j = iInicio;
                                                            for(i = 0; i < (iFim - iInicio); i++)
                                                            {
                                                                           saida[i] = entrada[j];
                                                                           j++;
                                                            }
                                                            saida[i] = '\0';

                                                            contador = tam_banco_str();

                                                            if(contador > BUFFER)break;                                 //evita o buffer overflow

                                                            //printf("Contador = %d\n",contador);
                                                            identstr(&banco_strings[contador-1],saida);
                                                           // printf("%d) string (%s), id (%s), saida (%s)\n",contador-1,banco_strings[contador-1].conteudo,banco_strings[contador-1].identificador,saida);

                                                            strtroca(entrada,banco_strings[contador-1].conteudo,banco_strings[contador-1].identificador,-1);
                                                            //strdel(entrada,saida);
                                                            //banco_strings = resize_banco_str(banco_strings);
                                                            //printf("contador = %d\n",contador) ;
                                                            /*for(int x = 0;x<contador;x++)
                                                            {
                                                                           printf("%d) id (%s), conteudo (%s)\n",x,banco_strings[x].identificador,banco_strings[x].conteudo);
                                                            }*/

                                                            contador = 0;
                                                            continue;
                                             }

                                             if(contador == 0) {
                                                            iInicio = i;
                                                            contador = 1;
                                                            continue;
                                             }

                              }
               }
               //if(contador) return NULL;


               //strncpy(entrada, saida,BUFFER);

               return saida;

}


strings *init_banco_str (void)
{
               strings *s;

               s = (strings *) malloc (sizeof(strings)*BUFFER);
               if(!s) exibe_erro (MALLOC_ERROR);

               return s;
}

int tam_banco_str (void)
{
               int i;
               sscanf(id_str,"?%d?",&i);
               //printf("tam = %d\n",i+1);
               if(i == 80)exibe_erro(MALLOC_ERROR);
               return i + 1;
}
void identstr (strings *str,const char *conteudo)
{
               //char temp[BUFFER];
               int id;

               //SALVA ID DA STRING
               strncpy(str->conteudo,conteudo,BUFFER-1);
               strncpy(str->identificador,id_str,BUFFER-1);

               //printf("(%s) , (%s)\n",str->identificador,str->conteudo);

               //strncpy(conteudo,id_str,BUFFER_LOW);
               //CONTABILIZA PROX IDENTIFICADOR

               sscanf(id_str,"?%d?",&id);
               id++;
               snprintf(id_str,BUFFER,"?%d?",id);

               //printf("id (%s)\n(%s)\n",id_str,str->conteudo);

}


strings *resize_banco_str (strings *banco)
{
               int i = tam_banco_str();
               strings *s;
               /*int i = tam_banco_str();
               strings *s = (strings *) malloc(sizeof(strings) * (num+1));

               for(int j = 0; j<i; j++)s[j] = banco[j];
               banco = (strings *) realloc(banco, sizeof(strings) * (num+2));
               if(!banco) exibe_erro(MALLOC_ERROR);
               //"asdaffff"
               destroi_banco_str(banco);

               banco = s;*/

               s = (strings *) realloc(banco,sizeof(strings[i]));
               if(!s) exibe_erro(MALLOC_ERROR);

               //banco = s;
               return s;
}


void destroi_banco_str (strings *banco)
{
               free(banco);
               banco = NULL;
}

char *restaura_strings (char *entrada,strings *banco_strings)
{
               int i,j;
               i = tam_banco_str();

               for(j = 0; j<i; j++)
               {
                              if(strstr(entrada,banco_strings[j].identificador)){
                                             //printf("(%s)\n",banco_strings[j].conteudo);
                                             strtroca(entrada,banco_strings[j].identificador,banco_strings[j].conteudo,-1);

                              }
               }

               return entrada;
}


char *strtolower (char *str)
{
               for(register int i = 0; i<strlen(str);i++)
               {
                              str[i] = tolower(str[i]);
               }

               return str;
}
#endif // ENGINE_STR_H_INCLUDED
