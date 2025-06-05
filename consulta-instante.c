#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "util.h"

#define MAX_LINHAS 2000

int busca_binaria(long timestamps[], int tamanho, long x);
int buscar_leitura(const char *nome_arquivo, long x);

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("Uso incorreto. Voce deve fornecer o intervalo de tempo e pelo menos um sensor.\n");
    printf("Formato: %s <SENSOR> <data_hora>\n", argv[0]);
    return EXIT_FAILURE;
  }

  char sensor[10];
  char data_hora[20];
  strncpy(sensor, argv[1], sizeof(sensor) - 1);
  sensor[sizeof(sensor) - 1] = '\0';
  strncpy(data_hora, argv[2], sizeof(data_hora) - 1);
  data_hora[sizeof(data_hora) - 1] = '\0';

  char nome_arquivo[20];
  snprintf(nome_arquivo, sizeof(nome_arquivo), "txt/%s.txt", sensor);
  printf("Buscando no arquivo: %s\n", nome_arquivo);
  time_t ts_desejado = converter_para_timestamp(data_hora);

  buscar_leitura(nome_arquivo, ts_desejado);
}

int buscar_leitura(const char *nome_arquivo, long x)
{
  FILE *arquivo = fopen(nome_arquivo, "r");
  if (!arquivo)
  {
    perror("Erro ao abrir o arquivo");
    return EXIT_FAILURE;
  }

  long timestamps[MAX_LINHAS];
  char sensores[MAX_LINHAS][20], valores[MAX_LINHAS][20];
  int tamanho = 0;

  while (fscanf(arquivo, "%ld %s %s", &timestamps[tamanho], sensores[tamanho], valores[tamanho]) == 3)
  {
    tamanho++;
  }
  fclose(arquivo);
  if (tamanho == 0)
  {
    printf("Arquivo vazio ou invalido.\n");
    return 0;
  }
  int indice = busca_binaria(timestamps, tamanho, x);
  if (indice != -1)
  {
    printf("Registro mais proximo encontrado:\n");
    printf("Timestamp: %ld\nSensor: %s\nValor: ", timestamps[indice], sensores[indice]);

    if (isdigit(valores[indice][0]))
    {
      if (strchr(valores[indice], '.') != NULL)
      {
        printf("%.2f\n", atof(valores[indice]));
      }
      else
      {
        printf("%d\n", atoi(valores[indice]));
      }
    }
    else
    {
      printf("%c\n", valores[indice][0]);
    }

    return 1;
  }

  printf("Nenhum registro encontrado.\n");
  return 0;
}

int busca_binaria(long timestamps[], int tamanho, long x)
{
  int esquerda = 0, direita = tamanho - 1;
  int melhor_posicao = -1;

  while (esquerda <= direita)
  {
    int meio = esquerda + (direita - esquerda) / 2;

    if (timestamps[meio] == x)
    {
      return meio;
    }

    if (timestamps[meio] < x)
    {
      esquerda = meio + 1;
    }
    else
    {
      direita = meio - 1;
    }

    if (melhor_posicao == -1 || labs(timestamps[meio] - x) < labs(timestamps[melhor_posicao] - x))
    {
      melhor_posicao = meio;
    }
  }

  return melhor_posicao;
}