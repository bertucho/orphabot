#include <stdio.h>
#include <winsock.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define SERVIDOR "5.145.168.3"
#define PUERTO 6667

void setnick(char *nick);
void ping(char *ping);
void join(char *canal);
int quien(char *quienes);
int correcto(char *buffer);
int verificar(char *comando,int tipo);
void conectar(void);
void mode(char *mode);
void privi(char*,char*);
void say(char *frase);
void setauto(int tipo);
void desetauto(int tipo);
void listar(int tipo);
void shell(char *comando);
DWORD WINAPI google(struct sgoogle *datos);
DWORD WINAPI cgroup(struct sforo *datos);



struct sgoogle{
       unsigned char tipo;
       char *busqueda;
       char user[17];
       }sgoogle;
       
struct sforo{
       unsigned char tipo;
       char user[17];
       }foro;
SOCKET clsock;
sockaddr_in sraddr;
WSADATA wsaData;
char buffer[2048],*p2,*p3,user[17],privado[17];
char temp[32],*ptr,temp2[200],*ptr2,*ptr3,accion[400],temp3[340],level[3][10][20],temp4[100],mensajes[10][100];
char nick[20]="orphabot",canal[20]="#orphabot",nicktemp[20],aleatorio[20],temprand[5];
int i,x=1,contador[3]={0,0,0},nomensajes=0;

int main(){
    HANDLE hilo;
    DWORD idhilo;
    conectar();
    int recibidos,boolean,random,boolean2=5,away=0,joining2=1;
    clsock=socket(AF_INET,SOCK_STREAM,0);
    if(clsock==INVALID_SOCKET){
                               printf("Error socket");
                               return 0;
                               }
    sraddr.sin_family=AF_INET;
    sraddr.sin_port=htons(PUERTO);
    sraddr.sin_addr.s_addr=inet_addr(SERVIDOR);
    
    if(connect(clsock,(sockaddr*)&sraddr,sizeof(sraddr))==SOCKET_ERROR){
                                   printf("Error connect");
                                   return 0;
                                   }
    recibidos=recv(clsock,buffer,sizeof(buffer)-1,0);
    buffer[recibidos] = '\0';
    send(clsock,"USER orphabot orphabot orphabot :orphabot\n",strlen("USER orphabot orphabot orphabot :orphabot\n"),0);
	setnick(nick);
	recibidos=recv(clsock, buffer, sizeof(buffer)-1, 0);
	buffer[recibidos] = '\0';
	if(strstr(buffer,"Nickname is already in use")!=NULL){
                                srand(clock());
			                    random = rand() % 10; 
			                    sprintf(temprand,"%d",random);
			                    strcat(nick,temprand);
			                    random = rand() % 10; 
			                    sprintf(temprand,"%d",random);
			                    strcat(nick,temprand);
			                    random = rand() % 10; 
			                    sprintf(temprand,"%d",random);
			                    strcat(nick,temprand);
			                    random = rand() % 10; 
			                    sprintf(temprand,"%d",random);
			                    strcat(nick,temprand);
			                    random = rand() % 10; 
			                    sprintf(temprand,"%d",random);
			                    strcat(nick,temprand);
                                setnick(nick);
                                joining2=0;
                                }
	printf("%s",buffer);
    int joining=0;
	for(;recibidos>0;){
        memset(temp,'\0',sizeof(temp));
        memset(temp2,'\0',sizeof(temp2));
        memset(accion,'\0',sizeof(accion));
        memset(temp3,'\0',sizeof(temp3)); 
        strncpy(temp,buffer,4);
        temp[4]='\0';
        if(strcmp(temp,"PING")==0){
		ping(buffer);
		joining=0;
        }   
        if(correcto(buffer)==2){
                                random=rand()%200;
                                sprintf(nick,"%d",random);
                                setnick(nick);
                                boolean=0;
                                }
        if(correcto(buffer)==0&&boolean2==1){
                                memset(nick,'\0',sizeof(nick));
                                strcpy(nick,nicktemp);
                                boolean2=0;
                                }
        //JOIN
        memset(temp,'\0',sizeof(temp));
        strcpy(temp,nick);
        if(boolean==0){
                       join("#botijo");
                       if(strstr(buffer," JOIN :#botijo")!=NULL) boolean=1;
                       }
        if(verificar(" join ",2)==0){
                              join(ptr);
                              }
        //NICK
        if(verificar(" nick ",2)==0){
                                        ptr2=strstr(ptr,"\n");
                                        ptr3=strstr(ptr,"\r");
                                        if(ptr2!=NULL||ptr3!=NULL){
                                                                   strcpy(ptr2,"\0");
                                                                   strcpy(ptr3,"\0");
                                                                   }
                                        strcpy(nicktemp,ptr); 
                                        printf("\n%s\n",nick);
                                        setnick(ptr);
                                        boolean2=1;
                                        }
        //PART
        if(verificar(" part",1)==0){
                            strcpy(accion,"PART ");
                            strcat(accion,ptr);
                            strcat(accion,"\n");
                            send(clsock,accion,strlen(accion),0);
                            }                              
        //QUIT
        if(verificar(" quit",2)==0){
                            say("adios\n");
                            memset(accion,'\0',sizeof(accion));
                            strcpy(accion,"QUIT\n");
                            send(clsock,accion,strlen(accion),0);
                            }
      
        
        //PRIVMSG
        if(verificar(" say",1)==0){
                            memset(accion,'\0',sizeof(accion));
                            strcpy(accion,"PRIVMSG ");
                            for(i=0;ptr[i]!=' ';i++);
                            i++;
                            strcpy(temp3+1,ptr+i);
                            ptr[i]='\0';
                            temp3[0]=':';
                            strcat(ptr,temp3);
                            strcat(accion,ptr);
                            send(clsock,accion,strlen(accion),0);
                            printf("%s",accion);
                            }
                            
        //TOPIC               
        if(verificar(" topic",1)==0){
                       memset(accion,'\0',sizeof(accion));
                       strcpy(accion,"TOPIC ");
                       for(i=0;ptr[i]!=' ';i++);
                       i++;
                       strcpy(temp3+1,ptr+i);
                       ptr[i]='\0';
                       temp3[0]=':';
                       strcat(ptr,temp3);
                       strcat(accion,ptr);
                       strcat(accion,"\n");
                       send(clsock,accion,strlen(accion),0);
                       }
                            
        //MODES
        if(verificar(" op",1)==0)
                       mode(" +o ");
        if(verificar(" voz",1)==0)
                       mode(" +v ");
        if(verificar(" deop",1)==0)
                       mode(" -o ");
        if(verificar(" devoz",1)==0)
                       mode(" -v ");
        if(verificar(" ban",1)==0)
                       mode(" +b ");
        if(verificar(" deban",1)==0)
                       mode(" -b ");
        if(verificar(" +m",1)==0)
                       mode(" +m ");
        if(verificar(" -m",1)==0)
                       mode(" -m ");
        if(verificar(" +t",1)==0)
                       mode(" +t ");
        if(verificar(" -t",1)==0)
                       mode(" -t ");
        if(verificar(" +n",1)==0)
                       mode(" +n ");
        if(verificar(" -n",1)==0)
                       mode(" -n ");
        if(verificar(" +s",1)==0)
                       mode(" +s ");
        if(verificar(" -s",1)==0)
                       mode(" -s ");
        if(verificar(" +l",1)==0)
                       mode(" +l ");
        if(verificar(" -l",1)==0)
                       mode(" -l ");
        if(verificar(" +k",1)==0)
                       mode(" +k ");
        if(verificar(" -k",1)==0)
                       mode(" -k ");
        if(verificar(" +i",1)==0)
                       mode(" +i ");
        if(verificar(" -i",1)==0)
                       mode(" -i ");
        if(verificar(" +p",1)==0)
                       mode(" +p ");
        if(verificar(" -p",1)==0)
                       mode(" -p ");
        if(verificar(" +R",1)==0)
                       mode(" +R ");
        if(verificar(" -R",1)==0)
                       mode(" -R ");
                       
        //KICK
        if(verificar(" kick",1)==0){
                       memset(accion,'\0',sizeof(accion));
                       strcpy(accion,"KICK ");
                       strcat(accion,ptr);
                       strcat(accion,"\n");
                       send(clsock,accion,strlen(accion),0);
                       }
        
        if(verificar(" autoop ",2)==0)
                       setauto(0);
        if(verificar(" deautoop ",2)==0)
                       desetauto(0);
        if(verificar(" listop",2)==0){
                       listar(0);
                       }
                  
        if(verificar(" autovoz ",2)==0)
                       setauto(1);
        if(verificar(" deautovoz ",2)==0)
                       desetauto(1);
        if(verificar(" listvoz",2)==0){
                       listar(1);
                       }
                       
        if(verificar(" autokick ",2)==0)
                       setauto(2);
        if(verificar(" deautokick ",2)==0)
                       desetauto(2);
        if(verificar(" listkick",2)==0)
                       listar(2);
        
        for(x=0;x<3;x++){
        for(i=0;i<contador[x];i++){
        if(quien(level[x][i])==1&&strstr(buffer,"JOIN :#s5.travian.master")!=NULL){
                                              if(x==0){
                                                     printf("efefefe");
                                                     memset(temp4,'\0',sizeof(temp4));
                                                     strcpy(temp4,"#s5.travian.master ");
                                                     strcat(temp4,level[x][i]);
                                                     ptr=temp4;
                                                     mode(" +o ");
                                                     }
                                              if(x==1){
                                                     memset(temp4,'\0',sizeof(temp4));
                                                     strcpy(temp4,"#s5.travian.master ");
                                                     strcat(temp4,level[x][i]);
                                                     ptr=temp4;
                                                     mode(" +v ");
                                                     }
                                              if(x==2){
                                                     memset(temp4,'\0',sizeof(temp4));
                                                     strcpy(temp4,"KICK ");
                                                     strcat(temp4,"#s5.travian.master ");
                                                     strcat(temp4,level[x][i]);
                                                     strcat(temp4,"\n");
                                                     send(clsock,temp4,strlen(temp4),0);
                                                     } 
                                                     }
                                   }
                       }
        if(quien("orphato")==1&&verificar(" away ",2)==0){
                                              if(strncmp(ptr,"on",2)==0){ 
                                                        away=1;
                                                        say("Hasta luego ;)");
                                                        }
                                              if(strncmp(ptr,"off",3)==0){
                                                        away=0;
                                                        if(nomensajes==0){
                                                                          say("No tienes mensajes");
                                                        }else{
                                                              say("Mensajes nuevos: ");
                                                              for(x=0;x<nomensajes;x++){
                                                                      say(mensajes[x]);
                                                                      Sleep(700);
                                                                      }
                                                              for(i=0;i<10;i++)
                                                              memset(mensajes[i],'\0',sizeof(mensajes[i]));
                                                              nomensajes=0;
                                                              }
                                                        }
                                              }
        if(away==1){
                    if(quien("orphato")!=1&&strstr(buffer,"PRIVMSG")!=NULL){
                                              ptr3=buffer;
                                              for(i=1;ptr3[i]!=':';i++);
                                              i++;
                                              if(strstr(ptr3+i,"orphato")){
                                                            say("orphato ahora mismo no esta");
                                                            say("puedes dejarle un mensaje de esta forma: orphabot mensaje el_mensaje_que_quieras");
                                                            say("ejemplo: orphabot mensaje vuelve desgraciao!!!");
                                                            }
                                              }
                    }                                              
        if(away==1&&verificar(" mensaje ",2)==1){
                                if(strlen(ptr)>98){
                                                   say("mensaje demasiado largo");
                                }else if(nomensajes>9){
                                                   say("ya no caben mas mensajes");
                                }else{
                                      for(i=1;buffer[i]!='!';i++)
                                      strncat(mensajes[nomensajes],buffer+i,1);
                                      strcat(mensajes[nomensajes],": ");
                                      for(i=0;ptr[i]!='\n'&&ptr[i]!='\r'&&ptr[i]!='\0';i++)
                                      strncat(mensajes[nomensajes],ptr+i,1);
                                      mensajes[nomensajes][i]='\0';
                                      nomensajes++;
                                      }
                                }     
                                                   
                                                   
        if(quien("orphato")==1&&verificar(" cmd ",2)==0){
                       shell(ptr);
                       } 
                       
        if(verificar(" google ",2)!=2){
                       quien(NULL);
                       strcpy(sgoogle.user,user);
                       sgoogle.busqueda=ptr;
                       sgoogle.tipo=1;
                       privi("Resultados:",sgoogle.user);
                       hilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)google,(PVOID)&sgoogle,0,&idhilo);
                       }
                       
        if(verificar(" aki-google ",2)!=2){
                       sgoogle.busqueda=ptr;
                       sgoogle.tipo=2;
                       hilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)google,(PVOID)&sgoogle,0,&idhilo);
                       }
                       
        if(verificar(" foro",2)!=2){
                       quien(NULL);
                       strcpy(foro.user,user);
                       foro.tipo=1;
                       privi("Ultimos posts. Formato (titulo |||| user |||| url):",foro.user);
                       hilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)cgroup,(PVOID)&foro,0,&idhilo);
                       }
        if(verificar(" aki-foro",2)!=2){
                       foro.tipo=2;
                       hilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)cgroup,(PVOID)&foro,0,&idhilo);
                       }
                       
        if(verificar(" query ",2)==0){
                       for(i=0;i<17&&ptr[i]!=' ';i++) strncpy(user+i,ptr+i,1);
                       user[i]='\0';
                       i++;
                       privi(ptr+i,user);
                       }
        
        if(joining==0||joining2==0){
		join(canal);
		joining=1;
        }
		recibidos=recv(clsock, buffer, sizeof(buffer)-1, 0);
		buffer[recibidos]='\0';
		printf("%s",buffer);
	}
	
}
void conectar(void){
     if(WSAStartup(MAKEWORD(1,1),&wsaData)!=0){
                                              printf("Error Startup");
                                              }
}

int correcto(char *buffer){
    char *p;
    memset(temp3,'\0',sizeof(temp3));
    temp3[0]=buffer[1];
    for(i=2;buffer[i]!=':';i++);
    strncat(temp3,buffer+i,1);
    if(strstr(temp3," 433 ")==NULL&&strstr(temp3," 438 ")==NULL) return 0;
    p=strstr(buffer,"Nick change too fast");
    if(p!=NULL) return 1;
    p=strstr(buffer,"Nickname is already in use");
    if(p!=NULL) return 2;
    return 0;
}

int verificar(char *comando,int tipo){
      memset(temp3,'\0',sizeof(temp3));
      memset(temp2,'\0',sizeof(temp2));
      strcpy(temp2,nick);
      strcat(temp2,comando);
      ptr=strstr(buffer,temp2);
      if(ptr==NULL) return 2;
      if(tipo!=2){
      if(strstr(ptr,"#")==NULL){
      strcpy(temp3,ptr+strlen(temp2));
      for(i=0;*(ptr-i)!='#';i++); //for(i=0;*(ptr-i)!='#'&&i<15;i++);
      for(;*(ptr-i)!=' ';i--)
      strncat(temp2,ptr-i,1);
      strcat(temp2," ");
      strcat(temp2,temp3);
      ptr=temp2+strlen(temp)+strlen(comando);
      }else{
            ptr=ptr+strlen(temp2)+1;
            }
      }else{
            ptr=ptr+strlen(temp2);
            }
      if(quien("orphato")==1) return 0;
      return 1;
      }

void mode(char *mode){
     memset(accion,'\0',sizeof(accion));
     strcpy(accion,"MODE ");
     int i;
     for(i=0;ptr[i]!=' '&&ptr[i]!='\r';i++)
     strncat(accion,ptr+i,1);
     strcat(accion,mode);
     i++;
     strcat(accion,ptr+i);
     strcat(accion,"\n");
     send(clsock,accion,strlen(accion),0);
     }

void setauto(int tipo){
     if(contador[tipo]>9){
                    say("La lista esta llena");
                    }else{
                       memset(temp4,'\0',sizeof(temp4));
                       for(i=0;ptr[i]!=' '&&ptr[i]!='\r'&&ptr[i]!='\n';i++)
                       strncat(level[tipo][contador[tipo]],ptr+i,1);
                       strcpy(temp4,"Agregado ");
                       strcat(temp4,level[tipo][contador[tipo]]);
                       strcat(temp4," a la lista");
                       say(temp4);
                       contador[tipo]++;
                       }
     }

void listar(int tipo){
     memset(temp4,'\0',sizeof(temp4));
     for(i=0;i<contador[tipo];i++){
                    strcat(temp4,level[tipo][i]);
                    strcat(temp4," ");
                    }
     say(temp4);
}
     
void desetauto(int tipo){
     memset(temp4,'\0',sizeof(temp4));
     for(i=0;ptr[i]!='\r'&&ptr[i]!='\n';i++)
     strncat(temp4,ptr+i,1);
     for(i=0;i<contador[tipo];i++){
                 if(strcmp(level[tipo][i],temp4)==0){
                             if(i==contador[tipo]-1){
                             memset(level[tipo][i],'\0',sizeof(level[tipo][i]));
                             }else{                  
                             for(x=i;x<contador[tipo]-1;x++){
                             strcpy(level[tipo][x],level[tipo][x+1]);
                             if(x==contador[tipo]-2)
                             memset(level[tipo][x+1],'\0',sizeof(level[tipo][x+1]));
                             }
                             }
                             memset(temp3,'\0',sizeof(temp3));
                             strcpy(temp3,"Borrado ");
                             strcat(temp3,temp4);
                             strcat(temp3," de la lista");
                             say(temp3);
                             contador[tipo]--;      
                             }
                 }
}

void setnick(char *name){
    char botnick[40];
    strcpy(botnick,"NICK ");
	strcat(botnick,name);
	strcat(botnick,"\n");
	printf("%s",botnick);
	send(clsock,botnick,strlen(botnick),0);
}

void ping(char *ping){
     char *p;
     char pong[50];
     strcpy(pong,ping);
     p=strchr(pong,'I');
     *p='O';
     printf("%s",pong);
     send(clsock,pong,strlen(pong),0);
}

void join(char *canal){
     char join[40]="JOIN ";
     if(strlen(canal)>30) return;
     strcat(join,canal);
     strcat(join,"\n");
     printf("\n%s",join);
     send(clsock,join,strlen(join),0);
}

int quien(char *quienes){
    char temp[32],*p,*p2;
    memset(temp,'\0',sizeof(temp));
    memset(user,'\0',sizeof(user));
    p2=temp;
    p=strstr(buffer,":");
    p++;
    int i;
    for(i=0;*p!='!'&&i<14;i++,p++,p2++){
               strncpy(p2,p,1);
               strncpy(user+i,p,1);
               }
    if(quienes==NULL) return -1;
    if(strcmp(temp,quienes)==0){
                                      return 1;
                                      }else{
                                            return 0;
                                            }
    }
    
void privi(char *frase,char *user){
     char temp[200];
     memset(accion,'\0',sizeof(accion));
     strcat(accion,"PRIVMSG ");
     strcat(accion,user);
     strcat(accion," :");
     for(i=0;frase[i]!='\0';i++){
     for(x=0;frase[i]!='\n'&&frase[i]!='\0'&&frase[i]!='\r'&&x<199;i++,x++)
                    strncpy(temp+x,frase+i,1);
     temp[x]='\0';
     strcat(accion,temp);
     printf("\n%s\n",accion);
     strcat(accion,"\n");
     send(clsock,accion,strlen(accion),0);
     accion[strlen(accion)-strlen(temp)-1]='\0';
     if(frase[i]=='\0') break;
     Sleep(1300);
     }
     }
     
void say(char *frase){
     char temp[200];
     int x;
     memset(accion,'\0',sizeof(accion));
     ptr3=strstr(buffer,"PRIVMSG #");
     ptr3+=strlen("PRIVMSG ");
     strcpy(accion,"PRIVMSG ");
     for(;*(ptr3-1)!=':';ptr3++)
     strncat(accion,ptr3,1);
     for(i=0;frase[i]!='\0';i++){
     for(x=0;frase[i]!='\n'&&frase[i]!='\0'&&frase[i]!='\r'&&x<199;i++,x++)
                    strncpy(temp+x,frase+i,1);
     temp[x]='\0';
     strcat(accion,temp);
     printf("\n%s\n",accion);
     strcat(accion,"\n");
     send(clsock,accion,strlen(accion),0);
     accion[strlen(accion)-strlen(temp)-1]='\0';
     if(frase[i]=='\0') break;
     Sleep(1200);
     }
     }
     
void shell(char *comando){
     int i,x;
     char buf[1025],*ptr;
     ptr=buf;
     for(i=0;i<strlen(comando);i++){
                                    if(comando[i]=='\n'||comando[i]=='\r') comando[i]=='\0';
                                    }
     FILE* consola;
     consola=_popen(comando,"r");
     if(consola==NULL){
                       return;
                       }else{
                             printf("\n\n%s\n\n",comando);
                             for(;!feof(consola);){
                             fread(buf,1,1023,consola);
                             strcat(buf,"\n");
                             printf("\n\n%s\n\n",buf);
                             say(buf);
                             memset(buf,'\0',sizeof(buf));
                             }
                             _pclose(consola);
                             }
}

DWORD WINAPI google(struct sgoogle *datos){
    char peticion[104]="GET /search?q=";
    char bufgl[30000],bufgl2[10000],*ptrgl,*ptrgl2,url[100],titulo[100],resul[212];
    short contador=0,tag=0;
    SOCKET glsock;
    sockaddr_in gladdr;
    WSADATA wsaData;
    if(strlen(datos->busqueda)>=39){
                             if(datos->tipo==1)
                             privi("busqueda demasiado larga",datos->user);
                             else if(datos->tipo==2) say("busqueda demasiado larga");
                             return 0;
                             }
    for(i=0;*(datos->busqueda+i)!='\n'&&*(datos->busqueda+i)!='\0'&&*(datos->busqueda+i)!='\t'&&*(datos->busqueda+i)!='\r';i++){
                                           if(*(datos->busqueda+i)==' '){
                                                              strncat(peticion,"+",1);
                                                         }else{
                                                               strncat(peticion,datos->busqueda+i,1);
                                                               }
                                           }
    strcat(peticion," HTTP/1.1\r\nHost: www.google.es\r\nAccept: text/plain\r\n\r\n");
    if(WSAStartup(MAKEWORD(1,1),&wsaData)!=0){
                                              printf("Error Startup");
                                              }
    glsock=socket(AF_INET,SOCK_STREAM,0);
    if(glsock==INVALID_SOCKET){
                               printf("Error socket");
                               return 0;
                               }
    
    gladdr.sin_family=AF_INET;
    gladdr.sin_port=htons(80);
    gladdr.sin_addr.s_addr=inet_addr("66.249.93.99");
    
    if(connect(glsock,(sockaddr*)&gladdr,sizeof(gladdr))==SOCKET_ERROR){
                                   printf("Error connect");
                                   return 0;
                                   }
    send(glsock,peticion,strlen(peticion),0);
    recv(glsock,bufgl,sizeof(bufgl)-1,0);
    recv(glsock,bufgl2,sizeof(bufgl2)-1,0);
    strcat(bufgl,bufgl2);
    recv(glsock,bufgl2,sizeof(bufgl2)-1,0);
    strcat(bufgl,bufgl2);
    free(bufgl2);
    printf("%s",bufgl);
    ptrgl=bufgl;
    for(;(ptrgl=strstr(ptrgl,"<h2 class=r><a class=l href=\""))!=NULL&&contador<6;contador++){
                   strcpy(titulo,"");
                   strcpy(url,"");
                   strcpy(resul,"");
                   ptrgl=ptrgl+strlen("<h2 class=r><a class=l href=\"");
                   for(i=0;*(ptrgl+i)!='\"'&&i<99;i++) strncat(url,ptrgl+i,1);
                   ptrgl+=i;
                   ptrgl+=2;
                   ptrgl2=strstr(ptrgl,"</a>");
                   for(i=0;ptrgl+i!=ptrgl2&&i<99;i++){
                                        if(*(ptrgl+i)=='<') tag=1;
                                        if(tag==0) strncat(titulo,ptrgl+i,1);
                                        if(*(ptrgl+i)=='>') tag=0;
                                        }
                   ptrgl+=i;
                   printf("%s",titulo);
                   strcpy(resul,titulo);
                   strcat(resul," |||| ");
                   strcat(resul,url);
                   strcat(resul,"\n");
                   if(datos->tipo==1)
                   privi(resul,datos->user);
                   else if(datos->tipo==2) say(resul);
                   }
    free(bufgl);
    closesocket(glsock);
    }
     
     
DWORD WINAPI cgroup(struct sforo *datos){
    char peticion[104]="GET /foro/index.php HTTP/1.1\r\nHost: www.c-group.org\r\nAccept: text/plain\r\n\r\n";
    char bufcg[17000],*ptrcg,*ptrcg2,url[160],titulo[100],resul[300],name[24];
    short contador=0,tag=0;
    memset(bufcg,'\0',sizeof(bufcg));
    SOCKET cgsock;
    sockaddr_in cgaddr;
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(1,1),&wsaData)!=0){
                                              printf("Error Startup");
                                              }
    cgsock=socket(AF_INET,SOCK_STREAM,0);
    if(cgsock==INVALID_SOCKET){
                               printf("Error socket");
                               return 0;
                               }
    
    cgaddr.sin_family=AF_INET;
    cgaddr.sin_port=htons(80);
    cgaddr.sin_addr.s_addr=inet_addr("213.66.69.252");
    
    if(connect(cgsock,(sockaddr*)&cgaddr,sizeof(cgaddr))==SOCKET_ERROR){
                                   printf("Error connect");
                                   return 0;
                                   }
    send(cgsock,peticion,strlen(peticion),0);
    for(i=0;i<10;i++)
    recv(cgsock,bufcg,sizeof(bufcg)-1,0);
    ptrcg=bufcg;
    printf("\n%s\n",bufcg);
    for(contador=0;(ptrcg=strstr(ptrcg,"<td class=\"middletext\" valign=\"top\" nowrap=\"nowrap\"><b><a href=\""))!=NULL&&contador<5;contador++){
                      strcpy(url,"");
                      strcpy(titulo,"");
                      strcpy(resul,"");
                      strcpy(name,"");
                      ptrcg+=strlen("<td class=\"middletext\" valign=\"top\" nowrap=\"nowrap\"><b><a href=\"");
                      for(i=0;*(ptrcg+i)!='\"'&&i<158;i++) strncat(url,ptrcg+i,1);
                      printf("\n\n%s",url);
                      ptrcg+=i;
                      ptrcg+=2;
                      ptrcg2=strstr(ptrcg,"</a>");
                      for(i=0;ptrcg+i!=ptrcg2&&i<99;i++) strncat(titulo,ptrcg+i,1);
                      printf("\n\n%s",titulo);
                      ptrcg+=i;
                      ptrcg+=strlen("</a></b> por ");
                      if(*(ptrcg)=='<'){
                                          ptrcg=strstr(ptrcg,"\">");
                                          ptrcg+=2;
                                          }
                      ptrcg2=strstr(ptrcg,"</a>");
                      for(i=0;ptrcg+i!=ptrcg2&&i<23;i++) strncat(name,ptrcg+i,1);
                      printf("\n\n%s",name);
                      ptrcg+=i;
                      strcpy(resul,titulo);
                      strcat(resul," |||| ");
                      strcat(resul,name);
                      strcat(resul," |||| ");
                      strcat(resul,url);
                      strcat(resul,"\n");
                      if(datos->tipo==1) privi(resul,datos->user);
                      else if(datos->tipo==2) say(resul);
                      }
    closesocket(cgsock);
    free(bufcg);
}
     
