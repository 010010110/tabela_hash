// Gabriel Carneiro 2098326
// Karen Alessandra 1833960

#include <stdio.h>
#include <dirent.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#define header() puts("----------------------trabalho final ED---------------------");
#define erro 	 puts("-------------------------Erro-------------------------");

#define _TRY if(status[0])
#define _ERROR else {erro printf("Nao ah Docker/Arquivo nao foi selecionado\n"); delay(timeout); }

#define buffer 102400
#define timeout 2000
#define strsize 100
#define sectors 26

// _______________System Structs_______________

typedef struct {                            // <informacoes de aquivo> ----<level 1>----
	FILE *File;                             // ponteiro do arquivo
	char name[strsize];                     // nome do arquivo
	int_fast64_t size;                      // tamanho do arquivo
	int_fast64_t allocFact;         		// allocacao dinamica dependendo do tamanho do arquivo
	bool runOver;                           // Indicador de reeabertura de arquivo
}Scan;

typedef struct {                            // <tracking dos aquivos na pasta> ----<level 2>----
	int_fast64_t qnt;                       // quantidade de arquivos ativos
	Scan scan[80];                          // arquivos % maximo de 80 aquivos, estatico pra poupar malloc %
}ShadowScan;

// ____________________________________________

// _______________Docker Structs_______________

typedef struct {                            // <armazenamento das strings> ----<level 1>----
	char nome[strsize];                     // nome mantido
}Data;

typedef struct {                            // <onde sao mantidos varios nomes da mesma letra inicial> ----<level 2>----
	bool enable;                            // status do container, (enable == true) <- ativo
	int_fast32_t letter;                    // letra de representacao
	int_fast64_t total;                     // tamanho da allocacao total
	int_fast64_t used;                      // quantidade de nomes
	Data *data;                             // ponteiro de dados
}Container;

typedef struct {                             // <docker main onde sao mantidos todos os containers> ----<level 3>----
	Container container[sectors];  // containers
}Docker;

// ____________________________________________


void dockerInit(Docker *);
void insert(Docker *);
void exitProgram(Docker *);
void shadowScan(ShadowScan *);
void selectSort(Docker *);
void createDocker(Scan *, Docker *);
void dockerKiller(Docker *);
void _fastPrintDocker(Docker *);
void _normalPrintDocker(Docker *);
void delay(int);
int fsize(FILE *);
bool compatible(char file[]);
Container reallocDocker(Container);
void bubbleSortContainer(Container *);
void selectSortContainer(Container *);
void bubbleSort(Docker *);
int _menuSort();


/////////////////////////////////MENU/////////////////////////////////

int _menuSort()
{
	int opc;
	system("@cls||clear");
	puts("-----------------Metodos de Ordenacao-----------------\n");
	puts("[1] - BubbleSort");
	puts("[2] - SelectSort\n");
	printf("escolha:");
	scanf("%d", &opc);
	getchar();
	return opc;
}

void menuText()
{
	puts("[1] Criar Docker");
	puts("[2] Ver Docker");
	puts("[3] Ordenar Docker");
	puts("[4] Pesquisar nome");
	puts("[5] Deletar nome");
	puts("[6] Inserir nome");
	puts("[7] Relatorio de execucao");
	puts("[8] Destruir docker");
	puts("[9] Sair\n");
	printf("Escolha:");
}

int menuPesquisa(){
	int opc;
	system("@cls||clear");
	puts("[1] - pesquisa simples");
	puts("[2] - pesquisa binaria");
	printf("Escolha:");
	scanf("%d", &opc);
	getchar();
	return opc - 1;
}

int showFiles(ShadowScan *sscan)
{
	int i, chose;
	puts("-----------------Arquivos Disponiveis-----------------\n");
	for (i = 0; i < sscan->qnt; i++)
		printf("[%d] %s\n", i + 1, sscan->scan[i].name);
	printf("escolha:");
	scanf("%d", &chose);
	if (chose < 0 || chose > sscan->qnt) 
	{
		system("cls||clear");
		chose = showFiles(sscan);
	}
	return chose;
}

/////////////////////////////////MENU/////////////////////////////////


int_fast64_t ratioAlloc(int fSize) { return ((fSize / 15) / 5); }
int_fast64_t _ASCII(char nome[]) { return tolower(nome[0]) - 97; }

void dockerInit(Docker *docker) 		// Inicializacao do docker - ok
{
	int_fast64_t i;
	int_fast64_t ltr;
	for (i = 0, ltr = 'a'; i < sectors; i++) 
	{
		docker->container[i].letter = ltr++;
		docker->container[i].total = 1;
		docker->container[i].used = 0;
		docker->container[i].enable = false;
		docker->container[i].data = malloc(sizeof(Data));		
	}
}

Container reallocDocker(Container cnt) // Realocacao do container
{
	int_fast64_t i;
	Container cntCpy;
	cntCpy.data = malloc((cnt.total * 2) * sizeof(Data));
	for (i = 0; i < cnt.total; i++)
		cntCpy.data[i] = cnt.data[i];
	cntCpy.enable = true;
	cntCpy.letter = cnt.letter;
	cntCpy.used = cnt.used;
	cntCpy.total = (cnt.total * 2);
	return cntCpy;
}


void reOpen(Scan *scan)
{
	char file[50];
	fclose(scan->File);
	strcpy(file, "./files/");
	strcat(file, scan->name);
	scan->File = fopen(file, "r");
	scan->runOver = false;
}

void createDocker(Scan *scan, Docker *docker) 	// Criar e alocar memoria dinamica para o docker
{
	char nome[strsize];
	int_fast64_t i;
	if (scan->runOver) 							// se alguna vez o arquivo ja foi percorido
		reOpen(scan);
	for (i = 0; i < sectors; i++) 
	{
		docker->container[i].data = malloc(scan->allocFact * sizeof(Data));
		docker->container[i].total = scan->allocFact;
	}
	while (feof(scan->File) == 0) 
	{
		fgets(nome, sizeof(nome), scan->File);
		if (!docker->container[_ASCII(nome)].enable)
			docker->container[_ASCII(nome)].enable = true;
		nome[strlen(nome) - 1] = '\0';
		strcpy(docker->container[_ASCII(nome)].data[docker->container[_ASCII(nome)].used].nome, nome);
		docker->container[_ASCII(nome)].used++;
		if (docker->container[_ASCII(nome)].used == docker->container[_ASCII(nome)].total)
			docker->container[_ASCII(nome)] = reallocDocker(docker->container[_ASCII(nome)]);
	}
	scan->runOver = true;
}

void dockerKiller(Docker *docker)           	 // Destruir docker
{
	int_fast64_t j, i;
	for (j = 0; j < sectors; j++) 
	{
		if (docker->container[j].enable) 
		{
			for (i = 0; i < docker->container[j].used; i++)
				strcpy(docker->container[j].data[i].nome, " ");
		}
	}
	free(docker);
}

void insert(Docker *docker)
{
	int_fast64_t indice;
	char name[strsize];
	printf("Novo nome:");
	scanf("%100[^\n]s", name);
	indice = _ASCII(name);
	if (indice >= 0 && indice < sectors) 
	{
		if (docker->container[indice].used == docker->container[indice].total)
			docker->container[indice] = reallocDocker(docker->container[indice]);
		strcpy(docker->container[indice].data[docker->container[indice].used].nome, name);
		docker->container[indice].used++;
		if (!docker->container[indice].enable)
			docker->container[indice].enable = true;
		puts("Nome inserido!");
		delay(timeout);
	}
}

void bubbleSortContainer(Container *container)
{
	char nameCpy[strsize];
	int_fast64_t i, j;
	for (i = 0; i < container->used; i++) 
	{
		for (j = 0; j < container->used - 1; j++) 
		{
			if (strcmp(container->data[j].nome, container->data[j + 1].nome) > 0) 
			{
				strcpy(nameCpy, container->data[j].nome);
				strcpy(container->data[j].nome, container->data[j + 1].nome);
				strcpy(container->data[j + 1].nome, nameCpy);
			}
		}
	}
}

void selectSortContainer(Container *container)
{
	int_fast64_t comp = 0, troca = 0, ant;
	int_fast64_t j, i;
	char temp[strsize];
	for (j = 0; j < container->used; j++) 
	{
		ant = j;
		comp++;
		for (i = (j + 1); i < container->used; i++) 
		{
			if (strcmp(container->data[ant].nome, container->data[i].nome) > 0)
			{
				ant = i;
				comp++;
			}
		}
		troca++;
		strcpy(temp, container->data[ant].nome);
		strcpy(container->data[ant].nome, container->data[j].nome);
		strcpy(container->data[j].nome, temp);
		comp++;
	}
}

void selectSort(Docker *docker)
{
	int_fast8_t i;
	for (i = 0; i < sectors; i++) 
	{
		if (docker->container[i].enable)
			selectSortContainer(&docker->container[i]);
	}
}

void bubbleSort(Docker *docker)
{
	int_fast8_t i;
	for (i = 0; i < sectors; i++) 
	{
		if (docker->container[i].enable)
			bubbleSortContainer(&docker->container[i]);
	}
}



bool sort(Docker *docker)
{
	int_fast8_t opc = _menuSort();
	switch (opc) 
	{
		case 1:
			bubbleSort(docker);
			return true;
		case 2:
			selectSort(docker);
			return true;
	}
	return false;
}

bool compatible(char file[])
{
	int_fast64_t size = strlen(file);
	if (size > 3) 
	{
		if ( file[size - 1] == 't' && file[size - 2] == 'x'  && file[size - 3] == 't') 
			return true;
	}
	return false;
}

void _fastPrintDocker(Docker *docker)
{
	int_fast64_t i, j;
	fflush(stdout);
	setvbuf(stdout, NULL, _IOFBF, buffer);
	for (i = 0; i < sectors; i++) 
	{
		for (j = 0; j < docker->container[i].used; j++) 
		{
			fputs(docker->container[i].data[j].nome, stdout);
			putc('\n', stdout);
		}
	}
	fflush(stdout);
	setvbuf(stdout, NULL, _IONBF, 1);
}

void normalPrintDocker(Docker *docker)
{
	int i, j;
	for (i = 0; i < sectors; i++) {
		if (docker->container[i].enable) 
		{
			printf("------------------------------------------------------------------------[%c]\n", docker->container[i].letter);
			for (j = 0; j < docker->container[i].used; j++)
				printf("Nome: %s\n", docker->container[i].data[j].nome);
		}
	}
	puts("\nEND----------------------------------------------------------------------END");
}

bool deepCheck(FILE *f, char fileName[])
{
	char nome[strsize];
	if (fsize(f) < 100)
		return false;
	while (feof(f) == 0) 
	{
		fgets(nome, sizeof(nome), f);
		if (nome[0] > 122 || nome[0] < 65) {
			erro;
			printf("\nArquivo %s nao eh compativel", fileName);
			printf("\nString ilegal:%s", nome);
			puts("\nTenha certeza que o arquivo esteja em UTF-8");
			erro;
			system("pause");
			return false;
		}
	}
	return true;
}

int fsize(FILE *file)
{
	int prev = ftell(file);
	fseek(file, 0L, SEEK_END);
	int sz = ftell(file);
	fseek(file, prev, SEEK_SET);
	return sz;
}

void delay(int ms)
{
	clock_t time[2];
	time[0] = clock();
	do {
		time[1] = clock();
	}while (((time[1] - time[0]) * 1000.0 / CLOCKS_PER_SEC) <= ms);
}

void shadowScan(ShadowScan *sscan)
{
	int i = 0;
	FILE *f;
	DIR *d;
	char file[50];
	struct dirent *dir;
	d = opendir("./files/./");
	if (d) 
	{
		while ((dir = readdir(d)) != NULL) 
		{
			strcpy(file, "./files/");
			strcat(file, dir->d_name);
			f = fopen(file, "r");
			if(!(f == NULL)) 
			{
				if (compatible(file) && deepCheck(f, file)) 
				{
					sscan->scan[i].File = fopen(file, "r");
					sscan->scan[i].size = fsize(sscan->scan[i].File);
					sscan->scan[i].allocFact = (ratioAlloc(sscan->scan[i].size));
					sscan->scan[i].runOver = false;
					strcpy(sscan->scan[i++].name, dir->d_name);
				}
				fclose(f);
			}	
		}
		closedir(d);
		sscan->qnt = i;
	}
}

bool searchCore(Docker *docker, char input[], int_fast64_t *i, int_fast64_t *local)
{
	if (docker->container[*local].enable) 
	{
		for (*i = 0; *i < docker->container[*local].used; (*i)++) 
		{
			if (strcmp(docker->container[*local].data[*i].nome, input) == 0) 
				return true;
		}
	}
	return false;
}

bool binarySearch(Container *cnt, char name[])
{
    int_fast64_t mid, low = 0, high = cnt->used;
    do 
	{
    	mid = (low + high) / 2;
    	if (strcmp(name, cnt->data[mid].nome) < 0)
    		high = mid - 1;
     	else if (strcmp(name, cnt->data[mid].nome) > 0)
    		low = mid + 1;
    } while (strcmp(name, cnt->data[mid].nome) != 0 && low <= high);
    if (strcmp(name, cnt->data[mid].nome) == 0) 
	{
    	printf("Nome encontrado:%s\n", name);
    	return true;
    } else {	
    	puts("Nome nao encontrado");
    	return false;
    }
}

bool search(Docker *docker, bool search[], int mode)
{
	bool found = false;
	int_fast64_t i, local;
	char input[strsize];
	printf("Informe o nome:");
	scanf("%[^\n]s", input);
	if(strlen(input) > 3) 
	{
		local = _ASCII(input);
		if (mode == 1)
			found = binarySearch(&docker->container[local], input);
		else
			found = searchCore(docker, input, &i,  &local);
		if (found) 
		{
			switch (mode) 
			{
			case 0:
				printf("Nome encontrado:%s\n", docker->container[local].data[i].nome);
				delay(timeout);
				return true;
			case 2:
				printf("Nome deletado:%s\n", docker->container[local].data[i].nome);
				delay(timeout);
				while (i < docker->container[local].used) {
					strcpy(docker->container[local].data[i].nome, docker->container[local].data[i + 1].nome);
					i++;
				}
				docker->container[local].used--;
				if (docker->container[local].used == 0)
					docker->container[local].enable = false;
				return true;
			}
		}
	}
	return false;
}



bool openFile(Docker *docker, ShadowScan *sscan)
{
	int chose;
	chose = showFiles(sscan);
	createDocker(&sscan->scan[chose - 1], docker);
	return true;
}


char* data()
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	return asctime(timeinfo);
}

float percent(Container cnt)
{
	return ((cnt.total - cnt.used) * (100.0 / cnt.total));
}

void relatorioSave(double tempoGasto[], double tempoFinal, Docker *docker, char filename[])
{
	float media = 0;
	FILE *r;
	int i, cont = 0;
	r = fopen("relatorio.txt", "a+");
	fprintf(r, "\n-------------------------------------------------------------------------------------\n");
	fprintf(r, "%-30c%s\n", ' ', data());
	fprintf(r, "Arquivo:%s\n", filename);
	fprintf(r, "Criar Docker:%gms\n", tempoGasto[0]);
	fprintf(r, "Ordenar:%gms\n", tempoGasto[1]);
	fprintf(r, "Imprimir:%gms\n", tempoGasto[2]);
	fprintf(r, "Pesquisar com sucesso:%gms\n", tempoGasto[3]);
	fprintf(r, "Pesquisar sem sucesso:%gms\n", tempoGasto[4]);
	fprintf(r, "Tempo final: %gms\n", tempoFinal);
	fprintf(r, "/////////////////////////////////////////////////////////////////////////\n\n");
	fprintf(r, "[System LOG]\n");
	fprintf(r, "Container usados:\n");
	for (i = 0; i < sectors; i++) 
	{
		if (docker->container[i].enable) 
		{
			fprintf(r, "[%c]", docker->container[i].letter);
			media += percent(docker->container[i]);
			cont++;
		}
		if(i == 13)
			fputc('\n', r);
	}
	fprintf(r, "\nMedia de espaco livre no docker: %.2g%%", media / cont);
	fclose(r);
}

void guessingName(Docker *docker, char name[])
{
	int i;
	bool found = false;
	srand(time(0));
	do 
	{
		for (i = rand() % sectors; i < sectors;i++) 
		{
			if(docker->container[i].enable) 
			{
				strcpy(name, docker->container[i].data[(rand() % docker->container[i].used)].nome);
				found = true;
				break;
			}
		}
	} while(!found);
}

void relatorioResult(char name[], time_t T[], Docker *docker, Scan file, bool found[])
{
	double tempoGasto[5], tempoFinal = 0;
	tempoGasto[0] = (T[1] - T[0]) * 1000.0 / CLOCKS_PER_SEC;
	tempoGasto[1] = (T[3] - T[2]) * 1000.0 / CLOCKS_PER_SEC;
	tempoGasto[2] = (T[5] - T[4]) * 1000.0 / CLOCKS_PER_SEC;
	tempoGasto[3] = (T[7] - T[6]) * 1000.0 / CLOCKS_PER_SEC;
	tempoGasto[4] = (T[9] - T[8]) * 1000.0 / CLOCKS_PER_SEC;
	tempoFinal = tempoGasto[0] + tempoGasto[1] + tempoGasto[2] + tempoGasto[3] + tempoGasto[4];
	puts("\n-------------------------------------------------------------------------------------\n");
	printf("Arquivo escolhido:%s\n", file.name);
	printf("Criar Docker:%gms\n", tempoGasto[0]);
	printf("Ordenar:%gms\n", tempoGasto[1]);
	printf("Imprimir:%gms\n", tempoGasto[2]);
	printf("Pesquisar com sucesso:%gms\tNome pesquisado:[%s]%s\n", tempoGasto[3], found[0]? "true":"false" ,name);
	printf("Pesquisar sem sucesso:%gms\tNome pesquisado:[%s]Joker envangelico\n", tempoGasto[4], found[1]? "true":"false");
	printf("Tempo final: %gms\n\n", tempoFinal);
	puts("\n-------------------------------------------------------------------------------------\n");
	relatorioSave(tempoGasto, tempoFinal, docker, file.name);
	system("pause");
}



void exitProgram(Docker *docker)
{
	if (docker != NULL)
		dockerKiller(docker);
	exit(EXIT_SUCCESS);
}

void relatorio(ShadowScan *sscan)
{
	Docker *docker;
	char name[strsize];
	docker = malloc(sizeof(Docker));
	dockerInit(docker);
	time_t T[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int chosesort, chose = showFiles(sscan);
	int_fast64_t dummyLocal, dummyIndice;
	bool found[2];
	T[0] = clock();
	createDocker(&sscan->scan[chose - 1], docker);
	T[1] = clock();
	guessingName(docker, name);
	chosesort = _menuSort();
	printf("\nOrdenando....");
	switch (chosesort) {
	case 1:
		T[2] = clock();
		bubbleSort(docker);
		T[3] = clock();
		break;
	case 2:
		T[2] = clock();
		selectSort(docker);
		T[3] = clock();
		break;
	}
	T[4] = clock();
	_fastPrintDocker(docker);
	T[5] = clock();
	dummyLocal = _ASCII(name);
	T[6] = clock();
	found[0] = searchCore(docker, name, &dummyIndice, &dummyLocal);
	T[7] = clock();
	dummyLocal = _ASCII("Joker envangelico");
	T[8] = clock();
	found[1] = searchCore(docker, "Joker envangelico", &dummyIndice, &dummyLocal);
	T[9] = clock();
	relatorioResult(name, T, docker, sscan->scan[chose - 1], found);
	free(docker);
}

void option(Docker *docker, ShadowScan *sscan)
{
	bool status[] = { false, false, false };
	int opc;
	while (1) {
		header();
		menuText();
		scanf("%d", &opc);
		getchar();
		system("cls||clear");
		switch (opc) 
		{
			case 1:
				if (!status[0]) {
					status[0] = openFile(docker, sscan);
					if (status[0]) 
					{
						printf("Docker criado");
						delay(timeout);
					}
				} else {
					erro
					puts("O Docker existente precisa ser destruido");
					delay(timeout);
				}
				break;
			case 2:
				_TRY
				{
					normalPrintDocker(docker);
					putchar('\n');
					system("pause");
				}
				_ERROR
				break;
			case 3:
				_TRY
					status[1] = sort(docker);
				_ERROR
				break;
			case 4:
				_TRY{
					switch(menuPesquisa()) 
					{
						case 0:
							status[2] = search(docker, status, 0);
							if(!status[2])
							{
								erro
								printf("Nome nao encontrado!");
								delay(timeout);
							}
							break;
						case 1:
							if(status[1]) 
								status[2] = search(docker, status, 1);	
							else {
								erro
								printf("Nao se da pra fazer uma pesquisa binaria sem a ordenacao");
							}
								delay(timeout);	
					}	
				}
				_ERROR
				break;
			case 5:
				_TRY
				{
					status[2] = search(docker, status, 2);
					if(!status[2]){
						erro
						printf("Nome nao encontrado!");
						delay(timeout);
					}
				}
				_ERROR
				break;
			case 6:
				_TRY
				insert(docker);
				_ERROR
				break;
			case 7:
				if (sscan->qnt > 0) 
					relatorio(sscan);
				else {
					erro
					puts("Nao ha arquivo a se processar!");
				}
				system("pause");
				break;
			case 8:
				dockerKiller(docker);
				docker = malloc(sizeof(Docker));
				dockerInit(docker);
				status[0] = false;
				break;
			case 9:
				exitProgram(docker);
		}
		system("cls||clear");
	}
}


int main()
{
	ShadowScan *sscan;
	sscan = malloc(sizeof(ShadowScan));
	Docker *docker;
	docker = malloc(sizeof(Docker));
	dockerInit(docker);
	shadowScan(sscan);
	option(docker, sscan);
}
