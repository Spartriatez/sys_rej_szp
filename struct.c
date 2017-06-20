#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <string.h>
#include <time.h>

#define MAX_LEKARZE 10
#define SECONDS_TO_REMIND -14*24*3600
#define REMINDER_FLAG (1 << 19)
/*
time_t start_time;

time_t my_time(time_t* timer);*/

struct lekarz{
    char im[20];
    char nazw[20];
    long id;
    int dzien[5];
    int imei;
    int godzin[5];
    int godzout[5];
    int max;
    int czy_zalogowany;
    int npacjentow;
    long urlop_od;
    long urlop_do;
};

struct msgbuf {
    long mtype;
    int akcja;
    char kto;
    long id;
    int komunikat;
    long int pesel ;
    char imie[40];
    char nazwisko[64];
    char haslo[20];
    int maxlek;
    struct lekarz lekarze2[10];
    long kiedy;
    int czy_usunac;
    long idwizyty;
    char imlek[20];
    char nazwlek[20];
    int leklist[10];
    int maxlistlek;
    int imei;
    long urlop_od;
    long urlop_do;
    long tabdni[30];
    int maxdni;
} msgbuf;

int maxl=0;
struct lekarz lekarze[MAX_LEKARZE];
/*
time_t my_time(time_t* timer)
{
  return start_time + (time(NULL) - start_time) * 24 * 60 * 60;
}*/

void podmien(int x)
{
  switch(x)
  {
    case 1: printf("poniedzialek");break;
    case 2: printf("wtorek");break;
    case 3: printf("sroda");break;
    case 4: printf("czwartek");break;
    case 5: printf("piatek");break;
  }
}
void wykonaj(/*struct lekarz *lek,int *maxl2*/)
{
    int i=0;
    strcpy(lekarze[maxl].im,"Andrzej");
    strcpy(lekarze[maxl].nazw,"Lis");
    lekarze[maxl].dzien[i]=1;
    lekarze[maxl].godzin[i]=8;
        lekarze[maxl].godzout[i]=12;
       i++;
     lekarze[maxl].dzien[i]=3;
       lekarze[maxl].godzin[i]=8;
      lekarze[maxl].godzout[i]=12;
        i++;
     lekarze[maxl].dzien[i]=4;
      lekarze[maxl].godzin[i]=9;
      lekarze[maxl].godzout[i]=13;
         i++;
      lekarze[maxl].dzien[i]=5;
        lekarze[maxl].godzin[i]=8;
       lekarze[maxl].godzout[i]=12;
      i++;
       lekarze[maxl].max=i;
       i=0;
       lekarze[maxl].czy_zalogowany=0;
       lekarze[maxl].npacjentow=0;
       lekarze[maxl].imei=1;
       maxl++;
       
     strcpy(lekarze[maxl].im,"Marianna");
    strcpy(lekarze[maxl].nazw,"Pogodna");
    lekarze[maxl].dzien[i]=1;
    lekarze[maxl].godzin[i]=10;
      lekarze[maxl].godzout[i]=15;
       i++;
     lekarze[maxl].dzien[i]=2;
       lekarze[maxl].godzin[i]=14;
      lekarze[maxl].godzout[i]=18;
        i++;
     lekarze[maxl].dzien[i]=4;
      lekarze[maxl].godzin[i]=10;
      lekarze[maxl].godzout[i]=15;
       i++;
      lekarze[maxl].dzien[i]=5;
        lekarze[maxl].godzin[i]=10;
      lekarze[maxl].godzout[i]=15;
      i++;
       lekarze[maxl].max=i;
       i=0;
       lekarze[maxl].czy_zalogowany=0;
       lekarze[maxl].npacjentow=0;
       lekarze[maxl].imei=2;
       maxl++;
       
        strcpy(lekarze[maxl].im,"Anna");
    strcpy(lekarze[maxl].nazw,"Kwiatkowska");
    lekarze[maxl].dzien[i]=1;
    lekarze[maxl].godzin[i]=13;
      lekarze[maxl].godzout[i]=18;
     i++;
     lekarze[maxl].dzien[i]=2;
       lekarze[maxl].godzin[i]=12;
     lekarze[maxl].godzout[i]=18;
        i++;
     lekarze[maxl].dzien[i]=4;
      lekarze[maxl].godzin[i]=11;
      lekarze[maxl].godzout[i]=16;
        i++;
      lekarze[maxl].dzien[i]=5;
        lekarze[maxl].godzin[i]=8;
      lekarze[maxl].godzout[i]=12;
      i++;
       lekarze[maxl].max=i;
       lekarze[maxl].czy_zalogowany=0;
       lekarze[maxl].npacjentow=0;
       lekarze[maxl].imei=3;
       i=0;
       maxl++;
       
        strcpy(lekarze[maxl].im,"Stefan");
    strcpy(lekarze[maxl].nazw,"Olszyna");
    lekarze[maxl].dzien[i]=1;
    lekarze[maxl].godzin[i]=13;
        lekarze[maxl].godzout[i]=18;
       i++;
     lekarze[maxl].dzien[i]=2;
    lekarze[maxl].godzin[i]=8;
        lekarze[maxl].godzout[i]=12;
      i++;
     lekarze[maxl].dzien[i]=3;
       lekarze[maxl].godzin[i]=13;
       lekarze[maxl].godzout[i]=18;
         i++;
     lekarze[maxl].dzien[i]=4;
      lekarze[maxl].godzin[i]=13;
      lekarze[maxl].godzout[i]=18;
       i++;
      lekarze[maxl].dzien[i]=5;
        lekarze[maxl].godzin[i]=13;
        lekarze[maxl].godzout[i]=18;
       i++;
       lekarze[maxl].max=i;
       i=0;
       lekarze[maxl].czy_zalogowany=0;
       lekarze[maxl].npacjentow=0;
       lekarze[maxl].imei=4;
       maxl++;
       
       strcpy(lekarze[maxl].im,"Lucyna");
    strcpy(lekarze[maxl].nazw,"Lisowska");
    lekarze[maxl].dzien[i]=3;
    lekarze[maxl].godzin[i]=11;
       lekarze[maxl].godzout[i]=16;
       i++;
       lekarze[maxl].max=i;
       i=0;
       lekarze[maxl].czy_zalogowany=0;
       lekarze[maxl].npacjentow=0;
       lekarze[maxl].imei=5;
       maxl++; 
       
        strcpy(lekarze[maxl].im,"Kamila");
    strcpy(lekarze[maxl].nazw,"Poniatowska");
     lekarze[maxl].dzien[i]=3;
       lekarze[maxl].godzin[i]=8;
        lekarze[maxl].godzout[i]=13;
        i++;
     lekarze[maxl].dzien[i]=4;
      lekarze[maxl].godzin[i]=8;
       lekarze[maxl].godzout[i]=13;
         i++;
      lekarze[maxl].dzien[i]=5;
        lekarze[maxl].godzin[i]=8;
      lekarze[maxl].godzout[i]=13;
       i++;
       lekarze[maxl].max=i;
       i=0;
       lekarze[maxl].czy_zalogowany=0;
       lekarze[maxl].npacjentow=0;
       lekarze[maxl].imei=6;
       maxl++;
      
}

void wypisz(struct lekarz *lk,int maxl2)
{
   int s=0;
    int l=0;
  while(s<maxl2)
 {
    printf("%s %s\n",lk[s].im,lk[s].nazw);
    for(l=0;l<lk[s].max;l++){
     podmien(lk[s].dzien[l]);
     printf(": ");
     printf("%d",lk[s].godzin[l]);
        printf(".00-");
        
        
       printf("%d",lk[s].godzout[l]);
     printf(".00");
         printf("\n");
   }
   printf("\n");
    s++;
 }
} 

void wypiszzz(struct lekarz *lk,int maxl2)
{
   int s=0;
    int l=0;
  while(s<maxl2)
 {
    printf("%s %s\n",lk[s].im,lk[s].nazw);
    for(l=0;l<lk[s].max;l++){
     podmien(lk[s].dzien[l]);
     printf(": ");
     printf("%d",lk[s].godzin[l]);
        printf(".00-");    
       printf("%d",lk[s].godzout[l]);
     printf(".00");
         printf("\n");
   }
    printf("imei %d %d ",lk[s].imei,lk[s].npacjentow);
    
   printf("\n");
    s++;
 }
} 

//void kopia(
/*int main(int argc,char *argv[])
{
  wykonaj();
wypisz(lekarze,maxl);
  return 0;
}*/
/*
time_t* sprawdzTermin(struct lekarz* lek, time_t dzien)
{
    int d, i, j;
    int size = 0;
    struct tm* dzienTm = localtime(&dzien);
    time_t* wynik = malloc(0);
    (*dzienTm).tm_min = 0;  //obcinamy do pełnych godzin
    (*dzienTm).tm_sec = 0;  //obcinamy do pełnych godzin
    
    for(d = 0; d < (*lek).max; d++)
    {
        if((*lek).dzien[d] == (*dzienTm).tm_wday)
        {
            break;  //w tym dniu mogą być dostępne terminy - zmienna d
        }
        if((*lek).dzien[d])
        {
            continue;
            return 0;  //nie znaleźliśmy terminów w danym dniu
        }
    }
    
    for(i = (*lek).godzin[d]; i < (*lek).godzout[d]; i++)
    {
        (*dzienTm).tm_hour = i; // sprawdzamy po kolei każdą godzinę dostępną w danym dniu dla danego lekarza
        time_t tempTime = mktime(dzienTm);
        int bool = 1;
        for(j = 0; j < sizeof((*lek).wizyty) / sizeof(time_t); j++)  //spradzamy, czy termin nie jest już zajęty
        {
            if((*lek).wizyty[j] == tempTime)
            {
                bool = 0;
                break;
            }
        }
        if(bool)
        {
            wynik = realloc(wynik, ++size * sizeof(wynik));
            wynik[sizeof(wynik) - 1] = tempTime;
        }
    }
}

time_t* dostepneTerminy()
{
    time_t sprawdz = time(NULL);
    struct tm* sprawdzTm = localtime(&sprawdz);
    int i, j;
    for(i = 0; i < MAX_LEKARZE; i++)
    {
        for(j = 0; j < lekarze[i].max; j++)  //sprawdzamy kolejne dni tygodnia
        {
            //if(lekarze[i].dzien[j] == *sprawdzTm.tm_wday && lekarze[i].godzin[j] < *sprawdzTm.tm_hour
        }
    }
}

void dodajWizyte(time_t kiedy, struct lekarz* lek)
{
    // wstawienie gotowego terminu wizyty do danego lekarza - czyli po prostu wstawienie timemstemapa do listy skierowanej albo tablicy
}

void usunWizyte(time_t kiedy, struct lekarz* lek)
{
    int i;
    for(i = 0; i < (*lek).lwizyt; i++)
    {
        if((*lek).wizyty[i] == kiedy)
        {
            int j;
            for(j = i; j < (*lek).lwizyt; j++)
            {
                (*lek).wizyty[i] = (*lek).wizyty[i + 1];
            }
            (*lek).wizyty = realloc((*lek).wizyty, --((*lek).lwizyt));
            //break;
        }
    }
}*/