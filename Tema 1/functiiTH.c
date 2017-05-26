/* FRUNZA Roxana - 311CB */
/* operatii pentru tabela hash */

#include "LDIG.h"
#include "TH.h"

/*Initializeaza tabelul hash. Intoarce tabelul sau NULL*/
TH* InitTH(int M, TFHash fh)
{
	TH *h=(TH*)calloc(sizeof(TH),1);
	if (!h) 
	{	
		printf("eroare alocare hash\n");
		return NULL;
	}

	h->v=(TLDI*)calloc(M, sizeof(TLDI));
	if(!h->v)
	{
		printf("eroare alocare vector de pointeri TLDI in hash\n");
		free(h);
		return NULL;
	}
	h->M=M;
	h->fh=fh;
	return h;
}
/*elibereaza memoria ocupata de tabela hash*/
void DistrTH(TH **h, TF fe)
{
	int i;	
	TH *aux = *h;
	/*Pentru fiecare lista nevida din vector, eliberam memoria*/
    for (i = 0; i < aux->M; i++)
    {
    	TLDI lista = aux->v[i];
    	if(lista != NULL)
	  		DistrLDI(&lista,fe);
  	}
  	free(aux->v); /*eliberam vectorul de liste*/
  	free(aux); /*eliberam tabela hash*/
  	aux = NULL;
}
/*inserareaza ordonat un element de dimensiune d*/
int InsTHO(TH*a,void*ae, size_t d, TFCmp f)
{
	int cod=a->fh(ae,a->M), rez; 
	TLDI current, head = a->v[cod];
    ALDI u;

	/*Inserare lista vida */
    if(head == NULL)
    {
       	rez = InsInc(&a->v[cod],ae,d);
       	
    }
    /*Inserare in lista cu un element*/
    else if (head == head->urm)
    {
    	if( f(head->info, ae) <= 0) 
    		rez = InsLDI(&a->v[cod],ae,d);
    	else if( f(head->info, ae) >= 0)
    		rez = InsInc(&(a->v[cod]),ae,d);
    	else 
    		return 0;
    }
    /*Inserare in lista cu mai multe elemente*/
    else
    {
    	
    	/*verificam daca informatia exista deja */
    	for(current = head; current->urm != a->v[cod]; current=current->urm)
		{	
			if( !f(current->info, ae))
			return 0;	
		}	
		/*inserare la inceput*/
		if(f(ae, head->info) <= 0)
		{
			rez = InsInc( &(a->v[cod]), ae, d);
		}
		/*inserare dupa primul element*/
		else if(f(ae, head->info) >= 0 && f(ae, head->urm->info) <= 0)
		{
			rez = InsLDI(&a->v[cod],ae,d);
		}
		/*inserare in interiorul listei*/
		else
		{		
			u = &head;
			/*cautam adresa unde trebuie inserat*/
			while((*u)->urm != head)
			{
				if( f( (*u)->info, ae ) > 0)
         			break;
        		u = &(*u) -> urm;
			}
			/*inseram elementul la adresa gasita*/
			rez = InsLDI(u, ae, d);
		}
    }
	return rez;	
}
/*calculeaza numarul de elemente din tabela hash*/
int ElemTH(TH *a)
{
	int nrel= a->M,i=0,suma=0;
	/*pentru fiecare lista, calculam lungimea*/
	for(; i < nrel; i++)
	{
		suma += LungimeLDI(a->v[i]); /*adunam la rezultatul final*/
	}

	return suma;	
}
/*afiseaza tabela hash*/
void AfiTH(TH*ah, TFAfi afiEl,FILE *f)
{
	TLDI*p, el;
	/*pentru fiecare lista nevida, afisam elementele */
	for(p=ah->v; p<ah->v+ah->M;p++)
	{
		if(*p)
		{
			fprintf(f,"%d: ",p-ah->v);
			for(el=*p;el->urm!=*p;el=el->urm)
				afiEl(el->info,f);
			afiEl(el->info,f);
			printf("\n");
		}
	}

}