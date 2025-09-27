#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// ==========================
// Structs das peças e estruturas
// ==========================
typedef struct {
    char nome; 
    int id;
} Peca;

typedef struct {
    Peca itens[MAX_FILA];
    int inicio, fim, qtd;
} Fila;

typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// ==========================
// Funções da fila
// ==========================
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
}

int filaCheia(Fila *f) { return f->qtd == MAX_FILA; }
int filaVazia(Fila *f) { return f->qtd == 0; }

int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->qtd++;
    return 1;
}

int dequeue(Fila *f, Peca *removida) {
    if (filaVazia(f)) return 0;
    *removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->qtd--;
    return 1;
}

Peca* acessarPosFila(Fila *f, int pos) {
    if (pos < 0 || pos >= f->qtd) return NULL;
    int idx = (f->inicio + pos) % MAX_FILA;
    return &f->itens[idx];
}

void exibirFila(Fila *f) {
    printf("Fila de peças: ");
    if (filaVazia(f)) {
        printf("[vazia]\n");
        return;
    }
    int pos = f->inicio;
    for (int i = 0; i < f->qtd; i++) {
        printf("[%c %d] ", f->itens[pos].nome, f->itens[pos].id);
        pos = (pos + 1) % MAX_FILA;
    }
    printf("\n");
}

// ==========================
// Funções da pilha
// ==========================
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) { return p->topo == MAX_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

int push(Pilha *p, Peca item) {
    if (pilhaCheia(p)) return 0;
    p->itens[++p->topo] = item;
    return 1;
}

int pop(Pilha *p, Peca *removida) {
    if (pilhaVazia(p)) return 0;
    *removida = p->itens[p->topo--];
    return 1;
}

Peca* topoPilha(Pilha *p) {
    if (pilhaVazia(p)) return NULL;
    return &p->itens[p->topo];
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[vazia]\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

// ==========================
// Gerador de peças
// ==========================
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ==========================
// Programa principal
// ==========================
int main() {
    srand(time(NULL));
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int idGlobal = 0;

    // Preenche a fila inicialmente
    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(&fila, gerarPeca(idGlobal++));
    }

    int opcao;
    do {
        printf("\n=== ESTADO ATUAL ===\n");
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca jogada;
            if (dequeue(&fila, &jogada)) {
                printf("👉 Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                enqueue(&fila, gerarPeca(idGlobal++));
            }
        } else if (opcao == 2) {
            Peca reservada;
            if (dequeue(&fila, &reservada)) {
                if (push(&pilha, reservada)) {
                    printf("✅ Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                    enqueue(&fila, gerarPeca(idGlobal++));
                } else {
                    printf("⚠️ Pilha cheia! Peça [%c %d] descartada.\n", reservada.nome, reservada.id);
                }
            }
        } else if (opcao == 3) {
            Peca usada;
            if (pop(&pilha, &usada)) {
                printf("👉 Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
            } else {
                printf("⚠️ Pilha vazia! Não há peça para usar.\n");
            }
        } else if (opcao == 4) {
            Peca *frente = acessarPosFila(&fila, 0);
            Peca *topo = topoPilha(&pilha);
            if (frente && topo) {
                Peca temp = *frente;
                *frente = *topo;
                *topo = temp;
                printf("🔄 Troca realizada entre frente da fila e topo da pilha!\n");
            } else {
                printf("⚠️ Não foi possível trocar (verifique fila/pilha).\n");
            }
        } else if (opcao == 5) {
            if (fila.qtd >= 3 && pilha.topo >= 2) {
                for (int i = 0; i < 3; i++) {
                    Peca *pFila = acessarPosFila(&fila, i);
                    Peca *pPilha = &pilha.itens[pilha.topo - i];
                    Peca temp = *pFila;
                    *pFila = *pPilha;
                    *pPilha = temp;
                }
                printf("🔄 Troca múltipla realizada entre fila e pilha!\n");
            } else {
                printf("⚠️ Não foi possível realizar a troca múltipla.\n");
            }
        }

    } while (opcao != 0);

    printf("🎮 Encerrando o jogo. Até a próxima!\n");
    return 0;
}
