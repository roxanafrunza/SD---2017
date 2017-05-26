/* FRUNZA Roxana - 311CB */
/*operatii de baza pentru liste generice dublu inlantuite circulare*/
#include "LDIG.h"
/*inserare dupa celula cu adresa *aL*/
int InsLDI(TLDI *aL, void * x, size_t d)
{
	TLDI aux = malloc(sizeof(TCelula2)); /*alocare celula*/
	if(!aux) return 0; /*alocare nereusita, iesire din functie*/
	aux->info = malloc(d); /*alocare spatiu informatie*/
	if(!aux->info) /*alocare nereusita*/
	{
		free(aux); /*eliberare celula*/
		return 0; /*iesire din functie*/
	}
	memcpy(aux->info, x, d); /*copiere informatie in celula*/
    /*Inserare in lista vida*/
	if( *aL == NULL)
	{
		*aL = aux;
		(*aL)->pre = aux;
		(*aL)->urm = aux;
	} 
	/*Inserare dupa celula cu adresa *aL*/
	else
	{
		aux->urm = (*aL)-> urm;
		(*aL)->urm = aux;
		aux->pre = (*aL);
		aux->urm->pre = aux;
	}
	
	return 1;
}
/*inserare element la inceput de lista*/
int InsInc(TLDI *aL, void * x, size_t d)
{
	TLDI aux = malloc(sizeof(TCelula2));
	if(!aux) return 0;
	aux->info = malloc(d);
	if(!aux->info)
	{
		free(aux);
		return 0;
	}
	memcpy(aux->info, x, d);
    /*Inserare in lista vida*/
	if( *aL == NULL)
	{
		*aL = aux;
		(*aL)->pre = aux;
		(*aL)->urm = aux;
	} 
	/*Inserare in lista cu mai multe elemente*/
	else
	{
		aux->urm = *aL;
		aux->pre = (*aL)->pre;
		aux->urm->pre = aux;
		aux->pre->urm = aux;
		(*aL) = aux;
	}
	
	return 1;
}
/*cautare element lista cu ajutorul unei functii tip TFCMP*/
int ExistaEl(TLDI L,void *element, TFCmp cmp)
{
	TLDI a = L;
	if(L == NULL)
		return 0;
	else
	{
		while( a->urm != L)
		{
			if(cmp(a->info,element) == 0)
				return 1;
			a = a->urm;
		}
		if(cmp(a->info,element) == 0)
			return 1;
	}
	return 0;
}
/*Eliminare element de la adresa *aL din lista */
void ElimLDI(TLDI *aL, TF FreeEl)
{
	TLDI aux = *aL;
	if(aux == NULL) return ;
	aux = *aL;
	*aL = aux->urm;
	FreeEl(aux->info);
	aux->pre->urm = aux->urm;
	aux->urm->pre = aux->pre;
	free(aux);
}
/*Elimina toate elementele*/
void DistrLDI(TLDI *aL, TF FreeEl)
{
	TLDI a = (*aL)->urm,aux;
	while(a != *aL)
	{
		aux = a;
		a = a->urm;
		ElimLDI(&aux,FreeEl);
	}	
	ElimLDI(aL,FreeEl);
	*aL =NULL;
}
/*Afisare lista cu ajutorul unei functii de tip TFAfi*/
void AfisareLDI(TLDI L, TFAfi AfiEl, FILE *f)
{
	TLDI a = L;
	if(a == NULL)
		printf("VIDA\n");
	else
	{
		while( a->urm != L)
		{
			AfiEl(a->info,f);
			a = a->urm;
		}
		AfiEl(a->info,f);
	}
}
/*Parcurge lista; intoarce lungimea ei*/
int LungimeLDI(TLDI L)
{
	int lg = 0;
	TLDI a = L;
	if(a == NULL)
		return 0;
	else
	{
		while( a->urm != L)
		{
			lg++;
			a = a->urm;
		}
		lg++;
	}
	return lg;
}
