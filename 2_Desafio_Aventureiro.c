#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// ==========================
// Struct da peça
// ==========================
typedef struct {
    char nome; 
    int id;
} Peca;

// ==========================
// Fila circular
// ==========================
typedef struct {
    Peca itens[MAX_FILA];
    int inicio, fim, qtd;
} Fila;

// ==========================
// Pilha de reserva
// ==========================
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

        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
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
                    printf("⚠️ Pilha cheia! Não foi possível reservar.\n");
                    // devolve a peça perdida (não volta pra fila)
                }
            }
        } else if (opcao == 3) {
            Peca usada;
            if (pop(&pilha, &usada)) {
                printf("👉 Peça da reserva usada: [%c %d]\n", usada.nome, usada.id);
            } else {
                printf("⚠️ Pilha vazia! Não há peça para usar.\n");
            }
        }

    } while (opcao != 0);

    printf("🎮 Encerrando o jogo. Até a próxima!\n");
    return 0;
}
