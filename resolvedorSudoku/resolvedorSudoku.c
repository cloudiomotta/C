#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef _WIN32
#define CLEAR_SCREEN system("cls")
#else
#define CLEAR_SCREEN puts("\x1b[H\x1b[2J")
#endif

int estado1 = 0, estado2 = 1;
void printarBoneco(int *estado)
{
    if (*estado == 0)
    {
        printf("\033[0;32m");
        printf("\n\n \
   __/^^^\\ \n  \
   |O  }| \n \
    < _  / \n \
    \\ | | / \n \
     \\| |/ \n \
      | | \n \
      | | \n \
     /   \\ \n \
    /     \\\n\
   _/       \\_\n\n");
        *estado += 1;
        printf("\033[0m");
    }

    else if (*estado == 1)
    {
        printf("\033[1;33m");
        printf("\n\n \
   __/^^^\\ \n  \
   |O  }| \n \
    < _  / \n \
      | |   \n \
     /| |\\ \n \
    / | | \\\n \
      | | \n \
     /   \\ \n \
    /     \\\n\
   _/       \\_\n\n");
        *estado += 1;
        printf("\033[0m");
    }
    else if (*estado == 2)
    {
        printf("\033[0;35m");
        printf("\n\n \
   __/^^^\\ \n  \
   |O  }| \n \
    < _  / \n \
      | |   \n \
    \\/| |\\/ \n \
      | |  \n \
      | | \n \
     /   \\ \n \
    /     \\\n\
   _/       \\_\n\n");
        *estado = 0;
        printf("\033[0m");
    }
}

void imprimirSudoku(int matriz[9][9], int *boneco1, int *boneco2)
{
    CLEAR_SCREEN;
    printarBoneco(boneco1);
    printf("+-----------------------------+\n");
    int linha = 0, coluna = 0, valor;
    for (linha = 0; linha < 9; linha++)
    {
        printf("| ");
        for (coluna = 0; coluna < 9; coluna++)
        {
            valor = matriz[linha][coluna];
            if ((coluna + 1) % 3 == 0 && coluna < 8)
            {
                if (valor != 0)
                    printf("%d | ", valor);
                else
                    printf("  | ");
            }
            else if (valor != 0)
                printf("%d  ", valor);
            else
                printf("   ");
            if (coluna == 8)
                printf("\b");
        }
        if ((linha + 1) % 3 == 0 && linha < 8)
            printf("|\n|---------+---------+---------|\n");
        else
            printf("|\n");
    }
    printf("+-----------------------------+\n\n");
    printarBoneco(boneco2);
    usleep(400000);
}

/*
    Regras do Sudoku:
    1) as entradas são de 1 a 9
    2) não pode repetir um número em nenhuma linha
    3) não pode repetir um número em nenhuma coluna
    4) não pode repetir um número em nenhuma célula
    A função abaixo retorna 1 se a inserção do valor na posição (linha, coluna) respeita a regra.
    Retorna 0 se a regra não for respeitada.
*/
int podeInserir(int valor, int matriz[9][9], int linha, int coluna)
{   
    // Condição para passar direto em podeIniciar quando o valor checado for zero.
    if (valor==0)
        return 1;
    int comecoLinha, comecoColuna, numLinha, numColuna;

    // Verificando se já tem o valor na coluna
    for (numColuna = 0; numColuna < 9; numColuna++)
        if (matriz[linha][numColuna] == valor)
            return 0;

    // Verificando se já tem o valor na linha
    for (numLinha = 0; numLinha < 9; numLinha++)
        if (matriz[numLinha][coluna] == valor)
            return 0;

    // Verificando se já tem o valor na célula
    comecoLinha = linha / 3 * 3;
    comecoColuna = coluna / 3 * 3;
    for (numLinha = 0; numLinha < 3; numLinha++)
        for (numColuna = 0; numColuna < 3; numColuna++)
            if (matriz[comecoLinha + numLinha][comecoColuna + numColuna] == valor)
                return 0;

    // Se não retornou 0 até aqui, a inserção é possível
    return 1;
}

/*
    Função que funciona como uma função de duas saídas com a utilização de ponteiros no argumento.
    Retorna a próxima posição ao percorrer linhas e colunas.
*/
void proximaPosicao(int linha, int coluna, int *proximaLinha, int *proximaColuna)
{
    if (coluna < 8)
    {
        *proximaLinha = linha;
        *proximaColuna = coluna + 1;
    }
    else
    {
        *proximaLinha = linha + 1;
        *proximaColuna = 0;
    }
}

int sudokuRecursivo(int matriz[9][9], int backup[9][9], int linha, int coluna)
{
    if (linha == 9)
    {
        imprimirSudoku(matriz, &estado1, &estado2);
        return 1;
    }

    int proximaLinha, proximaColuna;
    proximaPosicao(linha, coluna, &proximaLinha, &proximaColuna);
    if (backup[linha][coluna] != 0)
        return sudokuRecursivo(matriz, backup, proximaLinha, proximaColuna);

    for (int valor = 1; valor <= 9; valor++)
    {
        if (podeInserir(valor, matriz, linha, coluna))
        {
            matriz[linha][coluna] = valor;
            imprimirSudoku(matriz, &estado1, &estado2);
            if (sudokuRecursivo(matriz, backup, proximaLinha, proximaColuna))
                return 1;
        }
    }
    matriz[linha][coluna] = 0;
    return 0;
}

int podeIniciar(int matriz[9][9])
{
    int linha, coluna, valor;
    for (linha = 0; linha < 9; linha++)
        for (coluna = 0; coluna < 9; coluna++)
        {
            valor = matriz[linha][coluna];
            matriz[linha][coluna] = 0;
            if (valor < 0 || valor > 9 || !podeInserir(valor, matriz, linha, coluna))
                return 0;
            matriz[linha][coluna] = valor;
        }
    return 1;
}

int main()
{
    int matriz[9][9], backup[9][9];
    FILE *ponteiroArquivo;
    char nomeArquivo[] = "entradaSudoku.in";

    if (ponteiroArquivo = fopen(nomeArquivo,"r"));
    else{
        printf("O programa espera a existência de um arquivo entradaSudoku.in, no mesmo diretório, que usará de entrada contendo 81 números respeitando as regras do Sudoku.");
        return 0;
    }
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++){
            fscanf(ponteiroArquivo, "%d", &matriz[i][j]);
            backup[i][j] = matriz[i][j];
        }
    if (!podeIniciar(matriz)){
        printf("Entrada inválida.\nAs entradas devem respeitar as regras do Sudoku:\n");
        printf("------------------------------------------------------------------\n");
        printf("1) as entradas são de 1 a 9\n2) não pode repetir um número em nenhuma linha\n\
        3) não pode repetir um número em nenhuma coluna\n4) não pode repetir um número em nenhuma célula (quadrado 3x3 limitado pelas barras)\n");
        printf("------------------------------------------------------------------\n\n");
        printf("Finalizando programa...\n");
        return 0;
    }

    sudokuRecursivo(matriz, backup, 0, 0);
    printf("Terminou :)\n");

    return 0;
}