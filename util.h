#ifndef UTIL_H
#define UTIL_H

#include <time.h>

time_t converter_para_timestamp(const char *data_hora);
time_t gerar_timestamp_aleatorio(time_t timestamp_inicial, time_t timestamp_final);
double gerar_valor_aleatorio(const char *tipo);
int eh_ano_bissexto(int ano);
int eh_data_valida(int ano, int mes, int dia, int hora, int min, int seg);
int eh_tipo_valido(const char *tipo);

#endif