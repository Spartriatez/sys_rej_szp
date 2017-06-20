#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include "inf112808_str.c"
#include "inf112808_sdaty.c"
#define MAX_USERS 40
#define MAX_DOCTORS 10

struct lekarz listlekarze[MAX_DOCTORS];
struct msgbuf my_msg1, my_msg2, msg_remind;
struct uzytkownik
{
    int id;
    int czyzalogowany;
    int ilerazy;
    long int pesel ;
    char imie[40];
    char nazwisko[64];
    char haslo[20];
};

typedef struct element {
    struct uzytkownik pacjent;
   struct element *next;
} el_listy;

 el_listy *first=NULL;

 time_t start_time;

time_t my_time(time_t* timer);

el_listy2 *wizyty=NULL, *wizyta;
 void zallek()
{
    wykonaj();
    int i;
    for(i=0;i<maxl;i++)
    {
        listlekarze[i]=lekarze[i];
    }     
}
long czaas[10];
int maxczas=0;
int minimum(struct lekarz* lek,int max)
{
   int min=lek[0].npacjentow;
    int i;
    int ktory=0;
for(i = 1; i<max;i++){
    printf("tabbbbbbbbbbbbb");
   if(listlekarze[i].npacjentow<min)
   {
       min=listlekarze[i].npacjentow;
       ktory=i;
  }
    }
  return ktory;
}

void usun_z_listy(el_listy *lista,long element)
 {
    if(first->pacjent.id==element)
    {
        el_listy *tmp=first;
        first=tmp->next;
        free(tmp);
    }
    else 
     {
   el_listy *wsk=lista;
   while (wsk->next != NULL)
     {
     if (wsk->next->pacjent.id == element) 
       {
       el_listy *usuwany=wsk->next;
       wsk->next= usuwany->next;   
       free(usuwany);               
       }
        else
         {
         wsk = wsk->next;           
         }                          
       }
     }    
 }
int losuj_wizyte(struct lekarz *lek, int ktory, time_t new,int *wybdzien)
{
    int d;
   
     int pomoc;
    time_t new2;
    int maxdni=0;
    int tabdni[10];
      int wybor=rand()%lek[ktory].max;
      (*wybdzien)=wybor;
    int dzientyg=lek[ktory].dzien[wybor];
    printf("zaczynamy\n");
     for(d = 0; d < 31; d++)
    {
    pomoc=(60+d)*24*3600;
    new2=new+pomoc;
   struct tm* dzienTemp = localtime(&new2);
   if((*dzienTemp).tm_wday==dzientyg){
        tabdni[maxdni++]=new2;
   }
    }
    
    int zdzien=rand()%maxdni;
    time_t wybrany_dzien=tabdni[zdzien];
    int delta=lek[ktory].godzout[wybor]-lek[ktory].godzin[wybor];
    int losgodz=(rand()%delta)+lek[ktory].godzin[wybor];
    int konwert=losgodz*3600;
    wybrany_dzien+=konwert;
      //  printf("zdzien %d wybrany %d dzientyg %d losgodz %d\n",zdzien,wybrany_dzien,dzientyg,losgodz);
        return wybrany_dzien;
}
void dodajWizyte(struct lekarz* lek, time_t dzien,int max,int czy,long dodatkowa)
{
    srand(time(0));
   struct tm* dzienTm = localtime(&dzien);
    (*dzienTm).tm_min = 0;  //obcinamy do pełnych godzin
    (*dzienTm).tm_sec = 0;  //obcinamy do pełnych godzin
    (*dzienTm).tm_hour = 1; // obcinamy godziny
    
    time_t new=mktime(dzienTm);
   // printf(" %d\n",new);
    int ktory;
    if(czy==-1)
    ktory=minimum(lek,max);
    else
        ktory=czy;
   int wybdzien=0;
    time_t wybranawizyta=losuj_wizyte(lek,ktory,new,&wybdzien);
    struct tm* czasTemp=localtime(&wybranawizyta);
  // printf("%d wybdzien %d\n",(*czasTemp).tm_hour-2,wybdzien);
  while(lek[ktory].godzout[wybdzien]<(*czasTemp).tm_hour-2)
    wybranawizyta=losuj_wizyte(lek,ktory,new,&wybdzien);
 // printf("%d wybdzien %d wybwiz %d\n",(*czasTemp).tm_hour-2,wybdzien,wybranawizyta);
  struct czasy czasTmp;
  czasTmp.data=wybranawizyta;
  czasTmp.idlekarz=lek[ktory].imei;
  czasTmp.idpacjent=dodatkowa;
  czasTmp.reminder_sent = 0;
  int bool=sprawdz2(wizyty,czasTmp);
  while(bool!=0)
  {
      wybranawizyta=losuj_wizyte(lek,ktory,new,&wybdzien);
      czasTmp.data=wybranawizyta;
        bool=sprawdz2(wizyty,czasTmp);
  }
  //printf("booll %d\n",bool);
  wizyty=first2;
  if(bool==0)
  {
      my_msg1.kiedy=czasTmp.data;
      dodaj_do_listy2(wizyty,czasTmp);
      czaas[maxczas++]=czasTmp.data;
  }
  lek[ktory].npacjentow+=1;
  wizyty=first2;
  wypisz_liste2(wizyty);
}


void zmienWizyte(el_listy2* list, long idpacjent,int xx)
{
     int ktory,imei;
usun_z_listy22(list,idpacjent,&imei);
//printf("kroe %d\n",imei);
int i;
for(i=0;i<maxl;i++)
{
    if(listlekarze[i].imei==imei)
    {
        ktory=i;
        break;
    }
}
dodajWizyte(listlekarze,xx,maxl,ktory,idpacjent);
wizyty=first2;
wypisz_liste2(wizyty);

}
void odwolajWizyte(el_listy2* list2,long idpacjent)
{
    int ktory;
usun_z_listy22(list2,idpacjent,&ktory);
printf("ktoryyyyyyyyyyyyy %d\n",ktory);
wizyty=first2;
usun_z_listy(first,idpacjent);
listlekarze[ktory].npacjentow-=1;
}

void usunWizyte(el_listy2* list,long time)
{
int ktory;
usun_z_listy2(list,time,&ktory);
printf("ktoryyyyyyyyyyyyy %d\n",ktory);
wizyty=first2;
wypisz_liste2(wizyty);
listlekarze[ktory].npacjentow-=1;
/*int i;
for(i=0;i<maxl;i++)
{
    printf("imei %d, npacjentow %d\n", listlekarze[i].imei,listlekarze[i].npacjentow);
}*/
}

int sprawdz_czyurlop(el_listy2 *lista,long czasod, long czasdo,int idlekarz,long *idpacjent)
{
    el_listy2 *wsk=lista;
    int wynik=0;
    while(wsk!=NULL)
    {
       // printf("ggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg\n");
        if((wsk->czas.data>czasod)&&(wsk->czas.data<czasdo) && (wsk->czas.idlekarz==idlekarz))
        {
        (*idpacjent)=wsk->czas.idpacjent;
            wynik=1;
            break;
        }
        wsk=wsk->next;
    }
    return wynik;
}

 
 void urlop(el_listy2* list,long czasod,long czasdo,int idlekarza)
{
    //printf("dssdfdfdbfxfvdbhshbdgfbsfbzcbxbxbvxbrbfxbtfgdbbtdrgdfghthfdxbxfbdthtdrhfbfhtyh\n");
    long idpacjenta;
    int imei,ktory,i;
   int czyok;
   czyok=sprawdz_czyurlop(list,czasod,czasdo,idlekarza,&idpacjenta);
  // printf("czy ok %d id pacjenta %d\n",czyok,idpacjenta);
   if(czyok==1){
  
    usun_z_listy22(list,idpacjenta,&imei);
   for(i=0;i<maxl;i++)
{
    if(listlekarze[i].imei==imei)
    {
        ktory=i;
        break;
    }
}
    dodajWizyte(listlekarze,czasdo,maxl,ktory,idpacjenta);
   wizyty=first2;
  // urlop(wizyty,czasod,czasdo,idlekarza);
   }
   
     // dodajWizyte(listlekarze,listlekarze[idlekarza].urlop_do,maxl,idlekarza,idpacjent);
    wypisz_liste2(wizyty); 
}
 void zaladujlekarzy()
{
    wykonaj();
    int i;
    printf("printf %d\n",maxl);
     my_msg2.maxlek=maxl;
    for(i=0;i<maxl;i++)
    {
        listlekarze[i]=lekarze[i];
    }     
}
 
 void dodaj_do_listy(el_listy* lista,struct msgbuf pacj)
 {  
     if(first==NULL)
     {
    first=(el_listy*) malloc (sizeof(el_listy));
    strcpy(first->pacjent.imie,pacj.imie);
    first->pacjent.id=pacj.id;
    first->pacjent.czyzalogowany=0;
    first->pacjent.ilerazy=0;
   strcpy(first->pacjent.nazwisko,pacj.nazwisko);
   
   first->pacjent.pesel=pacj.pesel;
   strcpy(first->pacjent.haslo,pacj.haslo);
   first->next = NULL;
     }
     else{
     el_listy *wsk, *nowy;
   wsk = lista;
   while (wsk->next != NULL)
     { 
     wsk = wsk->next; /* przesuwamy wsk aż znajdziemy ostatni element */
     }
   nowy =(el_listy*) malloc (sizeof(el_listy));
   nowy->pacjent.id=pacj.id;
   nowy->pacjent.czyzalogowany=0;
    nowy->pacjent.ilerazy=0;
   strcpy(nowy->pacjent.nazwisko,pacj.nazwisko);
   strcpy(nowy->pacjent.imie,pacj.imie);
   nowy->pacjent.pesel = pacj.pesel;
   strcpy(nowy->pacjent.haslo,pacj.haslo);
   nowy->next = NULL;
   wsk->next = nowy; /* podczepiamy nowy element do ostatniego z listy */
 }
 }

 long ktorydzien(el_listy2 *lista,long idpacjenta)
 {
     el_listy2 *wsk=lista;
     long ktory=0;
     while(wsk!=NULL)
     {
         if(wsk->czas.idpacjent==idpacjenta)
         {
             ktory=wsk->czas.data;
             break;
         }
         wsk=wsk->next;
     }
     return ktory;
 }
 int ktorylekarz(el_listy2 *lista,long idpacjenta)
 {
     el_listy2 *wsk=lista;
     int ktory=0;
     while(wsk!=NULL)
     {
         if(wsk->czas.idpacjent==idpacjenta)
         {
             ktory=wsk->czas.idlekarz;
             break;
         }
         wsk=wsk->next;
     }
     return ktory;
 }
 
 void wypisz_liste(el_listy *lista)
 {
     int i=0;
   el_listy *wsk=lista;
   while( wsk != NULL )
     {
     printf ("%d %s %s %lu %s czy zalogowany %d\n",wsk->pacjent.id,wsk->pacjent.imie,wsk->pacjent.nazwisko,wsk->pacjent.pesel,wsk->pacjent.haslo, wsk->pacjent.czyzalogowany);
   i++;
     wsk = wsk->next;
     }
      printf ("\n");
 }
 
int sprawdz(el_listy *lista,struct msgbuf usery)
{
    el_listy *wsk=lista;
    while(wsk!=NULL)
    {
        if(wsk->pacjent.id==usery.id)
        {
            return 1;
            break;
        }
        if(wsk->pacjent.pesel==usery.pesel)
        {
            return 2;
            break;
        }
        wsk=wsk->next;
    }
    return 0;
}

int zaloguj(el_listy *lista,struct msgbuf usery)
{
    el_listy *wsk=lista;
    int a=0;
    while(wsk!=NULL)
    {
      if(wsk->pacjent.pesel==usery.pesel)
      {
        a=1;
        break;
      }
          
      wsk=wsk->next;
    }
    
    if(wsk==NULL)
    {
        a=0;
    }
    
    if(a==1)
    {
            if((strcmp(wsk->pacjent.haslo,usery.haslo)==0)&&(wsk->pacjent.czyzalogowany==0)&&(wsk->pacjent.ilerazy<5))
      {
          wsk->pacjent.czyzalogowany=1;
          wsk->pacjent.ilerazy=0;
          return 3;
      }else if((strcmp(wsk->pacjent.haslo,usery.haslo)!=0)&&(wsk->pacjent.czyzalogowany==0)&&(wsk->pacjent.ilerazy>=5))
      {
          return 7;
      }else if((strcmp(wsk->pacjent.haslo,usery.haslo)==0)&&(wsk->pacjent.czyzalogowany!=0)&&(wsk->pacjent.ilerazy<5))
      {
          return 6;
      }else if((strcmp(wsk->pacjent.haslo,usery.haslo)!=0)&&(wsk->pacjent.czyzalogowany==0)&&(wsk->pacjent.ilerazy<5))
      {
          wsk->pacjent.ilerazy+=1;
          return 4;
      }
        
    }else
    {
        return 5;
    }
    return 0;
}

int wyloguj(el_listy *lista,struct msgbuf usery)
{
    el_listy *wsk=lista;
    int a=0;
    while(wsk!=NULL)
    {
        if(wsk->pacjent.pesel==usery.pesel)
	{
	 a=1;
          break;
	}else
	{
	  a=0;
	}
        wsk=wsk->next;
    }
    if(a==1){
     if((strcmp(wsk->pacjent.haslo,usery.haslo)==0)&& (wsk->pacjent.czyzalogowany==1)&&(strcmp(wsk->pacjent.imie,usery.imie)==0)&&(strcmp(wsk->pacjent.nazwisko,usery.nazwisko)==0))
	{
	  wsk->pacjent.czyzalogowany=0;
	  return 15;
	}else
	{
	   return 5;
	}
    }
    else 
        return 0;
    
    return 0;
}

int czyaktywny(el_listy *lista,struct msgbuf usery)
{
    el_listy *wsk=lista;
    int a=0;
    while(wsk!=NULL)
    {
         if(wsk->pacjent.pesel==usery.pesel){
             a=1;
            break;
         }
         else 
             a=0;
    
        wsk=wsk->next;
    }  
    if(a==1)
    {
         if((strcmp(wsk->pacjent.haslo,usery.haslo)==0)&& (wsk->pacjent.czyzalogowany==1)&&(strcmp(wsk->pacjent.imie,usery.imie)==0)&&(strcmp(wsk->pacjent.nazwisko,usery.nazwisko)==0)){
            return 2;
         }
         else 
             return 1;
    }else
    {
        return 1;
    }
    return 0;
}

time_t my_time(time_t* timer)
{
  return start_time + (time(NULL) - start_time) *24 * 60 * 60;
} 
int main(){
    zaladujlekarzy();
    start_time = time(NULL);  //pobieramy rzeczywisty czas uruchomienia serwera
    printf("starded\n");
    int mid=msgget(0x123,0666|IPC_CREAT);
    int rcvresult = 0;
    //int mid2=msgget(0x124,0666|IPC_CREAT);
    printf("got queue id %d\n", mid);
    my_msg2.kto='R';
    start_time = time(NULL);  //pobieramy rzeczywisty czas uruchomienia serwera
    time_t now = time(NULL);
    printf("%u\n", (unsigned)now);
    struct tm* info;
    info = localtime(&now);
    char buffer[80];
    strftime(buffer,80,"%x - %I:%M%p", info);
    printf("current time is %s\n", buffer);
    printf("tm_year: %d\n", 1900 + (*info).tm_year);
    printf("tm_mon: %d\n", (*info).tm_mon+1);
    printf("tm_mday: %d\n", (*info).tm_mday);
    printf("tm_hour: %d\n", (*info).tm_hour);
    printf("tm_min: %d\n", (*info).tm_min);
    printf("tm_sec: %d\n", (*info).tm_sec);
    static const char default_format[] = "%D %Y %R %u";
    const char *format = default_format;
    struct tm bb;
    char res[32];
    time_t new2 = mktime(info);
    printf("new 2 %ld\n",now);
    localtime_r(&now, &bb);
    if (strftime(res, sizeof(res), format, &bb) == 0) {
        fprintf(stderr,  "strftime(3): cannot format supplied "
            "date/time into buffer of size %u "
            "using: '%s'\n",
            (unsigned)sizeof(res), format);
        return 1;
    }
        printf("%lu -> %s\n",new2,res);
    while(1) {
       
        // wypisz(listlekarze,maxl);
    time_t temp;
    temp=my_time(NULL);
    localtime_r(&temp, &bb);
   if (strftime(res, sizeof(res), format, &bb) == 0) {
                (void) fprintf(stderr,  "strftime(3): cannot format supplied "
                                        "date/time into buffer of size %u "
                                        "using: '%s'\n",
                                         (unsigned)sizeof(res), format);
                return 1;
        }

        (void) printf("%u -> '%s'\n", (unsigned)my_time(NULL), res);
        printf("Nasłuchujemy ...\n");
        rcvresult = msgrcv(mid,&my_msg1,sizeof(struct msgbuf) - sizeof(long), 1, IPC_NOWAIT);
        printf("MSGRCV: %d\n", rcvresult);
        printf("Imie: %s, Nazwisko: %s, Pesel: %ld, haslo %s\n",my_msg1.imie,my_msg1.nazwisko,my_msg1.pesel,my_msg1.haslo);
        int spr;

        printf("akcja %d\n",my_msg1.akcja);
        if(rcvresult > 0){
            switch(my_msg1.kto){
                case 'P':{
                    printf("Pacjnet msg\n");
                switch(my_msg1.akcja){
                    case 1:{
                    my_msg1.czy_usunac=0;
                        spr=sprawdz(first,my_msg1);
                            if(spr==1){
                                my_msg1.komunikat=2;
                            }else if(spr==2){
                                my_msg1.komunikat=2;
                            }else{
                                my_msg1.komunikat=1;
                                dodaj_do_listy(first,my_msg1);
                            }
                            
                        wypisz_liste(first);
                        int czy=-1;
                    dodajWizyte(listlekarze,new2,maxl,czy,my_msg1.id);
                        my_msg1.akcja=0;
                    
                        my_msg2.mtype = my_msg1.id;
                        my_msg2.komunikat=my_msg1.komunikat;
                        my_msg2.kiedy=my_msg1.kiedy;
                        printf("kom %d\n",my_msg2.komunikat);
                        msgsnd(mid, &my_msg2,sizeof(struct msgbuf) - sizeof(long), 0);
                        break;
                    }
                    case 2:
                    {
                        wypisz_liste(first);
                        my_msg2.komunikat=zaloguj(first,my_msg1);
                        my_msg2.mtype = my_msg1.id;
                        printf("kom %d\n",my_msg2.komunikat);
                        msgsnd(mid, &my_msg2,sizeof(my_msg2) - sizeof(long), 0);
                        break;
                        }
                    case 3 :{
                        int spr2=czyaktywny(first,my_msg1);
                        if(spr2==2){
                            my_msg2.komunikat=8;
                        }else if(spr2==1){
                            my_msg2.komunikat=5;
                        }
                        if(my_msg2.komunikat==8){
                            int temp;
                        for(temp=0;temp<my_msg2.maxlek;temp++)
                                my_msg2.lekarze2[temp]=listlekarze[temp];
                            // wypisz(my_msg2.lekarze2,my_msg2.maxlek);
                        printf("maxlllllllllllllll %d\n",maxl);
                    }
                        my_msg2.mtype = my_msg1.id;
                        printf("kom %d\n",my_msg2.komunikat);
                        msgsnd(mid, &my_msg2,sizeof(my_msg2) - sizeof(long), 0);
                        break;
                    }
                    case 6:{
                        int imeilek;
                            long idwiz;
                        int spr2=czyaktywny(first,my_msg1);
                        if(spr2==2){
                            my_msg2.komunikat=12;
                        }else if(spr2==1){
                            my_msg2.komunikat=5;
                        }
                        if(my_msg2.komunikat==12){
                            
                    znajdz_termin(wizyty,my_msg1.id,&imeilek,&idwiz);
                    }
                        my_msg2.mtype = my_msg1.id;
                    // printf("kom %d\n",my_msg2.komunikat);
                        my_msg2.kiedy=my_msg1.kiedy;
                        // wypisz_liste(first);
                    // printf("imei lekarza %d\n",imeilek);
                        int i;
                        for(i=0;i<maxl;i++)
                        {
                            if(listlekarze[i].imei==imeilek){
                                strcpy(my_msg1.imlek,listlekarze[i].im);
                                strcpy(my_msg1.nazwlek,listlekarze[i].nazw);
                                break;
                            }
                        }
                        strcpy(my_msg2.imlek,my_msg1.imlek);
                        strcpy(my_msg2.nazwlek,my_msg1.nazwlek);
                        my_msg2.idwizyty=my_msg1.idwizyty=idwiz;
                                
                        msgsnd(mid, &my_msg2,sizeof(my_msg2) - sizeof(long), 0);
                        break;
                    }
                    case 7:{
                        int spr2=czyaktywny(first,my_msg1);
                        if(spr2==2){
                            my_msg2.komunikat=10;
                        }else if(spr2==1){
                            my_msg2.komunikat=5;
                        }
                        if(my_msg2.komunikat==10){
                        zmienWizyte(wizyty,my_msg1.id,now);
                    }
                        my_msg2.mtype = my_msg1.id;
                        printf("kom %d\n",my_msg2.komunikat);
                        my_msg2.kiedy=my_msg1.kiedy;
                        msgsnd(mid, &my_msg2,sizeof(my_msg2) - sizeof(long), 0);
                        break;
                    }
                    case 8:{
                        int spr2=czyaktywny(first,my_msg1);
                        if(spr2==2){
                            my_msg2.komunikat=11;
                        }else if(spr2==1){
                            my_msg2.komunikat=5;
                        }
                        if(my_msg2.komunikat==11){
                            printf("my msg 1 %ld",my_msg1.id);
                    odwolajWizyte(wizyty,my_msg1.id);
                    }
                        my_msg2.mtype = my_msg1.id;
                        my_msg1.czy_usunac=1;
                        printf("kom %d\n",my_msg2.komunikat);
                        msgsnd(mid, &my_msg2,sizeof(my_msg2) - sizeof(long), 0);
                        printf("wizyty\n");
                        wypisz_liste2(wizyty);
                        printf("lista pacjentów\n");
                        wypisz_liste(first);
                        break;
                    }
                    case 9:{ 
                        my_msg2.komunikat=wyloguj(first,my_msg1);
                        my_msg2.mtype = my_msg1.id;
                       // printf("kom %d\n",my_msg1.komunikat);
                        msgsnd(mid, &my_msg2,sizeof(my_msg2) - sizeof(long), 0);
                        break; 
                    }
                     case 5:{
                            int spr2=czyaktywny(first,my_msg1);
                            if(spr2==2){
                                my_msg2.komunikat=13;
                            }else if(spr2==1){
                                my_msg2.komunikat=5;
                            }
                            int pomoc;
                            long tabdni[30];
                            int maxdni=0;
                            int ktory=ktorylekarz(wizyty,my_msg1.id)-1;
                            if(my_msg2.komunikat==13){
                            int d;
                                time_t new3;
                            for(d = 0; d < 31; d++)
                            {
                                int i;
                            
                                pomoc=(60+d)*24*3600;
                                new3=new2+pomoc;
                                struct tm* dzienTemp = localtime(&new3);
                                for(i=0;i<listlekarze[ktory].max;i++){
                                
                                            if((*dzienTemp).tm_wday==listlekarze[ktory].dzien[i]){
                                                        tabdni[maxdni++]=new3;
                                    }
                                }
                            }
                            }
                            my_msg2.maxdni= my_msg1.maxdni=maxdni;
                        
                            int k;
                            for(k=0;k<maxdni;k++)
                                my_msg2.tabdni[k]= my_msg1.tabdni[k]=tabdni[k];
                        
                            strcpy(my_msg2.imlek,listlekarze[ktory].im);
                            strcpy(my_msg2.nazwlek,listlekarze[ktory].nazw);
                    
                            my_msg2.mtype = my_msg1.id;
                            msgsnd(mid, &my_msg2,sizeof(my_msg2) - sizeof(long), 0);
                            break; 
                        }
                     case 4:{
                           int spr2=czyaktywny(first,my_msg1);
                            if(spr2==2){
                                my_msg2.komunikat=14;
                            }else if(spr2==1){
                                my_msg2.komunikat=5;
                            }
                            if(my_msg2.komunikat==14){
                                int leklist[10];
                                int tmp=0;
                                long data=ktorydzien(wizyty,my_msg1.id);
                                printf("dzien %ld\n",data);
                                time_t konwertowanie=data;
                                struct tm* dzienTemp = localtime(&konwertowanie);
                                int i,j;
                                int temp;
                                for(temp=0;temp<my_msg2.maxlek;temp++)
                                        my_msg2.lekarze2[temp]=listlekarze[temp];
                                for(i=0;i<maxl;i++)
                                {
                                    for(j=0;j<listlekarze[i].max;j++){
                                        
                                                    if((*dzienTemp).tm_wday==listlekarze[i].dzien[j]){
                                                    printf("%d , %d , %d\n",(*dzienTemp).tm_wday,listlekarze[i].dzien[j],i);
                                                                leklist[tmp++]=i;
                                            }
                                        }
                                }
                                
                                my_msg1.maxlistlek=my_msg2.maxlistlek=tmp;
                                int t;
                                for(t=0;t<tmp;t++)
                                {
                                    my_msg1.leklist[t]=my_msg2.leklist[t]=leklist[t];
                                printf("id lekarzy %d\n",leklist[t]);
                                }
                                my_msg2.kiedy=data;
                        }
                          my_msg2.mtype = my_msg1.id;
                        msgsnd(mid, &my_msg2,sizeof(my_msg2) - sizeof(long), 0);
                        break; 
                     }
                     
                    // default:printf("error\n");
                }
            break;
            }
            case 'L':{
                printf("Lekarz msg\n");
                switch(my_msg1.akcja){
                    case 1:{
                        int i;
                        int tym=0,wybr;
                        printf("msg imei %d\n",my_msg1.imei);
                        for(i=0;i<maxl;i++)
                        {
                            if((strcmp(my_msg1.nazwisko,listlekarze[i].nazw)==0)&&(listlekarze[i].imei==my_msg1.imei)/*)&& (listlekarze[i].czy_zalogowany==0)*/)
                            {
                                printf("imei2 %d\n",listlekarze[i].imei);
                                tym=1;
                                wybr=i;
                                i=maxl;
                            }
                        } 
                        if(tym==1)
                         {
                                listlekarze[wybr].czy_zalogowany=1;
                                my_msg2.komunikat=1;
                        } else
                            my_msg2.komunikat=2;
                
                        my_msg2.mtype = my_msg1.id;
                        msgsnd(mid,&my_msg2,sizeof(msgbuf) - sizeof(long), 0);
                          for(i=0;i<maxl;i++)
                        {
                            printf("czy zalogowany  %d %d\n",i,listlekarze[i].czy_zalogowany);
                            }
                        break;
                        }
                    case 2:{
                        printf("jest\n");
                    int i,tymczasowa=0,imeilek;
                    for(i=0;i<maxl;i++)
                    {
                 /*        printf("i %d nazw %d msg nazw %d \n",i,listlekarze[i].imei,my_msg1.imei);*/
                            if((my_msg1.imei==listlekarze[i].imei)&& (listlekarze[i].czy_zalogowany==1))
                            {
                                printf("i %d nazw %d msg nazw %d \n",i,listlekarze[i].imei,my_msg1.imei);
                                imeilek=listlekarze[i].imei;
                                tymczasowa=1;
                                
                                break;
                            }else tymczasowa=0;
                    }
                    if(tymczasowa==1){
                        printf("imei %d czas od %lu czas do %lu\n",imeilek,my_msg1.urlop_od,my_msg1.urlop_do);
                            urlop(wizyty,my_msg1.urlop_od,my_msg1.urlop_do,imeilek);//???
                            my_msg2.komunikat=7;
                    }else
                    {
                        my_msg2.komunikat=3;
                    }
                    my_msg2.mtype = my_msg1.id;
                        msgsnd(mid,&my_msg2,sizeof(msgbuf) - sizeof(long), 0);
                        break;
                    }
                    case 3:{
                        int i,pom=0,kt;
                        for(i=0;i<maxl;i++)
                        {
                            if(listlekarze[i].imei==my_msg1.imei)
                            {
                                pom=1;
                                kt=i;
                                i=maxl;
                            }
                        }
                        if(pom==1)
                        {
                            my_msg2.komunikat=15;
                            listlekarze[kt].czy_zalogowany=0;
                        }else
                            my_msg2.komunikat=3;
                         my_msg2.mtype = my_msg1.id;
                        printf("kom %d\n",my_msg1.komunikat);
                        msgsnd(mid, &my_msg2,sizeof(my_msg2) - sizeof(long), 0);
                        break;
                    }
                    }
                    break;
                }
                    default:{
                        printf("Nie wiadomo, od kogo wiadomść\n");
                    }
            }
        }
        else{
        sleep(1);
        }
        // tutaj sprawdzamy, czy któras wizyta jest za 2 tygodnie i wysyłamy powiadomienie
        
      wizyta = wizyty;

        //printf("%ld SECONDS_TO_REMIND\n",SECONDS_TO_REMIND);
        while(wizyta != NULL){
        
            if(!wizyta->czas.reminder_sent && (wizyta->czas.data-(14*24*3600)<=my_time(0))) /*my_time(0) - wizyta->czas.data <= SECONDS_TO_REMIND)*/{
                printf("Za 14 dni wizyta %ld pacjenta %ld\n", wizyta->czas.data, wizyta->czas.idpacjent);
                msg_remind.mtype = wizyta->czas.idpacjent | REMINDER_FLAG;
                msg_remind.idwizyty = wizyta->czas.data;
                int i;
                for(i=0;i<maxl;i++)
                {
                    if(listlekarze[i].imei==wizyta->czas.idlekarz){
                        strcpy(msg_remind.imlek,listlekarze[i].im);
                        strcpy(msg_remind.nazwlek,listlekarze[i].nazw);
                        break;
                    }
                }
                
                msgsnd(mid, &msg_remind,sizeof(struct msgbuf) - sizeof(long), 0);
                wizyta->czas.reminder_sent = 1;
            }
            printf("Nast. wizyta\n");
            wizyta = wizyta->next;
        }
    }
    return 0;
}

