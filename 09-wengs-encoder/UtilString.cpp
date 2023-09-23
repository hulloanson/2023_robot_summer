#include <string.h>

int indexOf2(char *str, char c)
{
  int len = strlen(str);
  int i = 0;
  while (*str++ != c)
  {
    i++;
    if (i >= len)
      return -1;
  }
  return i;
}

void tokenize2(char *str, char t, char *t1, char *t2)
{
  char *c = t1;
  *t1 = '\0';
  *t2 = '\0';
  while (*str != '\0')
  {
    if (*str != t)
    {
      *c++ = *str++;
      continue;
    }

    *(c + 1) = '\0';
    c = t2;
    *c = '\0';
    str++;
  }

  *(c + 1) = '\0';
}

int atoi2(char *str)
{
  int result;
  int puiss;

  result = 0;
  puiss = 1;
  while (('-' == (*str)) || ((*str) == '+'))
  {
    if (*str == '-')
      puiss = puiss * -1;
    str++;
  }
  while ((*str >= '0') && (*str <= '9'))
  {
    result = (result * 10) + ((*str) - '0');
    str++;
  }
  return (result * puiss);
}