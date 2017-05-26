#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef struct 
{
	char cod;
	char caracter;
} TInfo;

typedef struct Node
{
	TInfo info;
	struct Node *frate, *fiu;
} TNod,*TArb, **AArb;

TArb InitA(); /*initializare arbore*/
	/*verifica daca arborele este vid; 1/0 - vid/nevid*/
int ArbVid(TArb arb);
	/*construieste nod cu cod c si caracter \0 */
TArb ConstrNod(char c); 
void AddArb(TArb arb,TArb root, char* codificare, char caracter,int index);
void AfisareArb(TArb arb, FILE *f);
void AfisareArb2(TArb arb);
void AfisLatime(TArb arb, FILE *f);
TArb ExistaArb(TArb arb, char caracter);
TArb ParinteNod(TArb arb, char caracter);
int VerificaInfo(TArb arb, char caracter);
void add(TArb arb, char *codificare, char caracter);
int delete(TArb arb, char caracter);
char Decodificare(TArb arb,TArb root, char *string,int index);
void Distruge(TArb arb);
void DistrugeArb(AArb arb);