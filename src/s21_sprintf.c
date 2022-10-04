#include <stdlib.h>
#include <limits.h>

#include "s21_string.h"

/*                      MAIN FUNCTION                    */

int s21_sprintf(char* str, const char* format, ...) {
  const char *tmp = &str[0];
  int situation = 1, result = 0;
  va_list src;
  va_start(src, format);
  while (*format != '\0') {
    struct FormsSprintf forms;
    init_FormsSprintf(&forms);
    int moveFormat = 0, moveStr = 0;
    if (*format != '%' && situation == 1) {
      *str = *format;
      str++, format++;
    } else if (*(++format) == '%') {
      format += 1;
      *str++ = '%';
    } else {
      moveFormat = Parsing(&forms, format);
      if (forms.type == '\0') situation = 0;
    }
    if (forms.type != '\0') {
      moveStr = CheckType(&forms, str, src, &moveStr);
      str += moveStr;
    }
    format += moveFormat;
  }
  va_end(src);
  result = (int)s21_strlen(tmp);
  return result;
}

/*                      READY              */
void init_FormsSprintf(struct FormsSprintf* p) {
  p->minus = 0;
  p->plus = 0;
  p->space = 0;
  p->width = 0;
  p->length = 0;
  p->precision = 0;
  p->type = '\0';
}

/*                      READY              */
int Parsing(struct FormsSprintf* p, const char* format) {
  int moveFormat = 0, tmp = 0;
  while (*format != '%' && *format != '\0' && p->type == '\0') {
    if (!p->width && !p->length && !p->precision && !p->type) {
      tmp = 0;
      tmp += FlagParsing(p, format);
      moveFormat += tmp;
      format += tmp;
    }
    if (!p->width && !p->precision) {
      tmp = 0;
      tmp += WidthParsing(p, format);
      moveFormat += tmp;
      format += tmp;
    }

    if (!p->precision) {
      tmp = 0;
      tmp += precisionParsing(p, format);
      moveFormat += tmp;
      format += tmp;
    }

    if (!p->length) {
      tmp = 0;
      tmp += LengthParsing(p, format);
      moveFormat += tmp;
      format += tmp;
    }

    if (!p->type) {
      tmp = 0;
      tmp += TypeParsing(p, format);
      moveFormat += tmp;
      format += tmp;
    }
  }

  return moveFormat;
}

/*                      READY              */
int FlagParsing(struct FormsSprintf* p, const char* format) {
  const char* formatCopy = format;
  int moveFormat = 0;
  if (*formatCopy == '-' || *formatCopy == '+' || *formatCopy == ' ') {
    if (*formatCopy == '-') {
      p->minus = 1;
      moveFormat += 1;
    }
    if (*format == '+') {
      p->plus = 1;
      moveFormat += 1;
    }
    if (*format == ' ') {
      p->space = 1;
      moveFormat += 1;
    }
  }
  return moveFormat;
}

/*                      READY              */
int WidthParsing(struct FormsSprintf* p, const char* format) {
  const char* formatCopy = format;
  int moveFormat = 0;
  int temp = 0;
  if (*formatCopy >= '1' && *formatCopy <= '9') {
    while (*formatCopy >= '0' && *formatCopy <= '9') {
      temp *= 10;
      temp += *formatCopy - '0';
      formatCopy++, moveFormat++;
    }
    p->width = temp;
  }
  return moveFormat;
}

/*                      READY              */
int LengthParsing(struct FormsSprintf* p, const char* format) {
  const char* formatCopy = format;
  int moveFormat = 0;
  if (*formatCopy == 'h' || *formatCopy == 'l') {
    if (*formatCopy == 'h') {
      p->length = 1;
      moveFormat += 1;
    }
    if (*formatCopy == 'l') {
      p->length = -1;
      moveFormat += 1;
    }
  }
  return moveFormat;
}

/*                      READY              */
int precisionParsing(struct FormsSprintf* p, const char* format) {
  const char* formatCopy = format;
  int moveFormat = 0;
  int temp = 0;
  if (*formatCopy == '.') {
    p->precision = -1;
    // if (*(formatCopy + 1) == '0') {
    //   p->precision = -1;
    //   formatCopy++;
    // }
    moveFormat += 1;
    if (*(formatCopy + 1) >= '1' && *(formatCopy + 1) <= '9') {
      while (*(formatCopy + 1) >= '0' && *(formatCopy + 1) <= '9') {
        temp *= 10;
        temp += *(formatCopy + 1) - '0';
        moveFormat++, formatCopy++;
      }
      p->precision = temp;
    }
  }
  return moveFormat;
}

/*                      READY              */
int TypeParsing(struct FormsSprintf* p, const char* format) {
  const char* formatCopy = format;
  int moveFormat = 0;
  if (*formatCopy == 'd' || *formatCopy == 'i' || *formatCopy == 'c' ||
      *formatCopy == 's' || *formatCopy == 'f' || *formatCopy == 'u') {
    p->type = *formatCopy;
    moveFormat += 1;
  }
  return moveFormat;
}

/*                      READY              */
int CheckType(struct FormsSprintf* p, char* str, va_list args, int* moveStr) {
  if (p->type == 'c') {
    *moveStr = SymboltoString(p, str, args);
  } else if (p->type == 's') {
    *moveStr = StringtoString(p, str, args);
  } else if (p->type == 'd' || p->type == 'i') {
    *moveStr = InttoString(p, str, args);
  } else if (p->type == 'f') {
    *moveStr = FLoattoString(p, str, args);
  } else if (p->type == 'u') {
    *moveStr = UnsignedInttoString(p, str, args);
  }
  return *moveStr;
}

/*                      READY              */
int InttoString(struct FormsSprintf* p, char* str, va_list args) {
  long long int number = 0;
  if (p->length == -1) {
    number = va_arg(args, long int);
  } else if (p->length == 1) {
    number = (short int)number;
    number = va_arg(args, int);
  } else {
    number = (int)number;
    number = va_arg(args, int);
  }
  if (p->precision == -1 && p->width) {
    SpacesZeroesLeft(p, p->width, str, 1);
  } else {
    int difference = 0, spaces = 1;
    int check = -1;
    char* buffer1 = (char*)calloc((p->width + p->precision + 200), sizeof(char));
    char* buffer2 = (char*)calloc((p->width + p->precision + 200), sizeof(char));
    GetStringFromInt(number, buffer1);
    if (p->space == 1) {
      buffer2[0] = ' ';
    }
    if (p->precision > (int)s21_strlen(buffer1)) {
      difference = p->precision - (int)s21_strlen(buffer1);
      SpacesZeroesLeft(p, difference, buffer1, 0);
    }
    if (p->width > (int)s21_strlen(buffer1)) {
      if (p->minus || (p->minus && p->plus)) {
        SpacesZeroesRight(p, buffer1, spaces);
        if ((p->plus && number > 0) || number < 0) {
          buffer1[(int)s21_strlen(buffer1) - 1] = '\0';
        }
      } else {
        if (spaces == 1) {
          if (p->plus && number >= 0) {
            buffer2[0] = '+';
            check = 1;
          }
          if (number < 0) {
            buffer2[0] = '-';
            check = 1;
          }
          s21_strcat(buffer2, buffer1);
          int i = 0;
          s21_size_t j = s21_strlen(buffer2);
          for (; j; j--, i++) {
            buffer1[i] = buffer2[i];
            buffer2[i] = '\0';
          }
        }
        if (p->precision < p->width) {
          difference = p->precision - (int)s21_strlen(buffer1);
          SpacesZeroesLeft(p, difference, buffer1, spaces);
          spaces = 1;
          s21_strcat(buffer2, buffer1);
          int i = 0;
          s21_size_t j = s21_strlen(buffer2);
          for (; j; j--, i++) {
            buffer1[i] = buffer2[i];
            buffer2[i] = '\0';
          }
          difference = p->width - (int)s21_strlen(buffer1);
          SpacesZeroesLeft(p, difference, buffer1, spaces);
        }
      }
    }
    if (p->plus && number >= 0 && check != 1) {
      buffer2[0] = '+';
    }
    if (number < 0 && check != 1) {
      buffer2[0] = '-';
    }
    s21_strcat(buffer2, buffer1);
    s21_strcat(str, buffer2);
    free(buffer1);
    free(buffer2);
  }
  return (int)s21_strlen(str);
}

/*                      READY              */
int FLoattoString(struct FormsSprintf* p, char* str, va_list args) {
  int spaces = 1, difference = 0;
  int check = 1;
  long double number = va_arg(args, double);
  char* buffer1 = (char*)calloc((p->width + p->precision + 200), sizeof(char));
  char* buffer2 = (char*)calloc((p->width + p->precision + 200), sizeof(char));

  if ((p->plus && number >= 0) || number < 0) {
    check = -1;
  }
  GetStringFromFloat(p, number, buffer1);
  if (p->space == 1) {
    buffer2[0] = ' ';
  }
  if (p->width > (int)s21_strlen(buffer1)) {
    if (p->minus || (p->minus && p->plus)) {
      SpacesZeroesRight(p, buffer1, spaces);
      if ((p->plus && number >= 0) || number < 0) {
        buffer1[(int)s21_strlen(buffer1) - 1] = '\0';
      }
    } else {
      if (spaces == 1) {
        if (p->plus && number >= 0) {
          buffer2[0] = '+';
          check = 1;
        }
        if (number < 0) {
          buffer2[0] = '-';
          check = 1;
        }
        s21_strcat(buffer2, buffer1);
        int i = 0;
        s21_size_t j = s21_strlen(buffer2);
        for (; j; j--, i++) {
          buffer1[i] = buffer2[i];
          buffer2[i] = '\0';
        }
      }
      difference = p->width - (int)s21_strlen(buffer1);
      SpacesZeroesLeft(p, difference, buffer1, spaces);
    }
  }
  if (p->plus && number >= 0 && check != 1) {
    buffer2[0] = '+';
  }
  if (number < 0 && check != 1) {
    buffer2[0] = '-';
  }
  s21_strcat(buffer2, buffer1);
  s21_strcat(str, buffer2);
  free(buffer1);
  free(buffer2);
  return (int)s21_strlen(str);
}

/*                      READY              */
int StringtoString(struct FormsSprintf* p, char* str, va_list args) {
  int Spaces = 1, moveStr = 0;
  int SizeOfARG = 0;
  char* buffer = (char*)calloc((p->width + p->precision + 200), sizeof(char));
  if (p->length == -1) {
    wchar_t* strleng = (wchar_t*)va_arg(args, wchar_t*);
    s21_tranc(buffer, strleng);
    SizeOfARG = s21_strlen(buffer);
  } else {
    char* ARG = va_arg(args, char*);
    SizeOfARG = s21_strlen(ARG);
    s21_strcat(buffer, ARG);
  }
  if (p->precision == -1 && p->width) {
    int diff = p->width;
    SpacesZeroesLeft(p, diff, str, Spaces);
    moveStr = (int)s21_strlen(str);
  } else {
    if ((p->precision < SizeOfARG && p->precision) ||
      (p->width < SizeOfARG && p->precision == -1)) {
      Reduceprecision(p, buffer);
    }
    if (p->width > SizeOfARG && p->minus) {
      SpacesZeroesRight(p, str, Spaces);
    }
    if ((p->width > SizeOfARG && !p->minus))  {
      int difference = p->width - (int)s21_strlen(buffer);
      SpacesZeroesLeft(p, difference, buffer, Spaces);
    }
    if (p->width > (int)s21_strlen(buffer) && p->width > p->precision && !p->minus) {
      int difference = p->width - (int)s21_strlen(buffer);
      SpacesZeroesLeft(p, difference, buffer, Spaces);
    }

    if (p->precision != -1) {
      for (int i = 0; i < (int)s21_strlen(buffer); i++) {
        str[i] = buffer[i];
      }
      moveStr = (int)s21_strlen(str);
    }
  }
  free(buffer);
  return moveStr;
}

/*                      READY              */
int SymboltoString(struct FormsSprintf* p, char* str, va_list args) {
  int spaces = 1;
  int moveStr = 0;
  if ((!p->minus && !p->width) || (p->minus && !p->width)) {
    char tmp = (char)va_arg(args, int);
    *str = tmp;
    moveStr += 1;
  } else if (p->minus && p->width) {
    char tmp = (char)va_arg(args, int);
    *str = tmp;
    SpacesZeroesRight(p, str, spaces);
    moveStr = s21_strlen(str);
  } else if (!p->minus && p->width) {
    char buffer[200] = {'\0'};
    buffer[0] = (char)va_arg(args, int);
    int difference = p->width - s21_strlen(buffer);
    SpacesZeroesLeft(p, difference, buffer, spaces);
    s21_strcat(str, buffer);
    moveStr = s21_strlen(str);
  }
  return moveStr;
}

/*                      READY              */
int UnsignedInttoString(struct FormsSprintf* p, char* str, va_list args) {
  unsigned long int number = 0;
  if (p->length == -1) {
    number = va_arg(args, unsigned long int);
  } else if (p->length == 1) {
    number = (unsigned short int)number;
    number = va_arg(args, unsigned int);
  } else {
    number = (unsigned)number;
    number = va_arg(args, unsigned int);
  }
  char* buffer1 = (char*)calloc((p->width + p->precision + 200), sizeof(char));
  char* buffer2 = (char*)calloc((p->width + p->precision + 200), sizeof(char));
  GetStringFromUnsignedInt(number, buffer1);
  int spaces = 1, difference = 0;
  if (p->precision > (int)s21_strlen(buffer1)) {
    spaces = 0;
    difference = p->precision - (int)s21_strlen(buffer1);
    SpacesZeroesLeft(p, difference, buffer1, spaces);
  }
  if (p->width > (int)s21_strlen(buffer1)) {
    if (p->minus || (p->plus && p->minus)) {
      spaces = 1;
      SpacesZeroesRight(p, buffer1, spaces);
    } else {
      if (p->precision < p->width) {
        difference = p->precision - (int)s21_strlen(buffer1);
        SpacesZeroesLeft(p, difference, buffer1, spaces);
        spaces = 1;
        s21_strcat(buffer2, buffer1);
        int i = 0;
        s21_size_t j = s21_strlen(buffer2);
        for (; j; j--, i++) {
          buffer1[i] = buffer2[i];
          buffer2[i] = '\0';
        }
        difference = p->width - (int)s21_strlen(buffer1);
        SpacesZeroesLeft(p, difference, buffer1, spaces);
      }
    }
  }
  s21_strcat(buffer2, buffer1);
  s21_strcat(str, buffer2);
  free(buffer1);
  free(buffer2);
  return (int)s21_strlen(str);
}

/*                      READY              */
void SpacesZeroesRight(struct FormsSprintf* p, char* str, int situation) {
  char* tmp = (char*)calloc((int)p->width - (int)s21_strlen(str), sizeof(char));
  for (int i = 0; i < (int)p->width - (int)s21_strlen(str); i++) {
    if (situation == 1) {
      tmp[i] = ' ';
    }
  }
  s21_strcat(str, tmp);
  free(tmp);
}

/*                      READY              */
void SpacesZeroesLeft(struct FormsSprintf* p, int difference, char* buffer,
                      int situation) {
  char* tmp = (char*)calloc((int)p->width, sizeof(char));
  for (int i = 0; i < difference; i++) {
    if (situation == 1) {
      tmp[i] = ' ';
    } else if (situation == 0) {
      tmp[i] = '0';
    }
  }
  s21_strcat(tmp, buffer);
  int j = 0;
  for (int k = 0; k < (int)s21_strlen(tmp); k++, j++) {
    buffer[k] = tmp[k];
  }
  buffer[j] = '\0';
  free(tmp);
}

/*                      READY              */
void Reduceprecision(struct FormsSprintf* p, char* buffer) {
  buffer[p->precision] = '\0';
}

char* s21_tranc(char* dest, const wchar_t* src) {
  s21_size_t l = s21_strlen(dest);
  int x = 0;
  for (int i = 0; src[i] != '\0'; i++) {
    dest[l + i] = src[i];
    x++;
  }
  dest[l + x] = '\0';
  return dest;
}

/*                      READY              */
void GetStringFromInt(long int number, char* str) {
  int flag_edge = 0;
  int f = 0;
  if (number < 0) {
    if (number == LONG_MIN || number == INT_MIN) {
      f = 1;
      number += 1;
      number = (-1) * (number);
    } else {
      number = (-1) * (number);
    }
    long int delitel = 0;
    long int j = number;
    for (delitel = 1; j >= 10; j /= 10) delitel *= 10;
    for (; delitel > 0; delitel /= 10) {
      *str++ = '0' + number / delitel;
      if (f == 1 && flag_edge == 0 && delitel <= 10) {
        number += 1;
        flag_edge = 1;
      } else {
        number %= delitel;
      }
      number %= delitel;
    }
    *str = '\0';
  } else {
    long int delitel = 0;
    long int j = number;
    for (delitel = 1; j >= 10; j /= 10) {
      delitel *= 10;
    }
    for (; delitel > 0; delitel /= 10) {
      *str++ = 48 + number / delitel;
      number %= delitel;
    }
    *str = '\0';
  }
}

/*                      READY              */
void GetStringFromUnsignedInt(unsigned long int number, char* str) {
  long unsigned delitel = 0, j = number;
  for (delitel = 1; j >= 10; j /= 10) delitel *= 10;
  for (; delitel > 0; delitel /= 10) {
    *str++ = 48 + number / delitel;
    number %= delitel;
  }
  *str = '\0';
}

/*                      READY              */
void GetStringFromFloat(struct FormsSprintf* p, long double number, char* str) {
  int flag = 0, precision_count = 0;
  long double number_fixed = number, Intn;
  if (number == s21_INF || number == -s21_INF) {
    if (number < 0) {
      number = fabsl(number);
    }
    char buffer[100] = "inf\0";
    Inf(str, buffer);
  } else {
      if (number < 0) {
      number_fixed = number * (-1);
    }
    if (p->precision == 0) {
      p->precision = 6;
    }
    if (p->precision == -1) {
      flag = 1;
      number_fixed = round(number_fixed);
    }
    s21_int64_t number_fixed_int = (s21_int64_t)number_fixed;
    s21_int64_t tmp = number_fixed_int;
    s21_int64_t delitel = 1;
    while (tmp >= 10) {
      delitel *= 10;
      tmp /= 10;
    }
    for (; delitel > 0; delitel /= 10) {
      *str++ = 48 + (int)(number_fixed_int / delitel);
      number_fixed_int %= delitel;
    }
    long double number_afterpoint = modfl(number_fixed, &Intn);
    if (flag == 0) {
      *str++ = '.';
      if (p->precision < 15) {
        long double e = 1 / pow(10, p->precision + 1);
        number_fixed = MyRound(number_afterpoint, p->precision);
        number_fixed += e;
        while (precision_count++ < p->precision) {
          *str++ = (s21_int64_t)(48 + ((s21_int64_t)(number_fixed * 10) + 0.5));
          number_fixed *= 10;
          number_fixed = modfl(number_fixed, &Intn);
        }
        *str++ = '\0';
      } else {
        number_afterpoint = modfl(number_fixed, &Intn);
        while (precision_count++ < p->precision) {
          *str++ = (s21_int64_t)(48 + ((s21_int64_t)(number_afterpoint * 10) + 0.5));
          number_afterpoint *= 10;
          number_afterpoint = modfl(number_afterpoint, &Intn);
        }
      }
    }
  }
}

long double MyRound(long double number, int precision) {
  long double result = round((number)*pow(10, precision)) / pow(10, precision);
  return result;
}

char* Inf(char *str, char *buff) {
  int count = 0;
  for (int i = 0; buff[i] != '\0'; i++, count++) {
    str[i] = buff[i];
  }
  str[count] = '\0';
  return &str[count];
}
