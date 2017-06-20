#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include "inf112808_str.c"
struct msgbuf my_msg1, my_msg2;

void* reminder(void* ptr);
void* interactive(void* ptr);
int mid;

void komunikaty(int komunikat)
{
  switch(komunikat)
  {
    case 1: printf("Proces rejestracji zostal pomyslnie zakonczony\n");break;
    case 2: printf("Użytkownik o takiej nazwie już istnieje\n");break;
    case 3: printf("Zostałeś pomyślnie zalogowany\n");break;
    case 4: printf("Blędne haslo lub login\n");break;
    case 5: printf("Uzytkownik o takiej nazwie nie istnieje\n");break;
    case 6: printf("Nie można ponownie się logować\n");break;
    case 7: printf("Wykorzystales wszystkie mozliwosci prob logowania\n");break;
    case 8: printf("Oto wypis lekarzy\n");break;
    case 9: printf("Blad wczytywania\n");break;
    case 10: printf("Twoja data zostal zmieniona\n");break;
    case 11: printf("Wizyta została odwolana\n");break;
    case 12: printf("status wizyty\n");break;
    case 13: printf("Dni w ktorych przyjmuje lekarz\n");break;
    case 14: printf("Lista lekarzy przyjmujaca w danym dniu\n\n");break;
    case 15: printf("zostales pomyslnie wylogowany\n");break;
  }
}

int main(){
   pthread_t interactive_t, reminder_t;
    int interactive_ret, reminder_ret;

    printf("Start main\n");
    
    interactive_ret = pthread_create(&interactive_t, NULL, interactive, (void*) NULL);
    if(interactive_ret){
        printf("Error: interactive_ret: %d\n", interactive_ret);
        exit(EXIT_FAILURE);
    }
    
    reminder_ret = pthread_create(&reminder_t, NULL, reminder, (void*) NULL);
    if(reminder_ret){
        printf("Error: reminder_ret: %d\n", reminder_ret);
        exit(EXIT_FAILURE);
    }
    
    pthread_join(interactive_t, NULL);
    pthread_join(reminder_t, NULL);
    printf("Exit main %d %d\n", interactive_ret, reminder_ret);
    exit(EXIT_SUCCESS);
    
 // interactive();
}

void* interactive(void* ptr)
{
    mid=msgget(0x123,0666|IPC_CREAT);
    //char textTmp[40];
    ///rejestracja
    my_msg1.id=getpid();
    my_msg1.mtype=1;
    my_msg1.kto='P';
    while(1){
    printf("1-rejestracja\n");
    printf("2-logowanie\n");
    printf("3-wypis lekarzy\n");
    printf("4-wypis lekarzy przyjmujacych w danym dniu\n");
    printf("5-wypis terminow do danego lekarza\n");
    printf("6-status wizyty\n");
    printf("7-zmiana wizyty\n");
    printf("8-odwolanie wizyty\n");
    printf("9-wylogowanie\n");
    scanf("%d",&my_msg1.akcja);

    switch(my_msg1.akcja){
    case 1:{
        printf("podaj imie\n");
        scanf("%s",my_msg1.imie);
        printf("podaj nazwisko\n");
        scanf("%s",my_msg1.nazwisko);
        printf("podaj pesel\n");
        scanf("%ld",&my_msg1.pesel);
        printf("haslo\n");
        scanf("%s", my_msg1.haslo);
    
        msgsnd(mid, &my_msg1,sizeof(struct msgbuf) - sizeof(long), 0);
        msgrcv(mid, &my_msg2, sizeof(struct msgbuf) - sizeof(long), getpid(), 0);
      
      //  printf("Registered %d**********%s&&&&&&&&&&\n", my_msg2.komunikat, my_msg2.imie);

        komunikaty(my_msg2.komunikat);
        static const char default_format[] = "%D %R %u";
        const char *format = default_format;
        struct tm bb;
        char res[32];
        time_t new2 =my_msg2.kiedy;
        printf("new 2 %ld\n",new2);
        localtime_r(&new2, &bb);
        if (strftime(res, sizeof(res), format, &bb) == 0) {
            fprintf(stderr,  "strftime(3): cannot format supplied "
                "date/time into buffer of size %u "
                "using: '%s'\n",
                (unsigned)sizeof(res), format);
            return 0;
        }
        printf("%lu -> %s\n",new2,res);

        break;
    }
    case 2:
    {
        printf("podaj pesel\n");
        scanf("%ld",&my_msg1.pesel);
        printf("podaj haslo\n");
        scanf("%s",my_msg1.haslo);
        msgsnd(mid, &my_msg1,sizeof(struct msgbuf) - sizeof(long), 0);
        msgrcv(mid, &my_msg2,sizeof(struct msgbuf) - sizeof(long),getpid(), 0);
        komunikaty(my_msg2.komunikat);
        break;
    }
    case 3:
    {
        msgsnd(mid, &my_msg1,sizeof(struct msgbuf) - sizeof(long), 0);
        msgrcv(mid, &my_msg2,sizeof(struct msgbuf) - sizeof(long),getpid(), 0);
        komunikaty(my_msg2.komunikat);
        if(my_msg2.komunikat==8)
        {
            wypisz(my_msg2.lekarze2,my_msg2.maxlek);
        }
        else{
            my_msg2.komunikat=9;
            komunikaty(my_msg2.komunikat);
        }
        break;
    }
    case 6:{
        msgsnd(mid, &my_msg1,sizeof(struct msgbuf) - sizeof(long), 0);
        msgrcv(mid, &my_msg2,sizeof(struct msgbuf) - sizeof(long),getpid(), 0);
        komunikaty(my_msg2.komunikat);

        static const char default_format[] = "%D %R %u";
        const char *format = default_format;
        struct tm bb;
        char res[32];
        time_t new2 =my_msg2.idwizyty;
        printf("new 2 %ld\n",new2);
        localtime_r(&new2, &bb);
        if (strftime(res, sizeof(res), format, &bb) == 0) {
            fprintf(stderr,  "strftime(3): cannot format supplied "
                "date/time into buffer of size %u "
                "using: '%s'\n",
                (unsigned)sizeof(res), format);
            return 0;
        }
        printf("%lu -> %s\n",new2,res);
        printf("%s %s\n\n",my_msg2.imlek,my_msg2.nazwlek);

    break;
    }
    case 7:{
        
        msgsnd(mid, &my_msg1,sizeof(struct msgbuf) - sizeof(long), 0);
    msgrcv(mid, &my_msg2,sizeof(struct msgbuf) - sizeof(long),getpid(), 0);
    komunikaty(my_msg2.komunikat);
        static const char default_format[] = "%D %R %u";
    const char *format = default_format;
    struct tm bb;
    char res[32];
    time_t new2 =my_msg2.kiedy;
        printf("new 2 %ld\n",new2);
    localtime_r(&new2, &bb);
    if (strftime(res, sizeof(res), format, &bb) == 0) {
                fprintf(stderr,  "strftime(3): cannot format supplied "
                                        "date/time into buffer of size %u "
                                        "using: '%s'\n",
                                        (unsigned)sizeof(res), format);
                return 0;
        }
        printf("%lu -> %s\n",new2,res);

        break;
    }
    case 8:{
        printf("Czy jestes pewny odwolania wizyt, tak=1,nie=0, UWAGA!! po wybraniu opcjii tak razem z odwolaniem wizyty nastapi usuniecie uzytkownika z rejetrcji i wykona sie automatycze wylaczenie programu\n");
        int wybor;
        scanf("%d",&wybor);
        if(wybor==1){
        msgsnd(mid, &my_msg1,sizeof(struct msgbuf) - sizeof(long), 0);
    msgrcv(mid, &my_msg2,sizeof(struct msgbuf) - sizeof(long),getpid(), 0);
    komunikaty(my_msg2.komunikat);
    return 0;
        }
    break;
    }
    case 9:
    {
    msgsnd(mid, &my_msg1,sizeof(struct msgbuf) - sizeof(long), 0);
    msgrcv(mid, &my_msg2,sizeof(struct msgbuf) - sizeof(long),getpid(), 0);
    komunikaty(my_msg2.komunikat);
    break;
    }
     case 5:{
          msgsnd(mid, &my_msg1,sizeof(struct msgbuf) - sizeof(long), 0);
       msgrcv(mid, &my_msg2,sizeof(struct msgbuf) - sizeof(long),getpid(), 0);
       komunikaty(my_msg2.komunikat);
       int k;
       time_t new2;
       if(my_msg2.komunikat==13){
        printf("Lekarz %s %s przyjmuje w nastepujace dni:\n",my_msg2.imlek,my_msg2.nazwlek);
        static const char default_format[] = "%D";
        const char *format = default_format;
        struct tm bb;
        char res[32];
       for(k=0;k<my_msg2.maxdni;k++){
           new2 =my_msg2.tabdni[k];
       
       // printf("new 2 %ld\n",new2);
    localtime_r(&new2, &bb);
    if (strftime(res, sizeof(res), format, &bb) == 0) {
                                        fprintf(stderr,  "strftime(3): cannot format supplied "
                                        "date/time into buffer of size %u "
                                        "using: '%s'\n",
                                        (unsigned)sizeof(res), format);
                            return 0;
                        }
                            printf("%lu -> %s\n",new2,res);
                    }   
            break;
            }
     }
     case 4:{
          msgsnd(mid, &my_msg1,sizeof(struct msgbuf) - sizeof(long), 0);
          msgrcv(mid, &my_msg2,sizeof(struct msgbuf) - sizeof(long),getpid(), 0);
          komunikaty(my_msg2.komunikat);
          if(my_msg2.komunikat==14){
              time_t new2;
                    int i;
                    static const char default_format[] = "%D";
                    const char *format = default_format;
                    struct tm bb;
                    char res[32];
                    
                    new2 =my_msg2.kiedy;
                
                // printf("new 2 %ld\n",new2);
                    localtime_r(&new2, &bb);
                    if (strftime(res, sizeof(res), format, &bb) == 0) {
                                                    fprintf(stderr,  "strftime(3): cannot format supplied "
                                                    "date/time into buffer of size %u "
                                                    "using: '%s'\n",
                                                    (unsigned)sizeof(res), format);
                                        return 0;
                                    }
                    printf("%lu -> %s\n",new2,res);
                    for(i=0;i<my_msg2.maxlistlek;i++)
                    {
                        int tmp= my_msg2.leklist[i];
                        printf("%s %s\n",my_msg2.lekarze2[tmp].im,my_msg2.lekarze2[tmp].nazw);
                    }
                    printf("\n");
          }
         break;
     }
    }

    }
    return 0;
}

void* reminder(void* ptr){
    printf("Hello 'reminder' thread here!\n");
    struct msgbuf my_msg;
    int rcvresult;
    while(1){
        rcvresult = msgrcv(mid,&my_msg,sizeof(struct msgbuf) - sizeof(long), getpid() | REMINDER_FLAG, 0);
        if(rcvresult > 0){
            printf("PRZYPOMNIENIE\n");
            printf("Wizyta u %s %s.\n", my_msg.imlek, my_msg.nazwlek);
            
              time_t new2;
                    int i;
                    static const char default_format[] = "%D";
                    const char *format = default_format;
                    struct tm bb;
                    char res[32];
                    new2 =my_msg.idwizyty;
                
                // printf("new 2 %ld\n",new2);
                    localtime_r(&new2, &bb);
                    if (strftime(res, sizeof(res), format, &bb) == 0) {
                                                    fprintf(stderr,  "strftime(3): cannot format supplied "
                                                    "date/time into buffer of size %u "
                                                    "using: '%s'\n",
                                                    (unsigned)sizeof(res), format);
                                        return 0;
                                    }
                    printf("%lu -> %s\n",new2,res);
        }
    }
}