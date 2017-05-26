/* FRUNZA Roxana - 311CB */
/* LISTE GENERICE DUBLU INLANTUITE CIRCULARE */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#ifndef _LISTA_DUBLU_INLANTUITA_
#define _LISTA_DUBLU_INLANTUITA_

typedef struct celula2
{ struct celula2 *pre, *urm;  /* legaturi spre celulele vecine */
  void *info;              /* adresa element extern */
} TCelula2, *TLDI, **ALDI;

typedef int (*TFCmp)(void*, void*); /* functie de comparare doua elemente */
typedef void (*TF)(void*);     /* functie prelucrare un element */
typedef void (*TFAfi)(void*,FILE*); /*functie afisare element */
/*Inserare element dupa celula cu adresa *aL; 0/1 -> inserare nereusita/reusita*/
int InsLDI(TLDI *aL, void * x, size_t d);
/*Inserare element la inceputul listei *aL; 0/1 -> inserare nereusita/reusita*/
int InsInc(TLDI *aL, void * x, size_t d);
/*Cauta element in lista; 1/0 -> elementul exista sau nu */
int ExistaEl(TLDI L,void *element, TFCmp cmp);
/* elimina celula si element; 0/1 -> eliminare nereusita/reusita*/
void ElimLDI(TLDI *aL, void (*freeEl)(void *));       
/* elibereaza tot spatiul ocupat de lista */
void DistrLDI(TLDI * aL,void (*freeEl)(void *)); 
void AfisareLDI(TLDI L, TFAfi AfiEl,FILE *f);  /* afisare lista */
int LungimeLDI(TLDI L); /*returneaza numarul de elemente din lista*/
#endif