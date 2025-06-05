#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "util.h"

time_t converter_para_timestamp(const char *data_hora)
{
  struct tm t = {0};

  int ano, mes, dia, hora, min, seg;

  if (sscanf(data_hora, "%d-%d-%dT%d:%d:%d", &ano, &mes, &dia, &hora, &min, &seg) != 6)
  {
    printf("Formato de data/hora invalido. Use YYYY-MM-DDTHH:MM:SS\n");
    return (time_t)-1;
  }
  t.tm_year = ano - 1900;
  t.tm_mon = mes - 1;
  t.tm_mday = dia;
  t.tm_hour = hora;
  t.tm_min = min;
  t.tm_sec = seg;
  t.tm_isdst = -1;
  time_t timestamp = mktime(&t);
  if (timestamp == -1)
  {
    printf("Data inválida. Tente novamente.\n");
  }
  return timestamp;
}

time_t gerar_timestamp_aleatorio(time_t timestamp_inicial, time_t timestamp_final)
{
  if (timestamp_inicial == -1 || timestamp_final == -1 || timestamp_inicial >= timestamp_final)
  {
    printf("Data inválida.\n");
    return -1;
  }

  return timestamp_inicial + rand() % (timestamp_final - timestamp_inicial + 1);
}

double gerar_valor_aleatorio(const char *tipo)
{
  if (strcmp(tipo, "char") == 0)
  {
    return 'A' + (rand() % 26);
  }
  else if (strcmp(tipo, "int") == 0)
  {
    return rand() % 1000;
  }
  else if (strcmp(tipo, "float") == 0 || strcmp(tipo, "double") == 0)
  {
    return ((double)rand() / RAND_MAX) * 100.0;
  }
  return 0.0;
}