#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<time.h>
#include "inf112808_str.c"


struct msgbuf my_msg1, my_msg2;
void komunikaty(int komunikat)
{
  switch(komunikat)
  {
    case 1: printf("Zostałeś pomyślnie zalogowany\n");break;
    case 2: printf("Blędne haslo lub login\n");break;
    case 3: printf("Lekarz o takiej nazwie nie istnieje\n");break;
    case 4: printf("Nie można ponownie się logować\n");break;
    case 5: printf("Wykorzystales wszystkie mozliwosci prob logowania\n");break;
    case 6: printf("Blad wczytywania\n");break;
    case 7: printf("Dane o urlopie zostaly umieszczone\n");break;
    case 15: printf("zostales pomyslnie wylogowany\n");break;
  }
}
void dane_urlopu()
{
   struct tm temp;
  /* time_t empty=NULL;
   struct tm* info;localtime(&empty);*/
  int dzien1,mies1,rok1;
    printf("podaj dzień rozpoczęcia urlopu\n");
   scanf("%d",&dzien1);
     printf("podaj miesiac rozpoczęcia urlopu\n");
    scanf("%d",&mies1);
    printf("podaj rok rozpoczęcia urlopu\n");
    scanf("%d",&rok1);
   time_t now = 0;
   printf("now %ld\n",now);
   localtime_r(&now, &temp);
   temp.tm_year=0;
   temp.tm_mday=dzien1;
    temp.tm_mon=mies1-1;
    temp.tm_year=rok1-1900;
    temp.tm_hour=1;
    temp.tm_min=0;
    temp.tm_sec=0;
    long czasod=mktime(&temp);
   
     //int dzien1,mies1,rok1;
    printf("podaj dzień zakończenia urlopu\n");
   scanf("%d",&dzien1);
     printf("podaj miesiac zakończenia urlopu\n");
    scanf("%d",&mies1);
    printf("podaj rok zakończenia urlopu\n");
    scanf("%d",&rok1);
   //time_t now = 0;
   printf("now %ld\n",now);
   localtime_r(&now, &temp);
   temp.tm_year=0;
   temp.tm_mday=dzien1;
    temp.tm_mon=mies1-1;
    temp.tm_year=rok1-1900;
    temp.tm_hour=1;
    temp.tm_min=0;
    temp.tm_sec=0;
    long czasdo=mktime(&temp);
   my_msg1.urlop_od=czasod;
    my_msg1.urlop_do=czasdo;
     printf("czasod %ld, czasdo %ld\n", my_msg1.urlop_od,my_msg2.urlop_do=czasdo);
}

int main(){
    int mid=msgget(0x123,0666|IPC_CREAT);

    my_msg1.id=getpid();
    my_msg1.mtype=1;
    my_msg1.kto='L';
    while(1){
        printf("1-logowanie\n");
        printf("2-urlop\n");
        printf("3-wylogowanie\n");
        scanf("%d",&my_msg1.akcja);
        switch(my_msg1.akcja){
            case 1:{
                printf("podaj nazwisko\n");
                scanf("%s",my_msg1.nazwisko);
                printf("podaj imei \n");
            scanf("%d",&my_msg1.imei);
                /*printf("MSGSND: %d queue_id: %d\n",*/ msgsnd(mid,&my_msg1,sizeof(struct msgbuf) - sizeof(long), 0);/*, mid);*/
                printf("Sent register request \n");
               /* printf("MSGRCV: %d\n",*/ msgrcv(mid, &my_msg2, sizeof(struct msgbuf) - sizeof(long), getpid(), 0);//);
                printf("Registered %d**********%s&&&&&&&&&&\n", my_msg2.komunikat, my_msg2.nazwisko);
                komunikaty(my_msg2.komunikat);
                break;
            }
            case 2:{
                 my_msg1.kto='L';
                dane_urlopu();
                 printf("podaj imei\n");
                scanf("%d",&my_msg1.imei);
               /*  printf("MSGSND: %d queue_id: %d\n",*/ msgsnd(mid,&my_msg1,sizeof(struct msgbuf) - sizeof(long), 0);/*, mid);*/
               /* printf("Sent register request \n");
                my_msg2.urlop_od=my_msg1.urlop_od;
                my_msg2.urlop_do=my_msg1.urlop_do;*/
                /*printf("MSGRCV: %d\n",*/ msgrcv(mid, &my_msg2, sizeof(struct msgbuf) - sizeof(long), getpid(), 0);//);
                printf("Registered %d**********%s&&&&&&&&&&\n", my_msg2.komunikat, my_msg2.nazwisko);
                komunikaty(my_msg2.komunikat);
            break;}
            case 3:{
                 my_msg1.kto='L';
                 msgsnd(mid, &my_msg1,sizeof(struct msgbuf) - sizeof(long), 0);
                 msgrcv(mid, &my_msg2,sizeof(struct msgbuf) - sizeof(long),getpid(), 0);
                komunikaty(my_msg2.komunikat);
                break;
                    }
        }
    }
    return 0;
}