#include "s21_string.h"

#include <stdlib.h>

/**
 * @brief Realization of s21_string.h
 *
 */

/*  NO  ERRORS  */
void *s21_memchr(const void *str, int c, s21_size_t n) {
  char *result = s21_NULL;
  const unsigned char *pstr = str;
  for (int i = 0; i < (int)n; i++) {
    if (*pstr == (unsigned char)c)
      result = (void *)pstr;
    else
      pstr++;
  }
  return result;
}

/*  NO  ERRORS  */
int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int result = 0, flag = 1;
  while (n != 0 && flag) {
    if ((*(unsigned char *)str1 - *(unsigned char *)str2) != 0) {
      result = (*(unsigned char *)str1 - *(unsigned char *)str2);
      flag = 0;
    }
    n--;
    str1++, str2++;
  }
  return result;
}

/*  NO  ERRORS  */
void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  char *buff = dest;
  const char *psrc = src;
  while (n--) {
    *buff++ = *psrc++;
  }
  return dest;
}

/*  NO  ERRORS  */
void *s21_memmove(void *dest, const void *src, s21_size_t n) {
  if (dest < src) {
    char *pdest = (char *)dest;
    char *psrc = (char *)src;
    for (int i = 0; i < (int)n; i++) {
      *(pdest + i) = *(psrc + i);
    }
  } else {
    char *pdest = (char *)dest + n;
    char *psrc = (char *)src + n;
    for (int i = (int)n - 1; i >= 0; i--) {
      *(--pdest) = *(--psrc);
    }
  }
  return dest;
}

/*  NO  ERRORS  */
void *s21_memset(void *str, int c, s21_size_t n) {
  char *pstr = str;
  s21_size_t tmp = 0;
  while (tmp < n) {
    *(pstr + tmp) = (unsigned char)c;
    tmp++;
  }
  return pstr;
}

/*  NO  ERRORS  */
char *s21_strcat(char *dest, const char *src) {
  char *buffer = dest;
  while (*dest != '\0') dest++;
  while (*src != '\0') {
    *dest++ = *src++;
  }
  *dest = '\0';
  return buffer;
}

/*  NO  ERRORS  */
char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *buffer = dest;
  while (*dest != '\0') dest++;
  while (n-- > 0) {
    *dest++ = *src++;
  }
  return buffer;
}

/*  NO  ERRORS  */
char *s21_strchr(const char *str, int c) {
  char *result = s21_NULL, flag = 1;
  while (*str != (unsigned char)c) {
    if (*str++ == '\0') {
      flag = 0;
    }
  }
  if (flag) result = (char *)str;
  return result;
}

/*  NO  ERRORS  */
int s21_strcmp(const char *str1, const char *str2) {
  int flag = 1;
  while (*str1 != '\0' && flag) {
    if (*str1 == *str2) {
      str1++, str2++;
    } else {
      flag = 0;
    }
  }
  return *(unsigned char *)str1 - *(unsigned char *)str2;
}

/*  NO  ERRORS  */
int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int result = 0, flag = 1;
  if (!n) flag = 0;
  while (--n && *str1 != '\0' && *str1 == *str2) {
    str1++;
    str2++;
  }
  if (flag) result = *(char *)str1 - *(char *)str2;
  return result;
}
/*  NO  ERRORS  */
char *s21_strcpy(char *dest, const char *src) {
  char *buff = dest;
  while (*src != '\0') {
    *dest++ = *src++;
  }
  *dest = '\0';
  return buff;
}

/*  NO  ERRORS  */
char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *buff = dest;
  int tmp = 0;
  while (tmp < (int)n) {
    if (*src != '\0') {
      *dest++ = *src++;
    }
    tmp++;
  }
  return buff;
}

/*  NO  ERRORS  */
s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t count = 0;
  while (*str1 != '\0' && !s21_strchr(str2, *str1++)) {
    count++;
  }
  return count;
}

/*  NO  ERRORS  */
char *s21_strerror(int errnum) {
  static char unknown[300];
  staticArrFree(unknown, 300);
#if defined(__APPLE__)
  char str[20] = "Unknown error:";
  LIST(0, 106, str)
#elif (__linux__)
  char str[20] = "Unknown error";
  LIST(0, 134, str)
#endif
}

/*  NO  ERRORS  */
s21_size_t s21_strlen(const char *str) {
  s21_size_t count = 0;
  while (*str != '\0') {
    count++;
    str++;
  }
  return count;
}

/*  NO  ERRORS  */
char *s21_strpbrk(const char *str1, const char *str2) {
  char *r = s21_NULL;
  int stop = 0;
  while (*str1 != '\0' && stop == 0) {
    for (unsigned int i = 0; i < s21_strlen(str2); i++) {
      if (*str1 == *(str2 + i)) {
        stop = 1;
        r = (char *)str1;
      }
    }
    str1++;
  }
  return r;
}

/*  NO  ERRORS  */
char *s21_strrchr(const char *str, int c) {
  char *r = s21_NULL;
  while (*str != '\0') {
    if (*str == (unsigned char)c) {
      r = (char *)str;
    }
    str++;
  }
  return r;
}

/*  NO  ERRORS  */
s21_size_t s21_strspn(const char *str1, const char *str2) {
  s21_size_t count = 0;
  while (*str1 != '\0' && s21_strchr(str2, *str1++)) {
    count++;
  }
  return count;
}

/*  NO  ERRORS  */
char *s21_strstr(const char *haystack, const char *needle) {
  s21_size_t i, j;
  char *result = s21_NULL;
  i = 0;
  if (needle[0] == '\0') {
    result = (char *)haystack;
  }
  while (haystack[i] != '\0') {
    j = 0;
    while (haystack[i + j] != '\0' && haystack[i + j] == needle[j]) {
      if (needle[j + 1] == '\0') {
        result = (char *)(haystack + i);
      }
      ++j;
    }
    ++i;
  }
  return result;
}

/*  NO  ERRORS  */
char *s21_strtok(char *str, const char *delim) {
  static char *res;
  int flag = 0;
  char *ret = s21_NULL;
  if (!str) {
    str = res;
  }
  if (str) {
    while (1 && flag == 0) {
      if (del_check(*str, delim)) {
        str++;
        continue;
      }
      if (*str == '\0') flag = 1;
      break;
    }
    char *inter = str;
    while (1 && flag == 0) {
      if (*str == '\0') {
        res = str;
        ret = inter;
        flag = 1;
      }
      if (del_check(*str, delim)) {
        *str = '\0';
        res = str + 1;
        ret = inter;
        flag = 1;
      }
      str++;
    }
  }
  return ret;
}

int del_check(char c, const char *delim) {
  int res = 0;
  while (*delim != '\0') {
    if (c == *delim) res++;
    delim++;
  }
  return res;
}

/*  NO  ERRORS  */
void *s21_to_lower(const char *str) {
  int i;
  char *buffer = (char *)str;

  i = 0;
  while (str[i]) {
    if (str[i] >= 65 && str[i] <= 90) buffer[i] = str[i] + 32;
    i++;
  }
  return (char *)str;
}

/*  NO  ERRORS  */
void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *buffer;

  if ((s21_strlen(src) < start_index) || (str == s21_NULL)) return (0);

  buffer = (char *)malloc(s21_strlen(src) + s21_strlen(str) + 1);
  s21_strncpy(buffer, (char *)src, start_index);
  buffer[start_index] = '\0';
  s21_strcat(buffer, (char *)str);
  s21_strcat(buffer, (char *)src + start_index);
  return buffer;
}

/*  CODE WORK IS NOT ON TOP  */
void *s21_trim(const char *src, const char *trim_chars) {
  char *buffer = (char *)src;
  char *result = (char *)src;
  if (s21_strlen(trim_chars) < 1) {
    result = s21_NULL;
  } else {
    int i = (int)s21_strlen(buffer) - 1;
    for (; i >= 0 && s21_strchr(trim_chars, buffer[i]) != s21_NULL; i--) {
      buffer[i] = '\0';
    }
    char *buffer2 = (char *)src;
    while (buffer2[0] != '\0' &&
           s21_strchr(trim_chars, buffer2[0]) != s21_NULL) {
      s21_memmove(&buffer2[0], &buffer2[1], s21_strlen(buffer2));
    }
  }
  return result;
}
