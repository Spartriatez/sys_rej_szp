#include <stdio.h>
 #include <stdlib.h>
#include<string.h>
#include<time.h>

struct czasy
{
    long idpacjent;
    int idlekarz;
    long data;
    short reminder_sent;
}czas;

typedef struct element2 {
    struct czasy czas;
   struct element2 *next;
} el_listy2;

 el_listy2 *first2=NULL;
 
 void dodaj_do_listy2(el_listy2* lista,struct czasy czas2)
 {  
     if(lista==NULL)
     {
    first2=(el_listy2*) malloc (sizeof(el_listy2));
    first2->czas.data=czas2.data;
     first2->czas.idpacjent=czas2.idpacjent;
      first2->czas.idlekarz=czas2.idlekarz;
   first2->next = NULL;
     }
     else{
     el_listy2 *wsk, *nowy;
   wsk = lista;
   while (wsk->next != NULL)
     { 
     wsk = wsk->next; /* przesuwamy wsk aż znajdziemy ostatni element */
}
   nowy =(el_listy2*) malloc (sizeof(el_listy2));
   nowy->czas.data=czas2.data;
   nowy->czas.idpacjent=czas2.idpacjent;
  nowy->czas.idlekarz=czas2.idlekarz;
   nowy->next = NULL;
   wsk->next = nowy; /* podczepiamy nowy element do ostatniego z listy */
}
 }

 void usun_z_listy2(el_listy2 *lista,long element,int *ktory)
 {
    if(first2->czas.data==element)
    {
        el_listy2 *tmp=first2;
        first2=tmp->next;
        free(tmp);
    }
    else 
     {
   el_listy2 *wsk=lista;
   while (wsk->next != NULL)
     {
     if (wsk->next->czas.data == element) 
       {
       el_listy2 *usuwany=wsk->next;
        (*ktory)=usuwany->czas.idlekarz;
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


 void usun_z_listy22(el_listy2 *lista,long pacjent,int *ktory)
 {
    if(first2->czas.idpacjent==pacjent)
    {
        el_listy2 *tmp=first2;
        first2=tmp->next;
        free(tmp);
    }
    else 
     {
   el_listy2 *wsk=lista;
   while (wsk->next != NULL)
     {
     if (wsk->next->czas.idpacjent == pacjent) 
       {
       el_listy2 *usuwany=wsk->next;
        (*ktory)=usuwany->czas.idlekarz;
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

int sprawdz2(el_listy2 *lista,struct czasy czas2)
{
    el_listy2 *wsk=lista;
    while(wsk!=NULL)
    {
        if(wsk->czas.data==czas2.data && wsk->czas.idlekarz==czas2.idlekarz)
        {
            return 1;
            break;
        }
        wsk=wsk->next;
    }
    return 0;
}

void znajdz_termin(el_listy2 *lista, long idpacjenta,int* imeilekarza,long* idwizyty)
{
    el_listy2* wsk=lista;
    printf("id pacjenta %ld\n",idpacjenta);
    while(wsk!=NULL)
    {
        if(wsk->czas.idpacjent==idpacjenta)
        {
            (*imeilekarza)=wsk->czas.idlekarz;
            // printf("id pacjenta %d\n",wsk->czas.idlekarz);
            (*idwizyty)=wsk->czas.data;
           // printf("id pacjenta %ld\n",wsk->czas.data);
            break;
        }
            wsk=wsk->next;
    }
}
 void wypisz_liste2(el_listy2 *lista)
 {
     int i=0;
   el_listy2 *wsk=lista;
   while( wsk != NULL )
     {
     printf ("%ld %ld %d\n",wsk->czas.data,wsk->czas.idpacjent,wsk->czas.idlekarz);
   i++;
     wsk = wsk->next;
     }
      printf ("\n");
 }

