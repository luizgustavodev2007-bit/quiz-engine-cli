#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TENTATIVAS 2
#define FILE_PERGUNTAS "perguntas.txt"
#define FILE_RANKING "ranking.txt"

typedef struct {
    char enunciado[200];
    char alternativas[4][100];
    char respostaCorreta;
} Questao;

typedef struct {
    char nome[50];
    int pontuacao;
} Jogador;

void flush_in() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char obter_resposta_validada() {
    char ch;
    while (1) {
        printf("Sua resposta (A, B, C ou D): ");
        flush_in();
        if (scanf("%c", &ch) == 1) {
            ch = toupper(ch);
            if (ch >= 'A' && ch <= 'D') return ch;
        }
        printf("[!] Opcao invalida. Use apenas A, B, C ou D.\n");
    }
}

Questao* carregar_banco_questoes(int *total) {
    FILE *f = fopen(FILE_PERGUNTAS, "r");
    if (!f) return NULL;

    int cap = 4;
    int qtd = 0;
    Questao *lista = malloc(cap * sizeof(Questao));
    if (!lista) {
        fclose(f);
        return NULL;
    }

    char linha[600];
    while (fgets(linha, sizeof(linha), f)) {
        if (qtd >= cap) {
            cap *= 2;
            Questao *tmp = realloc(lista, cap * sizeof(Questao));
            if (!tmp) {
                free(lista);
                fclose(f);
                return NULL;
            }
            lista = tmp;
        }

        char *tok = strtok(linha, "|");
        if (!tok) continue;
        strcpy(lista[qtd].enunciado, tok);

        int i;
        for (i = 0; i < 4; i++) {
            tok = strtok(NULL, "|");
            if (tok) strcpy(lista[qtd].alternativas[i], tok);
        }

        tok = strtok(NULL, "|");
        if (tok) {
            while (*tok == ' ' || *tok == '\t') tok++;
            lista[qtd].respostaCorreta = toupper(tok[0]);
        }
        qtd++;
    }

    fclose(f);
    *total = qtd;
    return lista;
}

void salvar_score(const char *nome, int pontos) {
    FILE *f = fopen(FILE_RANKING, "a");
    if (!f) return;
    fprintf(f, "%s;%d\n", nome, pontos);
    fclose(f);
}

void exibir_instrucoes() {
    printf("\n=== COMO FUNCIONA O QUIZ ===\n");
    printf("1. O jogo carregara uma serie de perguntas de multipla escolha.\n");
    printf("2. Para cada pergunta, voce tem ate %d tentativas para acertar.\n", MAX_TENTATIVAS);
    printf("3. Sistema de Pontuacao:\n");
    printf("   - Acerto na 1ª tentativa: +10 pontos\n");
    printf("   - Acerto na 2ª tentativa: +5 pontos\n");
    printf("   - Se errar ambas, voce nao pontua e o quiz avanca.\n");
    printf("4. Suas pontuacoes sao salvas e voce pode disputar o TOP 5 no Ranking.\n");
    printf("============================\n");
    printf("Pressione ENTER para voltar ao menu...");
    flush_in();
    getchar(); // Aguarda o input para não sumir com o texto da tela
}

void exibir_ranking() {
    FILE *f = fopen(FILE_RANKING, "r");
    if (!f) {
        printf("\nNenhum registro encontrado no ranking ainda.\n");
        return;
    }

    Jogador ranking[100];
    int n = 0;

    while (fscanf(f, " %49[^;];%d\n", ranking[n].nome, &ranking[n].pontuacao) == 2) {
        n++;
        if (n >= 100) break;
    }
    fclose(f);

    if (n == 0) return;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (ranking[j].pontuacao < ranking[j + 1].pontuacao) {
                Jogador tmp = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = tmp;
            }
        }
    }

    printf("\n=== RANKING GLOBAL (TOP 5) ===\n");
    int topo = n > 5 ? 5 : n;
    for (int i = 0; i < topo; i++) {
        printf("%d. %-20s - %d pts\n", i + 1, ranking[i].nome, ranking[i].pontuacao);
    }
    printf("==============================\n");
}

void rodar_quiz(const char *nome, Questao *questoes, int total) {
    int score = 0;

    printf("\n-> Iniciando quiz para: %s\n", nome);

    for (int i = 0; i < total; i++) {
        int tentativa = 1;
        int acertou = 0;

        printf("\n[%d/%d] %s\n", i + 1, total, questoes[i].enunciado);
        for (int j = 0; j < 4; j++) {
            printf("  %s\n", questoes[i].alternativas[j]);
        }

        while (tentativa <= MAX_TENTATIVAS && !acertou) {
            char resp = obter_resposta_validada();

            if (resp == questoes[i].respostaCorreta) {
                acertou = 1;
                if (tentativa == 1) {
                    score += 10;
                    printf("[+] Correto! (+10 pts)\n");
                } else {
                    score += 5;
                    printf("[+] Correto na segunda tentativa! (+5 pts)\n");
                }
            } else {
                if (tentativa < MAX_TENTATIVAS) {
                    printf("[-] Resposta incorreta. Voce tem mais 1 chance.\n");
                } else {
                    printf("[-] Errado. Resposta correta: %c\n", questoes[i].respostaCorreta);
                }
                tentativa++;
            }
        }
    }

    printf("\n>> Fim de jogo! Voce fez %d pontos.\n", score);
    salvar_score(nome, score);
}

int main() {
    char nome[50];
    int opcao;
    int total_questoes = 0;

    Questao *banco = carregar_banco_questoes(&total_questoes);
    if (!banco || total_questoes == 0) {
        printf("[Erro] Falha ao carregar arquivo de perguntas (%s).\n", FILE_PERGUNTAS);
        return 1;
    }

    printf("=== SISTEMA DE QUIZ v2.1 ===\n");
    printf("Digite seu nome de usuario: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Jogar Quiz\n");
        printf("2. Como Funciona (Instrucoes)\n");
        printf("3. Ver Ranking\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            opcao = 0;
            flush_in();
        }

        switch (opcao) {
            case 1:
                rodar_quiz(nome, banco, total_questoes);
                break;
            case 2:
                exibir_instrucoes();
                break;
            case 3:
                exibir_ranking();
                break;
            case 4:
                printf("\nSaindo... Ate logo, %s.\n", nome);
                break;
            default:
                printf("[!] Opcao invalida.\n");
        }
    } while (opcao != 4);

    free(banco);
    return 0;
}