#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5 // tamanho fixo da fila

// ==========================
// Struct da peça do Tetris
// ==========================
typedef struct {
    char nome; // I, O, T, L
    int id;    // identificador único
} Peca;

// ==========================
// Fila circular
// ==========================
typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int qtd;
} Fila;

// ==========================
// Funções de controle da fila
// ==========================

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
}

// Verifica se fila está cheia
int filaCheia(Fila *f) {
    return f->qtd == MAX;
}

// Verifica se fila está vazia
int filaVazia(Fila *f) {
    return f->qtd == 0;
}

// Gera uma peça aleatória
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Insere uma peça no fim (enqueue)
int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("⚠️ Fila cheia! Não é possível inserir nova peça.\n");
        return 0;
    }
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX;
    f->qtd++;
    return 1;
}

// Remove peça da frente (dequeue)
int dequeue(Fila *f, Peca *removida) {
    if (filaVazia(f)) {
        printf("⚠️ Fila vazia! Não há peça para jogar.\n");
        return 0;
    }
    *removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->qtd--;
    return 1;
}

// Exibe o estado atual da fila
void exibirFila(Fila *f) {
    printf("\n📌 Fila de peças: ");
    if (filaVazia(f)) {
        printf("[vazia]\n");
        return;
    }
    int i, pos = f->inicio;
    for (i = 0; i < f->qtd; i++) {
        Peca p = f->itens[pos];
        printf("[%c %d] ", p.nome, p.id);
        pos = (pos + 1) % MAX;
    }
    printf("\n");
}

// ==========================
// Programa principal
// ==========================
int main() {
    Fila fila;
    inicializarFila(&fila);

    srand(time(NULL));
    int idGlobal = 0;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < MAX; i++) {
        enqueue(&fila, gerarPeca(idGlobal++));
    }

    int opcao;
    do {
        exibirFila(&fila);

        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca removida;
            if (dequeue(&fila, &removida)) {
                printf("👉 Peça jogada: [%c %d]\n", removida.nome, removida.id);
            }
        } else if (opcao == 2) {
            if (enqueue(&fila, gerarPeca(idGlobal++))) {
                printf("✅ Nova peça adicionada!\n");
            }
        }

    } while (opcao != 0);

    printf("🎮 Encerrando o jogo. Até a próxima!\n");
    return 0;
}
