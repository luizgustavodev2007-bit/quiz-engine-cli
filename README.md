# Quiz Engine CLI

Um jogo de perguntas e respostas interativo rodando diretamente no terminal, desenvolvido em C puro. O projeto foi projetado para demonstrar maturidade técnica em conceitos fundamentais de engenharia de software, como gerenciamento dinâmico de memória, persistência de dados em arquivos e implementação manual de algoritmos de ordenação.

## 🚀 Funcionalidades

- **Banco de Questões Dinâmico:** As perguntas não ficam presas no código. Elas são carregadas e processadas em tempo real a partir de um arquivo externo (`perguntas.txt`).
- **Sistema de Repescagem:** O jogador tem até 2 tentativas por pergunta. Acertos de primeira garantem 10 pontos; acertos de segunda garantem 5 pontos.
- **Persistência de Scores:** As pontuações são salvas automaticamente em um arquivo de histórico local (`ranking.txt`).
- **Ranking Global (Top 5):** O sistema lê o histórico, ordena as maiores pontuações e exibe dinamicamente os 5 melhores jogadores.
- **Interface CLI Robusta:** Tratamento contra entradas inválidas de dados (proteção contra quebras de buffer e caracteres inesperados).

## 🛠️ Conceitos Técnicos Aplicados

Para afastar o design padrão de códigos puramente acadêmicos, o projeto implementa:

1. **Alocação Dinâmica de Memória:** Uso estratégico de `malloc` e `realloc` para redimensionar o vetor do banco de questões na memória conforme o arquivo é lido.
2. **Parsing e Tokenização de Strings:** Uso da função `strtok` e expressões de filtragem no `fscanf` (`" %49[^;];%d\n"`) para ler dados formatados por delimitadores (`|` e `;`).
3. **Algoritmos de Ordenação:** Implementação manual do algoritmo *Bubble Sort* para classificar a estrutura de dados do ranking sem depender de bibliotecas externas de alto nível.
4. **Gerenciamento de Fluxo I/O:** Limpeza manual e segura do buffer do teclado (`stdin`) para evitar pulos de linha crônicos do `scanf`.

## 📁 Estrutura do Arquivo de Perguntas (`perguntas.txt`)

O programa espera um arquivo chamado `perguntas.txt` no mesmo diretório do executável. O formato deve seguir o padrão `Enunciado|Alternativa A|Alternativa B|Alternativa C|Alternativa D|Gabarito`:

```text
Qual das seguintes opcoes NAO e um tipo primitivo em C?|A) int|B) float|C) string|D) char|C
Qual operador e usado para comparar igualdade em C?|A) =|B) ==|C) ===|D) !=|B