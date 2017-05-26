/* FRUNZA Roxana - 311CB */
#include "LDIG.h"
#include "TH.h"

typedef struct 
{
	char *key;
	char *value;
	int frequency;
} TKey;
/*Afisare element; functie folosita de AfiLDI*/
void AfiEl(void *elem, FILE *f)
{
	TKey *key = (TKey*)elem;
	fprintf(f," (%s)",key->value);
}
/*Afisare element; folosita de functia get*/
void AfiElGet(void *elem, FILE *f)
{
	TKey *key = (TKey*)elem;
	fprintf(f,"%s\n",key->value);
}
/*Eliberare spatiu element*/
void FreeEL(void *elem)
{
	TKey *element = (TKey*) elem;
	free(element->value);
	free(element->key);
	free(element);
}
/*Functie de comparare a 2 elemente */
/*descrescator dupa frecventa; in caz de egalitate, alfabetic*/
int cmp(void *a, void *b)
{
	TKey key1 = *(TKey *) a;
	TKey key2 = *(TKey *) b;
	if(key1.frequency == key2.frequency)
		return strcmp(key1.key, key2.key);
	else 
		return key2.frequency - key1.frequency;
		
}
/*compara campul value a doua elemente */
int cmpname(void *a, void *b)
{
	TKey key1 = *(TKey *) a;
	TKey key2 = *(TKey *) b;
		return strcmp(key1.key, key2.key);
}
/*Functia Hash*/
int hash_function(void *ae, int M)
{
	char *element = ((TKey*)ae)->key;
	int i, sum = 0,result;
	for ( i = 0; i < strlen(element); i++)
    	sum = sum + element[i];
    result = sum  % M;
	return result; 
}

/*Functia set. 1/0 - reusita sau nu */
int set(char *key, char *value, TH *hash)
{
	int M = hash->M;
	int oldM = M;
	TKey *element = calloc(1,sizeof(TKey)); /*alocare structura*/
	if(!element) /*alocare nereusita, iesire din functie*/
		return 0;
	/*alocare campuri structura*/
	element->key = strdup(key);
	if(!element->key)
	{
		free(element);
		return 0;
	}
	element->value = strdup(value);
	if(!element->value)
	{
		free(element->key);
		free(element);
		return 0;
	}
	/*calculare lista in care trebuie inserat*/
	int hashcode = hash->fh(element,M);
	ALDI aL = &hash->v[hashcode];
	TLDI L = *aL;
	int rez;
	int lgl = LungimeLDI(*aL); /*lungime lista*/
	int elt = ElemTH(hash); /*numar elemente din tabela hash*/

	if(ExistaEl(L,element,cmpname) == 1) /*daca elementul exista*/
		return 0; /*iesiredin functie*/
	/*daca sunt mai putin de 2*M elemente*/
	if(elt <= 2*M)
	{
		/*daca nu sunt M elemente in lista*/	
		if(lgl < M) 
			rez = InsTHO(hash,element,sizeof(TKey),cmp); /*inseram in tabela*/
		/*altfel, eliminam ultimul element si apoi inseram*/
		else
		{
			ElimLDI(&((*aL)->pre),FreeEL);
			rez = InsTHO(hash,element,sizeof(TKey),cmp);
		}
	}
	/*daca sunt mai mult de 2*M elemente*/
	else
	{
		/*salvam vectorul de liste intr-un auxiliar*/
		TLDI *p = calloc(M,sizeof(TLDI));
		if(!p)
			return 0;
		ALDI  aux;
		TLDI L;
		p = hash->v;
		/*alocam vectorului din tabela spatiu pentru 2*M liste*/
		hash->v = calloc(2*M, sizeof(TLDI));
		if(!hash->v)
		{
			hash->v = p;
			free(p);
			return 0;
		}
		hash->M = 2*M;
		int code,lungime;
		int i;
		/*pentru fiecare lista, inseram elemente care existau deja*/
		for(i = 0; i < oldM; i++)
		{
			L = p[i];
			if(p[i] != NULL)
			{
				/*luam fiecare element si il inseram in tabela*/
				for(;L->urm != p[i]; L = L->urm)
			{

				code = hash->fh(L->info, M); /*calcularea noului hashcode*/
				aux = &hash->v[code];
				lungime = LungimeLDI(*aux);
				if(lungime < M)
					rez = InsTHO(hash,(L)->info,sizeof(TKey),cmp);
				else
				{
					ElimLDI(&((*aux)->pre),FreeEL);
					rez = InsTHO(hash,(L)->info,sizeof(TKey),cmp);
				}
			}
			/*verificam daca lista in care inseram nu sunt M elemente*/
			if(lungime < M)
					rez = InsTHO(hash,p[i]->pre->info,sizeof(TKey),cmp);
			else
				{
					ElimLDI(&((*aux)->pre),FreeEL); 
					rez = InsTHO(hash,p[i]->pre->info,sizeof(TKey),cmp);
				}
			}
			
			
		}
		/*dupa realocare, inseram noul element*/
		rez = InsTHO(hash,element,sizeof(TKey),cmp);

	}
	return rez;
}
/*Functia get. Intoarce pointer 1/0 - element gasit sau nu */
void *get(char *key, TH *hash)
{
	int M = hash->M;
	TKey *element = calloc(1,sizeof(TKey));
	if(!element)
		return 0;
	/*cauta lista din tabela hash*/
	element->key = strdup(key);
	int hashcode = hash->fh(element,M);
	ALDI aL = &hash->v[hashcode], u = aL;
	TLDI a = *aL;
	/*cauta elementul in lista*/
	if(ExistaEl(*aL,element,cmpname) != 1)
		return NULL;		
	while( a->urm != *aL)
		{
			if(cmpname(a->info,element) == 0)
			{
				break;
			}
			a = a->urm;
		}	
	TKey *element2;
	element2 = calloc(1,sizeof(TKey)); /*alocare strucura*/
	if(!element2) /*alocare esuata, iesire din functie*/
		return NULL;
	/*copiere informatie*/
	element2->key= strdup(((TKey*)(a->info))->key);
	if(!element2->key)
	{
		free(element2);
		return NULL;
	}
	element2->value= strdup(((TKey*)(a->info))->value);
	if(!element2->value)
	{
		free(element2->key);
		free(element2);
		return NULL;
	}
	/*incrementare frecventa*/
	element2->frequency = ((TKey*)(a->info))->frequency + 1;
	/*inserarea elementului cu noua frecventa*/
	InsTHO(hash,element2,sizeof(TKey),cmp);
	/*eliminare element cu frecventa initiala*/
	/*eliminare daca este primul element*/
	if( cmp( ((*aL)->pre)->info, element2 ) > 0 && cmpname( ((*aL)->pre)->info, element2 ) == 0 )
       	ElimLDI(&((*aL)->pre),FreeEL);
    /*eliminare in interiorul listei*/
	while((*u)->urm != hash->v[hashcode])
		{
			if( cmp( (*u)->info, element2 ) > 0 && cmpname( (*u)->info, element2 ) == 0 )
				ElimLDI(u,FreeEL);
        	u = &(*u) -> urm;
		}
	free(element->key);
	free(element);
	return element2;

}

/*Functia remove*/
void delete(char *key, TH *hash)
{
	int M = hash->M;
	TKey *element = calloc(1,sizeof(TKey)); /*alocare structura*/
	if(!element)
		return ; /*alocare nereusita*/
	element->key = strdup(key);
	if(!element->key)
	{
		free(element);
		return;
	}
	/*calculare lista din care trebuie sters*/
	int hashcode = hash->fh(element,M);
	int lg = LungimeLDI(hash->v[hashcode]);
	ALDI aL = &hash->v[hashcode];
	/*verificare daca exista element in lista*/
	if(ExistaEl(*aL,element,cmpname) != 1)
		return ;
	/*variabila in care salvam valoare key a ultimului element*/
	char *ending = strdup(((TKey*)(hash->v[hashcode]->pre->info))->key);
	if(!ending)
	{
		free(element->key);
		free(element);
		return;
	}
	/*verificare daca lista e vida*/
	if(*aL == NULL)
		return;
	/*eliminare in caz ca este ultimul element din lista*/
	else if(strcmp(ending,element->key) == 0)
    {
    	ElimLDI(&(hash->v[hashcode]->pre),FreeEL);

    }
    /*eliminare daca este in interior sau la inceput*/
	else
	{
		for( ; (*aL)->urm != hash->v[hashcode];)
  		{	
    		TKey *element = (TKey*) (*aL) -> info;
    		if(strcmp(element->key,key) == 0)
    		{
    			ElimLDI(aL,FreeEL);
      			return;
      		}
      		else
      		aL = &(*aL)->urm;
	    } 
    }
    if(lg == 1)
    	hash->v[hashcode] = NULL;
}
/*Functia print */
void print(TH *hash,FILE *f)
{
	TLDI*p, el;
	/*parcurgem vectorul de liste*/
	for(p=hash->v; p<hash->v+hash->M;p++)
	{
		/*daca lista e nevida, afisam elementele*/
		if(*p)
		{
			fprintf(f,"%d:",p-hash->v);
			for(el=*p;el->urm!=*p;el=el->urm)
			{
				AfiEl(el->info,f); 
			}

			AfiEl(el->info,f);
			fprintf(f,"\n");
		}
	}
}
/*Functia print lista index*/
void print_list(int index, TH *hash,FILE *f)
{
	TLDI L = hash->v[index];
	/*daca indexul nu este in intervalul 0,M-1*/
	if(index < 0 || index >= hash->M)
		return ;
	/*daca lista este vida*/
	if(L == NULL)
		fprintf(f,"%d: VIDA\n",index);
	/*afisare lista nevida*/
	else 
	{
		fprintf(f,"%d:",index);
		AfisareLDI(L,AfiEl,f);
		fprintf(f,"\n");
	}	
}
int main(int argc, char *argv[])
{
	int M = atoi(argv[1]);
	int index;
	char input[100],output[100],comanda[100];
	char buffer_key[100],buffer_value[100];
	strcpy(input, argv[2]);
	strcpy(output, argv[3]);

	/*initializare tabela hash*/
	TH *hash_table;
	hash_table = InitTH(M,hash_function);
	if(!hash_table) /*initializare esuata*/
		return 0;
	/*deschidere fisiere*/
	FILE *fin = fopen(input,"r");
	if (!fin)
		printf("Eroare deschidere fisier\n");
	FILE *fout = fopen(output,"w");
	if (!fout)
		printf("Eroare deschidere fisier\n");
	/*citire comenzi din fisier*/
	while(fscanf(fin,"%s",comanda) == 1)
	{
		/*Apel functie SET*/
		if(strcmp(comanda, "set") == 0)
		{
			fscanf(fin,"%s %s",buffer_key,buffer_value);
			set(buffer_key, buffer_value, hash_table);
		}
		/*Apel functie GET*/
		else if(strcmp(comanda, "get") == 0)
		{
			fscanf(fin,"%s",buffer_key);
			void *aux;
			aux = get(buffer_key, hash_table);
			if(aux == NULL)
				fprintf(fout,"NULL\n");
			else
			{
				AfiElGet(aux,fout);
			}
		}
		/*Apel functie REMOVE*/
		else if(strcmp(comanda, "remove") == 0)
		{
            fscanf(fin,"%s",buffer_key);
			delete(buffer_key, hash_table);
		}
		/*Apel functie print*/
		else if(strcmp(comanda, "print") == 0)
		{
			print(hash_table,fout);
		}
		/*Apel functie print_list*/
		else if(strcmp(comanda, "print_list") == 0)
		{
			fscanf(fin,"%d",&index);
			print_list(index,hash_table,fout);
		}
	}

	/*Inchidere fisiere*/
	fclose(fin);
	fclose(fout);
	return 0;
}