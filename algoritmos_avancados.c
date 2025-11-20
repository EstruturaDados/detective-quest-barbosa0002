#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 20

// ==================== ESTRUTURAS ====================

// Estrutura para representar cada cômodo da mansão (Árvore Binária)
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Estrutura para armazenar pistas coletadas (Árvore BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Estrutura para a tabela hash (pista -> suspeito)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* proximo;
} HashNode;

// Tabela hash global
HashNode* tabelaHash[TAM_HASH];

// ==================== FUNÇÕES DE CRIAÇÃO ====================

/**
 * criarSala() - Cria dinamicamente um cômodo da mansão
 * @param nome: Nome do cômodo
 * @return: Ponteiro para a sala criada
 */
Sala* criarSala(char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/**
 * criarNoPista() - Cria dinamicamente um nó para armazenar uma pista
 * @param pista: Texto da pista
 * @return: Ponteiro para o nó criado
 */
PistaNode* criarNoPista(char* pista) {
    PistaNode* novoNo = (PistaNode*)malloc(sizeof(PistaNode));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novoNo->pista, pista);
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

// ==================== FUNÇÕES DA ÁRVORE BST DE PISTAS ====================

/**
 * inserirPista() - Insere uma pista na árvore BST de forma ordenada
 * @param raiz: Raiz da árvore de pistas
 * @param pista: Pista a ser inserida
 * @return: Nova raiz da árvore
 */
PistaNode* inserirPista(PistaNode* raiz, char* pista) {
    if (raiz == NULL) {
        return criarNoPista(pista);
    }
    
    int comparacao = strcmp(pista, raiz->pista);
    
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se comparacao == 0, a pista já existe, não insere novamente
    
    return raiz;
}

/**
 * exibirPistas() - Exibe todas as pistas em ordem alfabética (in-order)
 * @param raiz: Raiz da árvore de pistas
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("  - %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// ==================== FUNÇÕES DA TABELA HASH ====================

/**
 * funcaoHash() - Calcula o índice hash para uma pista
 * @param pista: Texto da pista
 * @return: Índice na tabela hash
 */
int funcaoHash(char* pista) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++) {
        soma += pista[i];
    }
    return soma % TAM_HASH;
}

/**
 * inserirNaHash() - Insere a associação pista-suspeito na tabela hash
 * @param pista: Texto da pista
 * @param suspeito: Nome do suspeito associado
 */
void inserirNaHash(char* pista, char* suspeito) {
    int indice = funcaoHash(pista);
    
    HashNode* novoNo = (HashNode*)malloc(sizeof(HashNode));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    
    strcpy(novoNo->pista, pista);
    strcpy(novoNo->suspeito, suspeito);
    novoNo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoNo;
}

/**
 * encontrarSuspeito() - Busca o suspeito associado a uma pista
 * @param pista: Texto da pista
 * @return: Nome do suspeito ou NULL se não encontrado
 */
char* encontrarSuspeito(char* pista) {
    int indice = funcaoHash(pista);
    HashNode* atual = tabelaHash[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    
    return NULL;
}

// ==================== FUNÇÃO PARA OBTER PISTA DE UMA SALA ====================

/**
 * obterPistaDaSala() - Retorna a pista associada a um cômodo específico
 * @param nomeSala: Nome do cômodo
 * @return: Texto da pista ou NULL se não houver
 */
char* obterPistaDaSala(char* nomeSala) {
    // Definição estática das pistas por sala
    if (strcmp(nomeSala, "Biblioteca") == 0)
        return "Livro de venenos aberto";
    else if (strcmp(nomeSala, "Cozinha") == 0)
        return "Faca com manchas";
    else if (strcmp(nomeSala, "Quarto do Mordomo") == 0)
        return "Carta ameacadora";
    else if (strcmp(nomeSala, "Sala de Estar") == 0)
        return "Copo com residuos";
    else if (strcmp(nomeSala, "Jardim") == 0)
        return "Pegadas na lama";
    else if (strcmp(nomeSala, "Escritorio") == 0)
        return "Documento rasgado";
    else if (strcmp(nomeSala, "Adega") == 0)
        return "Garrafa quebrada";
    
    return NULL; // Sala sem pista
}

// ==================== FUNÇÃO DE EXPLORAÇÃO ====================

/**
 * explorarSalas() - Permite a navegação interativa pela mansão
 * @param salaAtual: Sala onde o jogador está
 * @param pistasColetadas: Árvore BST com as pistas já coletadas
 * @return: Árvore atualizada com as novas pistas
 */
PistaNode* explorarSalas(Sala* salaAtual, PistaNode* pistasColetadas) {
    if (salaAtual == NULL) {
        printf("Você não pode ir nessa direção!\n\n");
        return pistasColetadas;
    }
    
    printf("\n========================================\n");
    printf("Você está em: %s\n", salaAtual->nome);
    printf("========================================\n");
    
    // Verifica se há pista nesta sala
    char* pista = obterPistaDaSala(salaAtual->nome);
    if (pista != NULL) {
        printf("🔍 Pista encontrada: %s\n", pista);
        pistasColetadas = inserirPista(pistasColetadas, pista);
        printf("✓ Pista adicionada ao seu caderno!\n");
    } else {
        printf("Nenhuma pista encontrada aqui.\n");
    }
    
    // Verifica se é uma folha (fim do caminho)
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf("\nEsta sala não tem mais saídas!\n");
        return pistasColetadas;
    }
    
    // Menu de opções
    char opcao;
    while (1) {
        printf("\nPara onde deseja ir?\n");
        if (salaAtual->esquerda != NULL)
            printf("  [e] Esquerda\n");
        if (salaAtual->direita != NULL)
            printf("  [d] Direita\n");
        printf("  [s] Sair da exploração\n");
        printf("Escolha: ");
        
        scanf(" %c", &opcao);
        
        if (opcao == 'e' && salaAtual->esquerda != NULL) {
            return explorarSalas(salaAtual->esquerda, pistasColetadas);
        } else if (opcao == 'd' && salaAtual->direita != NULL) {
            return explorarSalas(salaAtual->direita, pistasColetadas);
        } else if (opcao == 's') {
            printf("\nVocê decidiu encerrar a exploração.\n");
            return pistasColetadas;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// ==================== FUNÇÃO DE CONTAGEM DE PISTAS POR SUSPEITO ====================

/**
 * contarPistasPorSuspeito() - Conta quantas pistas apontam para um suspeito
 * @param raiz: Raiz da árvore de pistas coletadas
 * @param suspeito: Nome do suspeito
 * @return: Número de pistas que apontam para o suspeito
 */
int contarPistasPorSuspeito(PistaNode* raiz, char* suspeito) {
    if (raiz == NULL) {
        return 0;
    }
    
    int count = 0;
    char* suspeitoAssociado = encontrarSuspeito(raiz->pista);
    
    if (suspeitoAssociado != NULL && strcmp(suspeitoAssociado, suspeito) == 0) {
        count = 1;
    }
    
    return count + 
           contarPistasPorSuspeito(raiz->esquerda, suspeito) + 
           contarPistasPorSuspeito(raiz->direita, suspeito);
}

/**
 * listarPistasPorSuspeito() - Lista todas as pistas que apontam para um suspeito
 * @param raiz: Raiz da árvore de pistas
 * @param suspeito: Nome do suspeito
 */
void listarPistasPorSuspeito(PistaNode* raiz, char* suspeito) {
    if (raiz == NULL) {
        return;
    }
    
    listarPistasPorSuspeito(raiz->esquerda, suspeito);
    
    char* suspeitoAssociado = encontrarSuspeito(raiz->pista);
    if (suspeitoAssociado != NULL && strcmp(suspeitoAssociado, suspeito) == 0) {
        printf("    • %s\n", raiz->pista);
    }
    
    listarPistasPorSuspeito(raiz->direita, suspeito);
}

// ==================== FUNÇÃO DE VERIFICAÇÃO FINAL ====================

/**
 * verificarSuspeitoFinal() - Conduz a fase de julgamento final
 * @param pistasColetadas: Árvore com todas as pistas coletadas
 */
void verificarSuspeitoFinal(PistaNode* pistasColetadas) {
    printf("\n\n╔════════════════════════════════════════╗\n");
    printf("║     HORA DE FAZER SUA ACUSAÇÃO!       ║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    printf("\nPistas coletadas:\n");
    exibirPistas(pistasColetadas);
    
    printf("\n\nSuspeitos disponíveis:\n");
    printf("  1. Mordomo\n");
    printf("  2. Cozinheira\n");
    printf("  3. Jardineiro\n");
    
    char suspeito[50];
    printf("\nQuem você acusa? ");
    scanf(" %[^\n]", suspeito);
    
    int numeroPistas = contarPistasPorSuspeito(pistasColetadas, suspeito);
    
    printf("\n\n════════════════════════════════════════\n");
    printf("RESULTADO DA INVESTIGAÇÃO\n");
    printf("════════════════════════════════════════\n");
    printf("Suspeito acusado: %s\n", suspeito);
    printf("Número de pistas encontradas: %d\n\n", numeroPistas);
    
    if (numeroPistas >= 2) {
        printf("🎉 PARABÉNS, DETETIVE!\n\n");
        printf("Você reuniu evidências suficientes!\n");
        printf("As seguintes pistas incriminam %s:\n\n", suspeito);
        listarPistasPorSuspeito(pistasColetadas, suspeito);
        printf("\nO culpado foi capturado! Caso resolvido! ✓\n");
    } else {
        printf("❌ EVIDÊNCIAS INSUFICIENTES!\n\n");
        printf("Você não coletou pistas suficientes para\n");
        printf("sustentar sua acusação contra %s.\n", suspeito);
        printf("São necessárias pelo menos 2 pistas.\n");
        printf("\nO culpado escapou... Caso não resolvido.\n");
    }
    
    printf("════════════════════════════════════════\n");
}

// ==================== FUNÇÃO PRINCIPAL ====================

int main() {
    // Inicializa a tabela hash
    for (int i = 0; i < TAM_HASH; i++) {
        tabelaHash[i] = NULL;
    }
    
    // Associa pistas a suspeitos na tabela hash
    inserirNaHash("Livro de venenos aberto", "Mordomo");
    inserirNaHash("Faca com manchas", "Cozinheira");
    inserirNaHash("Carta ameacadora", "Mordomo");
    inserirNaHash("Copo com residuos", "Cozinheira");
    inserirNaHash("Pegadas na lama", "Jardineiro");
    inserirNaHash("Documento rasgado", "Mordomo");
    inserirNaHash("Garrafa quebrada", "Jardineiro");
    
    // Cria o mapa da mansão (Árvore Binária)
    Sala* hall = criarSala("Hall de Entrada");
    
    // Lado esquerdo
    hall->esquerda = criarSala("Biblioteca");
    hall->esquerda->esquerda = criarSala("Sala de Estar");
    hall->esquerda->direita = criarSala("Escritorio");
    
    // Lado direito
    hall->direita = criarSala("Cozinha");
    hall->direita->esquerda = criarSala("Quarto do Mordomo");
    hall->direita->direita = criarSala("Jardim");
    hall->direita->direita->direita = criarSala("Adega");
    
    // Inicializa a árvore de pistas coletadas
    PistaNode* pistasColetadas = NULL;
    
    // Apresentação do jogo
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║          BEM-VINDO AO DETECTIVE QUEST         ║\n");
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║                                                ║\n");
    printf("║  Um crime ocorreu na mansão!                  ║\n");
    printf("║  Explore os cômodos, colete pistas e          ║\n");
    printf("║  descubra quem é o culpado!                   ║\n");
    printf("║                                                ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");
    
    // Inicia a exploração
    pistasColetadas = explorarSalas(hall, pistasColetadas);
    
    // Fase de acusação final
    if (pistasColetadas != NULL) {
        verificarSuspeitoFinal(pistasColetadas);
    } else {
        printf("\n\nVocê não coletou nenhuma pista!\n");
        printf("Impossível fazer uma acusação.\n");
    }
    
    printf("\n\nObrigado por jogar Detective Quest!\n\n");
    
    return 0;
}