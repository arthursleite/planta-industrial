#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "util.h"

#define TIME_T_INVALIDO ((time_t)0)

time_t converter_para_timestamp(const char *data_hora)
{
  struct tm t = {0};

  int ano, mes, dia, hora, min, seg;

  if (sscanf(data_hora, "%d-%d-%dT%d:%d:%d", &ano, &mes, &dia, &hora, &min, &seg) != 6)
  {
    printf("Formato de data/hora invalido. Use YYYY-MM-DDTHH:MM:SS\n");
    return TIME_T_INVALIDO;
  }
  if (!eh_data_valida(ano, mes, dia, hora, min, seg))
  {
    return TIME_T_INVALIDO;
  }
  t.tm_year = ano - 1900;
  t.tm_mon = mes - 1;
  t.tm_mday = dia;
  t.tm_hour = hora;
  t.tm_min = min;
  t.tm_sec = seg;
  t.tm_isdst = -1;
  time_t timestamp = mktime(&t);
  return timestamp;
}

time_t gerar_timestamp_aleatorio(time_t timestamp_inicial, time_t timestamp_final)
{
  if (timestamp_inicial == -1 || timestamp_final == -1 || timestamp_inicial >= timestamp_final)
  {
    printf("Data invalida.\n");
    return TIME_T_INVALIDO;
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

int eh_ano_bissexto(int ano)
{
  return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

int eh_data_valida(int ano, int mes, int dia, int hora, int min, int seg)
{
  int dias_no_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  if (ano < 1900)
    return 0;
  if (mes < 1 || mes > 12)
    return 0;

  if (mes == 2 && eh_ano_bissexto(ano))
  {
    dias_no_mes[1] = 29;
  }

  if (dia < 1 || dia > dias_no_mes[mes - 1])
    return 0;
  if (hora < 0 || hora > 23)
    return 0;
  if (min < 0 || min > 59)
    return 0;
  if (seg < 0 || seg > 59)
    return 0;

  return 1;
}

int eh_tipo_valido(const char *tipo)
{
  const char *tipos_validos[] = {"int", "char", "float", "double"};
  for (int i = 0; i < 4; i++)
  {
    if (strcmp(tipo, tipos_validos[i]) == 0)
    {
      return 1;
    }
  }
  printf("Erro: tipo \"%s\" invalido. Tipos validos: int, char, float, double\n", tipo);
  return 0;
}