/* FRUNZA Roxana - 311CB */
/* TABELA HASH */

#include "LDIG.h"
/*pointer la functia hash*/
typedef int (*TFHash)(void*,int);
/*declarare structura tabela hash*/
typedef struct 
{
	size_t M; /*dimensiune tabela hash*/
	TFHash fh; /*functia hash*/
	TLDI *v; /*vector de liste */
} TH;

TH* InitTH(int M, TFHash fh); /* initializare tabela hash */
void DistrTH(TH**aa, TF fe); /* eliberare spatiu tabela hash */
/* inserare ordonata in tabela hash; 1/0 -> inserare reusita sau nu*/
int InsTHO(TH*a, void*ae, size_t d, TFCmp f); 
int ElemTH(TH *a); /* intoarce numarul de elemente din tabela hash */
void AfiTH(TH*a, TFAfi afiEl,FILE *f); /* afisare tabela hash */
