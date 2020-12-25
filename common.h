#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

/*
        Desenvolvido por: Raphael Rodrigues de Sena
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <locale.h>
#include <conio.h>


// MACROS
#define TEMP   "file.temp"


#define CLEAR                           "cls"
#define PAUSE                           "pause"

#define BUFFER  180
#define FILE_BUFFER           259

#define OPTION_ERROR      -100
#define FILE_ERROR              -101
#define UNKNOW_ERROR  -102
#define MALLOC_ERROR    -103
#define DIR_ERROR               -104
#define SYNTAX_ERROR      -105

// DADOS E OBJETOS

FILE *arq = NULL;
char file_name[FILE_BUFFER];


//PROTÓTIPOS

char *volta_pasta(char *pasta, int grau);

int checa_arquivo(const char *caminho);

int checa_dir (char *caminho);

void save_as ();

void exibe_top();

void exibe_erro(int erro);

static int exibe_menu(void);

static void seleciona_arquivo();


#include "engine_var.h"
#include "engine.h"
#include "engine_str.h"
#include "engine_func.h"

//extern's

extern int engine (int  (*entrada)  (char *str),int  (*saida)  (texto text));


void save_as ()
{
               char temp[BUFFER],sys_temp[FILE_BUFFER+BUFFER];
               int erro_sys;
               exibe_top();

               printf("Salvar como :  ");
               fgets(temp,BUFFER-1,stdin);
               fflush(stdin);
               tira_linha(temp);

               if(!strstr(temp,".py"))strncat(temp,".py",BUFFER-1);

               if(strlen(file_name)){
                              strncat(file_name,"\\",FILE_BUFFER-1);
                              volta_pasta(file_name,1);
               }
               strncat(file_name,temp,FILE_BUFFER-1);

               snprintf(sys_temp,FILE_BUFFER+BUFFER,"copy %s, \"%s\"",TEMP,file_name);
               if(system(sys_temp) != 0) return;
               printf("\nExecutar (s\\n) : ");
               do{
                              switch(getche())
                              {
                              case 's':
                              case 'S':
                                             goto executa;
                                             break;
                              case 'n':
                              case 'N':
                                             goto n_executa;
                                             break;
                              default:
                                             break;
                              }
               }while(1);
               executa:
                              exibe_top();
                              printf("\"%s\" no Python Shell:\n",file_name);
                              snprintf(sys_temp,FILE_BUFFER+BUFFER,"py \"%s\"",file_name);
                              if((erro_sys = system(sys_temp)))
                              {
                                             printf("Chamada Python Shell retorna erro (valor %d)",erro_sys);
                              }
                              printf("\n");
                              system("pause");
                                             n_executa:
                                             return;
}

void exibe_top(){
    system("cls");
    printf("-----------------------------------------\n");
    printf("           AlgToPy v.1.1\n");
    printf("-----------------------------------------\n");
}

static int exibe_menu()                                                         //Exibe o menu principal
{
    char str[BUFFER];
    int option;

    exibe_top();
    printf("1) Traduzir inline\n2) Abrir arquivo\n3) Creditos\n4) Sair\n");
    printf("-----------------------------------------\n");

    do{
        fgets(str,BUFFER-1,stdin);
        fflush(stdin);

        option = (int) strtol(str,NULL,9);

        if(option < 1 || option > 4) exibe_erro(OPTION_ERROR);
        else break;
    }while(1);

    return option;
}

static void seleciona_arquivo()
{
    DIR * dir;
      struct dirent * ptr;
      int option;

      register int i;

      char str[FILE_BUFFER];
      char warning[BUFFER] = "";
      char ** list;

      setlocale(LC_ALL, "Portuguese");

      dir = opendir ("./");
      if(!dir)return;

      strncpy(str,dir->dd_name,FILE_BUFFER);
      closedir(dir);

      for(i = 0; i <strlen(str); i++){
               if(str[i] == '*') str[i] = '\0';
      }

      list = (char **) malloc(sizeof(char *) * FILE_BUFFER);
      for(i = 0; i<FILE_BUFFER; i++)list[i] = (char *) malloc(sizeof(char[FILE_BUFFER]));

      do{

                     //dir->dd_name[strlen(dir->dd_name)-2] = '\0';

                     exibe_top();

                     dir = opendir(str);
                     if(!dir)volta_pasta(str,1);                                                       //Implementar para reconhecer arquivo ou pasta vazia

                     printf("%s\n(%s)\n",warning,str);
                     strncpy(warning,"",BUFFER-1);

                     for(i = 0; (ptr = readdir (dir)) && i < FILE_BUFFER; i++)
                     {
                              strncpy(list[i],ptr->d_name,FILE_BUFFER-1);
                              printf("%d) %s\n",i+1,list[i]);
                     }
                     printf("%d) Digitar caminho\n",i+1);
                     printf("%d) Cancelar\n",i+2);
                     closedir (dir);

                     do{
                              printf("\nDigite sua escolha : \n");
                              scanf("%d",&option);
                              fflush(stdin);

                              if(option<1 || option > (i+2))continue;
                              else break;
                     }while(1);

                     if((option == 1 || option ==2) && (strlen(str)>4))volta_pasta(str,option);              //Bloqueia a volta em C:
                     else{
                              if(option < (i+1)){
                                             //snprintf(str,BUFFER,"%s%s",str,list[option-1]);
                                             strncat(str,list[option-1],FILE_BUFFER-1);
                                             if(checa_arquivo(str) > 0){
                                                            strncpy(file_name,str,FILE_BUFFER-1);
                                                            arq = fopen(file_name,"r");
                                                            if(!arq){
                                                                           for(register int i = 0; i<FILE_BUFFER; i++)free(list[i]);
                                                                           free(list);
                                                                           return;
                                                            }
                                                            /*fwrite(str,sizeof(char),BUFFER,arq);
                                                            fclose(arq);
                                                            arq = NULL;*/

                                                            return;
                                             }
                                             else{
                                                            if(checa_arquivo(str) == 0){
                                                                           strncat(str,"\\",FILE_BUFFER-1);
                                                                           printf("\n%s\n", str);
                                                            }
                                                            else {
                                                                           strncpy(warning,"Arquivo de formato incompativel!\n",BUFFER-1);
                                                                           strdel(str,list[option-1]);
                                                                           continue;
                                                            }
                                             }
                              }
                              else{
                                             if(option == (i+2)){
                                                            arq = NULL;
                                                            break;
                                             }
                                             printf("\nCaminho >> ");
                                             scanf("%249s",str);
                                             fflush(stdin);
                                             if(checa_arquivo(str)){
                                                            strncpy(file_name,str,FILE_BUFFER-1);
                                                            arq = fopen(file_name,"r");
                                                            if(!arq){
                                                                           for(register int i = 0; i<FILE_BUFFER; i++)free(list[i]);
                                                                           free(list);
                                                                           return;
                                                            }


                                                            break;
                                             }
                                             else {
                                                            if(checa_dir(str))continue; //implementar
                                                            else{
                                                                           for(register int i = 0; i<FILE_BUFFER; i++)free(list[i]);
                                                                           free(list);
                                                                           return;
                                                            }
                                             }
                              }
                     }


      }while(1);
      for(register int i = 0; i<FILE_BUFFER; i++)free(list[i]);
      free(list);

      return;
}


char *volta_pasta(char *pasta, int grau)                    //recebe o caminho da pasta e o numero de pastas a voltar (1 ou 2)
{
               register int i;
               int ultima_barra = 0, penultima_barra; //barra[numero] = posição

               //barra = (int *) malloc(sizeof(int));
               for(i = 0; i < (strlen(pasta)-1); i++)       // -1 ignora a barra da pasta atual
               {
                              if(pasta[i] == '\\'){
                                             penultima_barra = ultima_barra;
                                             ultima_barra = i;
                              }
                              else continue;
               }
               if(ultima_barra == 0) {
                              //snprintf(pasta,80,"%s\\",pasta);                            //no caso de não haver barras, desenha-se a ultima
                              strncat(pasta,"\\",FILE_BUFFER);
                              return pasta;
               }
               if(grau == 1)pasta[ultima_barra+1] = '\0';
               if(grau == 2){
                              if(penultima_barra == 0){
                                             if(ultima_barra == 0)volta_pasta(pasta,0);
                                             else pasta[ultima_barra] = '\0';
                              }
                              else pasta[penultima_barra+1] = '\0';
                              //printf("\nchamando volta_pasta de novo\nstrlen(pasta) = %d\n",strlen(pasta));
                              volta_pasta(pasta,0);
               }

               return pasta;
}


int checa_arquivo(const char *caminho)                                     //implementar teste de extensão
{
               FILE *p = fopen(caminho,"r");
               if(!p) return 0;
               fclose(p);
               arq = NULL;

               if(strstr(caminho,".txt") ||  strstr(caminho,".TXT"))return 1;
               else{
                              if(strstr(caminho,".alg") ||  strstr(caminho,".ALG"))return 1;
                              else return -1;
               }
}

int checa_dir (char *caminho)
{
               DIR * dir;
               dir = opendir(caminho);
               if(!dir)return 0;

               closedir(dir);
               if(caminho[strlen(caminho)-1] != '\\')strncat(caminho,"\\",FILE_BUFFER-1);
               return 1;
}


void exibe_erro(int erro)                                                //Interpreta exceções: break para exceção que force a parada do programa
{
    switch(erro)
    {
    case OPTION_ERROR:
        printf("\nOpcao Inválida!!!\n");
        return;
    case MALLOC_ERROR:
        printf("\nERRO FATAL!!! Memoria indisponivel!!!\n");
        break;
    case FILE_ERROR:
        printf("\nERRO FATAL!!! Impossível manipular arquivos!!!\n");
        break;
    case DIR_ERROR:
        printf("\nErro! Arquivo ou pasta nao encontrados!!!\n");
        return;
    case SYNTAX_ERROR:
        printf("\nErro de sintax!!! Variavel ou comando inexestente!\n");
        return;
    }

    system(PAUSE);
    exit(-1);
}

#endif // COMMON_H_INCLUDED
