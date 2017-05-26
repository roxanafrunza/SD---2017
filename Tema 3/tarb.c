#include "tarb.h"
#include "TCoada-V.h"
//#include "functii_coada.h"
TArb InitA() /*initializare arbore*/
{
	TArb arb = (TArb) malloc(sizeof(TNod));
	if(!arb)
		return NULL;
	arb->fiu = NULL;
	arb->frate = NULL;
	arb->info.cod = '\0';
	arb->info.caracter = '\0';
	return arb;
}
int ArbVid(TArb arb)
{
	if(arb == NULL)
		return -1; /*arbore neinitializat*/
	if(arb->fiu == NULL && arb->frate == NULL) /*arbore vid*/
		return 1;
	return 0; /*arbore nevid*/
}
TArb ConstrNod(char c)
{
	TArb nod = malloc(sizeof(TNod));
	if(!nod)
		return NULL;
	nod->fiu = NULL;
	nod->frate = NULL;
	nod->info.cod = c;
	nod->info.caracter = '\0';
	return nod;
}
void AddArb(TArb arb,TArb root, char* codificare, char caracter,int index)
{
	int urm = index + 1;
	if(codificare[urm] == '\0' && codificare[index] == arb->info.cod) /*sfarsitul sirului codificat */
	{
		arb->info.caracter = caracter;
		return ;
	}
	if(arb == root)
	{
		if(ArbVid(root) == 1)
			arb->fiu = ConstrNod(codificare[index]);
		AddArb(arb->fiu,root,codificare,caracter,index);
	}
	else if(arb->info.cod == codificare[index]) 
	{
		if(arb->fiu == NULL || codificare[index+1] == '\0')
			arb->fiu = ConstrNod(codificare[index+1]);
		AddArb(arb->fiu,root,codificare,caracter,index + 1);
	}
	else
	{
		if(arb->frate == NULL && arb->info.cod == codificare[index]) 
		{
			AddArb(arb->fiu,root,codificare,caracter,index + 1);
		}
		/*suntem la sfarsitul listei de frati si nu a fost gasit caracterul*/
		if(arb->frate == NULL && arb->info.cod != codificare[index]) 
		{
			arb->frate = ConstrNod(codificare[index]);
			AddArb(arb->frate,root,codificare,caracter,index);
		}
		else
		{
			AddArb(arb->frate,root,codificare,caracter,index);
		}
	}
}
TArb ConstrArb(int N, FILE *f)
{
	int i;
	char caracter;
	char cod[50];
	TArb arb = InitA();
	TArb root = arb;
	if(!arb)
		return NULL;
	for(i = 0; i < N; i++)
	{
		fscanf(f,"%c %s\n", &caracter, cod);
		AddArb(arb,root, cod,caracter,0);
		
	}
	return arb;
}
void AfisareArb2(TArb arb)
{
	if(!arb)
		return ;
	printf("(%c:%c) \n", arb->info.cod, arb->info.caracter);
	AfisareArb2(arb->frate);
	AfisareArb2(arb->fiu);
}
void AfisLatime(TArb arb, FILE *f)
{
	if(!arb)
		return ;
	TArb rad = arb,aux;
	int lg;
	void *element = malloc(sizeof(TNod));
	if(!element)
		return ;
	void *coada = InitQ(sizeof(TNod),200);
	if(!coada)
	{
		free(element);
		return ;
	}
	IntrQ(coada,rad);
	lg = NrEQ(coada);
	while(!VidaQ(coada))
	{
		lg = NrEQ(coada);
		while(lg > 0)
		{
			ExtrQ(coada,element);

			if ( ( (TArb) element)->info.cod == '\0' 
				&& ( (TArb) element )->info.caracter == '\0')
				fprintf(f,"(:)");
			else if ( ( (TArb) element)->info.caracter == '\0' )
				fprintf(f,"(%c:)", ( (TArb) element)->info.cod);
			else
				fprintf(f, "(%c:%c)",((TArb) element)->info.cod,
									((TArb) element)->info.caracter);

			aux  = ((TArb) element)->fiu;
			while(aux != NULL)
			{
				IntrQ(coada,aux);
				aux = aux->frate;
			}
			lg--;			
		}
		fprintf(f,"\n");
	}
	free(element);
	DistrQ(&coada);	
}
void add(TArb arb, char *codificare, char caracter)
{
	TArb exista = ExistaArb(arb, caracter);
	if(exista == NULL) /*caracterul nu exista*/
		AddArb(arb,arb,codificare,caracter,0);
	else
	{
		delete(arb,caracter);
		AddArb(arb,arb,codificare,caracter,0);
	}
}
TArb ExistaArb(TArb arb, char caracter)
{
	if(!arb)
		return NULL;
	else if(arb->info.caracter == caracter)
		return arb;
	else
	{
		TArb gasit = ExistaArb(arb->frate,caracter);
		return gasit ? gasit : ExistaArb(arb->fiu,caracter);
	}
}
int VerificaInfo(TArb arb, char caracter)
{
	if(arb == NULL)
		return 0;
	if(arb->info.caracter == caracter)
		return 1;
	return 0;
}
TArb ParinteNod(TArb arb, char caracter)
{
	if(arb == NULL)
		return NULL;
	else
	{
		if ( (VerificaInfo(arb->frate,caracter))
			|| (VerificaInfo(arb->fiu,caracter)) )
			return arb;
		else
		{
			TArb gasit =  ParinteNod(arb->frate,caracter);
			return gasit ? gasit : ParinteNod(arb->fiu,caracter);
		}
	}
}
int delete(TArb arb, char caracter)
{
	TArb exista = ExistaArb(arb,caracter);
	TArb parinte = ParinteNod(arb,caracter);
	if(arb == NULL) /*arbore neinitializat*/
		return 0;
	if(ArbVid(arb) == 1) /*arbore doar cu radacina*/
		return 0;
	if(exista == NULL)
		return 0; /*nu exista elementul in arbore*/
	else if(exista->fiu != NULL)
	{
		exista->info.caracter = '\0';
		return 1;
	}
	else if (caracter != '\0')
	{
		if(parinte->frate == exista)
		{
			parinte->frate = exista->frate;
		}
		else if(parinte->fiu == exista)
		{
			parinte->fiu = exista->frate;
			if(parinte->info.caracter == '\0' && parinte->fiu == NULL)
			{
				parinte->info.caracter = '#';
				while(delete(arb,'#'));
			}
		}
		free(exista);
	}
	return 1; /*eliminare cu succes*/
}
char Decodificare(TArb arb,TArb root, char *string,int index)
{
	if(arb == NULL)
		return 0;
	if(string[index + 1] == '\0' 
		&& arb != NULL && string[index] == arb->info.cod)
	{
		return arb->info.caracter;
	}
	if(arb == root)
		return Decodificare(arb->fiu,root,string,index);
	if(string[index] == arb->info.cod)
	{
		return Decodificare(arb->fiu,root,string,index+1);
	}
	else
	{
		return Decodificare(arb->frate,root,string,index);
	}
}
void Distruge(TArb arb)
{
	if(!arb)
		return ;
	Distruge(arb->fiu);
	Distruge(arb->frate);
	free(arb);
}
void DistrugeArb(AArb arb)
{
	if(!(*arb))
		return ;
	Distruge(*arb);
	*arb = NULL;
}
int main(int argc, char *argv[])
{
	int N,M,i;
	TArb arb;
	char input1[20],input2[20];
	char output1[20],output2[20];
	char comanda[10], cod[20];
	char caracter, delimitator,c;

	strcpy(input1,argv[1]);
	strcpy(input2,argv[2]);
	strcpy(output1,argv[3]);
	strcpy(output2,argv[4]);

	FILE *input_arb = fopen(input1,"r");
	if(!input_arb)
		printf("Eroare deschidere fisier\n");
	FILE *input_text = fopen(input2,"r");
	if(!input_text)
		printf("Eroare deschidere fisier\n");
	FILE *output_arb = fopen(output1,"w");
	if(!output_arb)
		printf("Eroare deschidere fisier\n");
	FILE *output_text = fopen(output2,"w");
	if(!output_text)
		printf("Eroare deschidere fisier\n");

	fscanf(input_arb, "%d\n", &N);

	arb = ConstrArb(N,input_arb);
	AfisLatime(arb,output_arb);

	fscanf(input_arb,"%c\n",&delimitator);

	fscanf(input_arb, "%d\n", &M);
	for(i = 0; i < M; i++)
	{
		fscanf(input_arb, "%s ",comanda);
		if(strcmp(comanda,"add") == 0)
		{
			fscanf(input_arb, "%c %s\n", &caracter, cod);
			add(arb,cod,caracter);
		}
		else if(strcmp(comanda,"delete") == 0)
		{
			fscanf(input_arb, "%c\n", &caracter);
			delete(arb,caracter);
		}
		AfisLatime(arb,output_arb);
	}
	int len = 0;
	while( (c = fgetc(input_text))  != EOF )
	{
		char codificare[20],litera;
		if(c == ' ')
			fprintf(output_text," ");
		else if (c == '\n')
			fprintf(output_text,"\n");
		else if(c != delimitator)
		{
				codificare[len] = c;
				len++;
		}
		else if( c == delimitator) 
		{
			while(len < 20)
				codificare[len++] = '\0';
			litera = Decodificare(arb,arb,codificare,0);
			if(litera == '\0');
			else
				fprintf(output_text,"%c", litera);
			len = 0;
		}
	}


	DistrugeArb(&arb);
	fclose(input_arb);
	fclose(input_text);
	fclose(output_arb);
	fclose(output_text);
	return 0;
}