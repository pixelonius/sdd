#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

//stiva
struct student {
	int cod;
	char* nume;
};
typedef struct student student;
typedef struct nodLista nodLista;
struct nodLista {
	student info;
	nodLista* next;
};

void push(nodLista** varf, student s) {
	nodLista* nou = (nodLista*)malloc(sizeof(nodLista));
	nou->next = NULL;
	nou->info.cod = s.cod;
	nou->info.nume = (char*)malloc(sizeof(char*) * (strlen(s.nume) + 1));
	strcpy(nou->info.nume, s.nume);
	if (*varf == NULL) {
		*varf = nou;
	}
	else {
		nou->next = *varf;
		*varf = nou;
	}
}

int pop(nodLista** varf, student* s) {
	if (*varf == NULL)
		return -1;
	else {
		s->cod = (*varf)->info.cod;
		s->nume = (char*)malloc(sizeof(char*) * (strlen((*varf)->info.nume) + 1));
		strcpy(s->nume, (*varf)->info.nume);
		nodLista* temp = *varf;
		*varf = (*varf)->next;
		free(temp->info.nume);
		free(temp);
		return 0;
	}
}

//coada
void put(nodLista** prim, nodLista** ultim, student s) {
	nodLista* nou = (nodLista*)malloc(sizeof(nodLista));
	nou->info.cod = s.cod;
	nou->info.nume = (char*)malloc(sizeof(char*) * (strlen(s.nume) + 1));
	strcpy(nou->info.nume, s.nume);
	nou->next = NULL;
	if (*prim == NULL && *ultim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		*ultim = nou;
	}

}

int get(nodLista** prim, nodLista** ultim, student* s) {
	if (*prim != NULL && *ultim != NULL) {
		s->cod = (*prim)->info.cod;
		s->nume = (char*)malloc(sizeof(char*) * (strlen((*prim)->info.nume) + 1));
		strcpy(s->nume, (*prim)->info.nume);
		nodLista* temp = *prim;
		*prim = (*prim)->next;
		free(temp->info.nume);
		free(temp);
		return 0;
	}
	else {
		if (*prim == NULL) {
			*ultim == NULL;
			return -1;
		}
	}
}

//hashtable
typedef struct hash hash;
struct hash {
	nodLista** vect;
	int marime;
};

int inserare(hash tabela, student stud) {
	int pozitie = 0;
	if (tabela.vect != NULL) {
		pozitie = stud.cod % tabela.marime;
		nodLista* nou = (nodLista*)malloc(sizeof(nodLista));
		nou->info.cod = stud.cod;
		nou->info.nume = (char*)malloc((strlen(stud.nume) + 1) * sizeof(char*));
		strcpy(nou->info.nume, stud.nume);
		nou->next = NULL;
		if (tabela.vect[pozitie] == NULL) {
			tabela.vect[pozitie] = nou;
		}
		else {
			nodLista* temp = tabela.vect[pozitie];
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = nou;

		}
	}
	return pozitie;

}

void traversareLista(nodLista* cap) {
	nodLista* temp = cap;
	while (temp) {
		printf("\nCodul: %d, Numele: %s ", temp->info.cod, temp->info.nume);
		temp = temp->next;
	}
}

void traversareTabela(hash tabela) {
	if (tabela.vect != NULL) {
		for (int i = 0; i < tabela.marime; i++) {
			if (tabela.vect[i] != NULL)
				traversareLista(tabela.vect[i]);
		}
	}
}
void stergereNod(nodLista* nod) {
	free(nod->info.nume);
	free(nod);
}

int stergere(hash tabela, int cod) {
	int pozitie = -1;
	if (tabela.vect != NULL) {
		pozitie = cod % tabela.marime;
		nodLista* temp = tabela.vect[pozitie];
		if (temp == NULL)
			return -1;
		else {
			if (temp->info.cod == cod) {
				if (temp->next == NULL) {
					tabela.vect[pozitie] = NULL;
					stergereNod(temp);
				}
				else {
					//nodLista* temp2 = temp;
					tabela.vect[pozitie] = temp->next;
					stergereNod(temp);//temp2
				}
			}
			else {
				while (temp->next) {
					nodLista* temp2 = temp->next;
					if (temp2->info.cod != cod)
						temp = temp->next;
					else break;
				}
				nodLista* gasit = temp->next;
				if (gasit != NULL) {
					if (gasit->next == NULL) {
						temp->next = NULL;
						stergereNod(gasit);
					}
					else {
						temp->next = gasit->next;
						stergereNod(gasit);
					}
				}
				else return -1;
			}
		}
	}
	return pozitie;
}

//heap
typedef struct heap heap;
struct heap {
	student* vect;
	int marime;
};

void filtrare(heap h, int index) {
	int indexMax = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS<h.marime && h.vect[indexS].cod>h.vect[indexMax].cod)
		indexMax = indexS;
	if (indexD<h.marime && h.vect[indexD].cod>h.vect[indexMax].cod)
		indexMax = indexD;

	if (indexMax != index) {
		student temp = h.vect[index];
		h.vect[index] = h.vect[indexMax];
		h.vect[indexMax] = temp;
		filtrare(h, indexMax);
	}
}

void inserareHeap(heap* h, student s) {
	student* vect1 = (student*)malloc(sizeof(student) * (h->marime + 1));
	for (int i = 0; i < h->marime; i++)
		vect1[i] = h->vect[i];

	h->marime++;
	free(h->vect);
	h->vect = vect1;
	h->vect[h->marime - 1] = s;

	for (int i = (h->marime - 1) / 2; i >= 0; i--)
		filtrare(*h, i);
}

void stergereHeap(heap* h, student* s) {
	student* vect1 = (student*)malloc(sizeof(student) * (h->marime - 1));
	student temp = h->vect[h->marime - 1];
	h->vect[h->marime - 1] = h->vect[0];
	h->vect[0] = temp;

	for (int i = 0; i < h->marime - 1; i++)
		vect1[i] = h->vect[i];

	h->marime--;
	free(h->vect);
	h->vect = vect1;

	for (int i = (h->marime - 1) / 2; i >= 0; i--) {
		filtrare(*h, i);
	}
}

void afisareHeap(heap h) {
	for (int i = 0; i < h.marime; i++) {
		printf("%d ", h.vect[i].cod);
	}
}

//LoL
typedef struct nodLS nodLS;
struct nodLS {
	student info;
	nodLS* next;
};

typedef struct nodLP nodLP;
struct nodLP {
	nodLS* info;
	nodLP* next;
};

void inserareLS(nodLS** cap, student s) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->info.cod = s.cod;
	nou->info.nume = (char*)malloc(sizeof(char*) * (strlen(s.nume) + 1));
	strcpy(nou->info.nume, s.nume);
	nou->next = NULL;
	if (*cap == NULL)
		*cap = nou;
	else {
		nodLS* temp = *cap;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
}

void inserareLP(nodLP** capLP, nodLS** capLS) {
	nodLP* nou = (nodLP*)malloc(sizeof(nodLP));
	nou->info = *capLS;
	nou->next = NULL;
	if (*capLP == NULL)
		*capLP = nou;
	else {
		nodLP* temp = *capLP;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
}

void traversareLS(nodLS* cap) {
	if (cap) {
		nodLS* temp = cap;
		while (temp) {
			printf("%d ", temp->info.cod);
			temp = temp->next;
		}
	}
}


void traversareLP(nodLP* cap) {
	if (cap) {
		nodLP* temp = cap;
		while (temp) {
			//printf sublista;
			traversareLS(temp->info);
			temp = temp->next;
		}
	}
}
//BST
typedef struct BST BST;
struct BST {
	student inf;
	BST* left;
	BST* right;
};
BST* creareNod(student s) {
	BST* nou = (BST*)malloc(sizeof(BST));
	nou->inf.cod = s.cod;
	nou->inf.nume = (char*)malloc(sizeof(char*) * (strlen(s.nume) + 1));
	strcpy(nou->inf.nume, s.nume);
	nou->left = NULL;
	nou->right = NULL;
	return nou;
}

BST* inserareBST(BST* rad, student s) {
	if (rad) {
		if (s.cod < rad->inf.cod) {
			rad->left = inserareBST(rad->left, s);
			return rad;
		}
		else if (s.cod > rad->inf.cod) {
			rad->right = inserareBST(rad->right, s);
			return rad;
		}
		else return rad;
	}
	else return creareNod(s);
}

BST* cautareBST(BST* rad, int cod) {
	if (rad) {
		if (rad->inf.cod == cod) return rad;
		else if (rad->inf.cod > cod)
			return cautareBST(rad->left, cod);
		else return cautareBST(rad->right, cod);
	}
	else return NULL;
}
void main() {
	char buffer[256];
	FILE* pFile = fopen("fisier.txt", "r");
	student* vect = (student*)malloc(sizeof(student) * 5);
	if (pFile) {
		char* token;
		char delimitator[] = ".,\n";
		int i = 0;
		while (fgets(buffer, sizeof(buffer), pFile)) {
			student s;
			token = strtok(buffer, delimitator);
			s.cod = atoi(token);
			printf("%s", token);
			token =strtok(NULL, delimitator);
			printf("%s", token);
			s.nume = (char*)malloc(sizeof(char*) * (strlen(token) + 1));
			strcpy(s.nume, token);
			vect[i] = s;
			i++;
		}
	}
	nodLista* stack = NULL;
	nodLista* queueP = NULL; nodLista* queueU = NULL;
	nodLS* LSImpara = NULL;
	nodLS* LSPara = NULL;
	nodLP* LP = NULL;
	heap h;
	BST* rad = NULL;
	h.marime = 5;
	h.vect = (student*)malloc(sizeof(student) * 5);
	hash tabela;
	tabela.marime = 10;
	tabela.vect = (nodLista**)malloc(sizeof(nodLista*) * tabela.marime);
	for (int i = 0; i < tabela.marime; i++)
		tabela.vect[i] = 0;
	for (int i = 0; i < 5; i++) {
		inserare(tabela, vect[i]);
		push(&stack, vect[i]);
		put(&queueP,&queueU, vect[i]);
		h.vect[i] = vect[i];
		rad=inserareBST(rad, vect[i]);
		if (vect[i].cod % 2 == 0)
			inserareLS(&LSImpara, vect[i]);
		else inserareLS(&LSPara, vect[i]);
	}
	for (int i = (h.marime - 1) / 2; i >= 0; i--)
		filtrare(h, i);
	inserareLP(&LP, &LSImpara);
	inserareLP(&LP, &LSPara);
	student stud6;
	stud6.cod = 50;
	stud6.nume = (char*)malloc(sizeof(char*) * (strlen("Adi") + 1));
	strcpy(stud6.nume, "Adi");
	printf("\nHeap---------------\n");
	afisareHeap(h);
	inserareHeap(&h, stud6);
	printf("\nHash---------------\n");
	traversareTabela(tabela);
	printf("\nLOL---------------\n");
	traversareLP(LP);
	printf("\n\n");
	stergere(tabela, 2);
	traversareTabela(tabela);
}