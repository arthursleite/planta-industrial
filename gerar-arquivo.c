#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "util.h"

#define NOME_MAX_SENSOR 10
#define NOME_MAX_TIPO 10
#define MAX_SENSORES 10
#define QUANTIDADE_LEITURAS 2000
#define MAX_TEMPO 20

void separar_sensor_tipo(const char *entrada, char *sensor, char *tipo);
void imprimir_leitura(FILE *file, time_t timestamp, const char *sensor, const char *tipo, double valor);

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    printf("Uso incorreto. Voce deve fornecer o intervalo de tempo e pelo menos um sensor.\n");
    printf("Formato: %s <data_hora_inicial> <data_hora_final> <NOME_DO_SENSOR>:<tipo do dado> [NOME_DO_SENSOR>:<tipo do dado] ...\n", argv[0]);
    return EXIT_FAILURE;
  }

  srand((unsigned int)time(NULL));

  int quant_sensores = argc - 3;
  if (quant_sensores > MAX_SENSORES)
  {
    printf("Número máximo de sensores permitidos: %d.\n", MAX_SENSORES);
    return EXIT_FAILURE;
  }

  char hora_inicial[MAX_TEMPO], hora_final[MAX_TEMPO];
  strncpy(hora_inicial, argv[1], MAX_TEMPO - 1);
  hora_inicial[MAX_TEMPO - 1] = '\0';
  strncpy(hora_final, argv[2], MAX_TEMPO - 1);
  hora_final[MAX_TEMPO - 1] = '\0';

  time_t ts_inicio = converter_para_timestamp(hora_inicial);
  time_t ts_fim = converter_para_timestamp(hora_final);

  for (int i = 0; i < quant_sensores; i++)
  {

    char sensor[NOME_MAX_SENSOR];
    char tipo[NOME_MAX_TIPO];
    char nome_arquivo[30];

    separar_sensor_tipo(argv[i + 3], sensor, tipo);
    snprintf(nome_arquivo, sizeof(nome_arquivo), "txt/%s.txt", sensor);

    FILE *file = fopen(nome_arquivo, "w");
    if (file == NULL)
    {
      perror("Erro ao abrir o arquivo");
      return EXIT_FAILURE;
    }

    for (int j = 0; j < QUANTIDADE_LEITURAS; j++)
    {
      time_t timestamp_aleatorio = gerar_timestamp_aleatorio(ts_inicio, ts_fim);
      double valor = gerar_valor_aleatorio(tipo);
      imprimir_leitura(file, timestamp_aleatorio, sensor, tipo, valor);
    }
    fclose(file);
  }
  return EXIT_SUCCESS;
}

void separar_sensor_tipo(const char *entrada, char *sensor, char *tipo)
{
  const char *ponto = strchr(entrada, ':');
  if (ponto == NULL)
  {
    printf("Erro: string \"%s\" não contém ':'\n", entrada);
    exit(EXIT_FAILURE);
  }

  size_t tamanho_sensor = ponto - entrada;
  if (tamanho_sensor >= NOME_MAX_SENSOR)
  {
    printf("Erro: nome do sensor muito longo\n");
    exit(EXIT_FAILURE);
  }

  strncpy(sensor, entrada, tamanho_sensor);
  sensor[tamanho_sensor] = '\0';

  strncpy(tipo, ponto + 1, NOME_MAX_TIPO - 1);
  tipo[NOME_MAX_TIPO - 1] = '\0';
}

void imprimir_leitura(FILE *file, time_t timestamp, const char *sensor, const char *tipo, double valor)
{
  if (strcmp(tipo, "char") == 0)
  {
    fprintf(file, "%ld %s %c\n", timestamp, sensor, (char)valor);
  }
  else if (strcmp(tipo, "int") == 0)
  {
    fprintf(file, "%ld %s %d\n", timestamp, sensor, (int)valor);
  }
  else if (strcmp(tipo, "float") == 0 || strcmp(tipo, "double") == 0)
  {
    fprintf(file, "%ld %s %.2f\n", timestamp, sensor, valor);
  }
  else
  {
    fprintf(stderr, "Tipo de dado \"%s\" não suportado.\n", tipo);
  }
}