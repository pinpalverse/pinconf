#ifndef __PIN_CONF
#define __PIN_CONF
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pinlog/pinlog.h"
#include "pinmem/pinmem.h"

typedef enum { INT,
               FLOAT,
               BOOL,
               TEXT,
               NONE } CONF_V_TYPE;

typedef struct
{
  char* k;
  char* v;
  CONF_V_TYPE v_type;
} ConfKV;

typedef struct
{
  char* filename;
  int size;
  int columns;
  ConfKV** values;
} Conf;

int parse(const char* filename, Conf* cfg);
ConfKV* search(Conf* cfg, char* key);

#endif
