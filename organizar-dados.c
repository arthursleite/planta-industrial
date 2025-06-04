#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SENSORES 10
#define NOME_MAX_SENSOR 20
#define NOME_ARQUIVO_SAIDA 30
#define TAMANHO_MAX_LINHA 100
#define MAX_LEITURAS_SENSOR 2000

typedef struct
{
  long timestamp;
  char valor_str[30];
} Leitura;

typedef struct
{
  char nome[NOME_MAX_SENSOR];
  Leitura leituras[MAX_LEITURAS_SENSOR];
  int total;
} SensorData;

int encontrar_ou_criar_sensor(SensorData sensores[], int *total_sensores, const char *nome);
int comparar_leituras(const void *a, const void *b);

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Uso: %s <arquivo de entrada>\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *entrada = fopen(argv[1], "r");
  if (entrada == NULL)
  {
    perror("Erro ao abrir arquivo de entrada");
    return EXIT_FAILURE;
  }

  SensorData sensores[MAX_SENSORES];
  int total_sensores = 0;

  char linha[TAMANHO_MAX_LINHA];
  while (fgets(linha, sizeof(linha), entrada))
  {
    long timestamp;
    char nome_sensor[NOME_MAX_SENSOR];
    char valor[30];

    if (sscanf(linha, "%ld %s %s", &timestamp, nome_sensor, valor) != 3)
    {
      fprintf(stderr, "Linha ignorada (formato inválido): %s", linha);
      continue;
    }

    int idx = encontrar_ou_criar_sensor(sensores, &total_sensores, nome_sensor);

    if (sensores[idx].total >= MAX_LEITURAS_SENSOR)
    {
      fprintf(stderr, "Erro: limite de leituras para o sensor %s foi atingido.\n", nome_sensor);
      exit(EXIT_FAILURE);
    }

    sensores[idx].leituras[sensores[idx].total].timestamp = timestamp;
    strncpy(sensores[idx].leituras[sensores[idx].total].valor_str, valor, sizeof(valor));
    sensores[idx].total++;
  }

  fclose(entrada);

  for (int i = 0; i < total_sensores; i++)
  {
    qsort(sensores[i].leituras, sensores[i].total, sizeof(Leitura), comparar_leituras);

    char nome_arquivo[NOME_ARQUIVO_SAIDA];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "txt/%s.txt", sensores[i].nome);

    FILE *saida = fopen(nome_arquivo, "w");
    if (saida == NULL)
    {
      perror("Erro ao criar arquivo de saída");
      return EXIT_FAILURE;
    }

    for (int j = 0; j < sensores[i].total; j++)
    {
      fprintf(saida, "%ld %s %s\n",
              sensores[i].leituras[j].timestamp,
              sensores[i].nome,
              sensores[i].leituras[j].valor_str);
    }

    fclose(saida);
  }

  printf("Dados organizados com sucesso para %d sensores.\n", total_sensores);
  return EXIT_SUCCESS;
}

int encontrar_ou_criar_sensor(SensorData sensores[], int *total_sensores, const char *nome)
{
  for (int i = 0; i < *total_sensores; i++)
  {
    if (strcmp(sensores[i].nome, nome) == 0)
    {
      return i;
    }
  }
  if (*total_sensores >= MAX_SENSORES)
  {
    fprintf(stderr, "Erro: número máximo de sensores atingido.\n");
    exit(EXIT_FAILURE);
  }

  strncpy(sensores[*total_sensores].nome, nome, NOME_MAX_SENSOR - 1);
  sensores[*total_sensores].nome[NOME_MAX_SENSOR - 1] = '\0';
  sensores[*total_sensores].total = 0;
  return (*total_sensores)++;
}

int comparar_leituras(const void *a, const void *b)
{
  const Leitura *la = (const Leitura *)a;
  const Leitura *lb = (const Leitura *)b;
  if (la->timestamp < lb->timestamp)
    return -1;
  if (la->timestamp > lb->timestamp)
    return 1;
  return 0;
}
