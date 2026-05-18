#ifndef __PIN_CONF
#define __PIN_CONF
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pinlog/pinlog.h"
#include "pinmem/pinmem.h"

typedef enum { INT, FLOAT, BOOL, TEXT, NONE } CONF_V_TYPE;

typedef struct {
  char *k;
  char *v;
  CONF_V_TYPE v_type;
} ConfKV;

typedef struct {
  char *filename;
  int size;
  int columns;
  ConfKV **values;
} Conf;

int parse(const char *filename, Conf* cfg) {
  PIN_DEBUG = true;
  FILE *conf = fopen(filename, "r");
  if (!conf) {
    pinlog(ERROR, strerror(errno));
    return 1;
  }
  fseek(conf, 0, SEEK_END);
  int fp = ftell(conf);
  fseek(conf, 0, SEEK_SET);

  char *data = (char *)pmalloc(fp+1);

  fread(data, 1, fp, conf);
  data[fp] = '\0';
  cfg->filename = (char*)pmalloc(strlen(filename)+sizeof(char));
  strcpy(cfg->filename, filename);
  cfg->size = fp;


  char* tk = strtok(data,"\n");

  int i = 0;
  while(tk != NULL){
    if(tk[0] == '#'){goto NEWTK;}
    char* s = strchr(tk,(int)'=');
    int ix = strcspn(tk,"="); 
    ConfKV c;
    c.k = (char*)pmalloc((ix+1) * sizeof(char));
    c.v = (char*)pmalloc(strlen(&s[1])+ sizeof(char));
    c.v_type = NONE;
    if(cfg->values == NULL){
      cfg->values = (ConfKV**) preallocarray(NULL, 1, sizeof(ConfKV*));
    }else{
      cfg->values = (ConfKV**) preallocarray(cfg->values, i+1, sizeof(ConfKV*));
    }
    cfg->values[i] = (ConfKV*)pmalloc(sizeof(c));
    strncpy(c.k, &tk[0], ix);
    c.k[ix] = '\0';
    if(s[1] == '"'){
      strncpy(c.v, &s[2], strlen(s)-3);
      c.v_type = TEXT;
    }
    else if(strcmp(&s[1], "false") == 0 || strcmp(&s[1], "true") == 0){
      strcpy(c.v, &s[1]);
      c.v_type = BOOL;
    }else if(strspn(&s[1], "0123456789") == strlen(&s[1])){
      strcpy(c.v, &s[1]);
      c.v_type = INT;
    }
    else if(strchrnul(&s[1],'.') == strrchr(&s[1], '.')){ // using strchrnul cause if '.' is not found, one will return the pointer to the null byte and one will return NULL, hence avoiding extra checks
      strcpy(c.v, &s[1]);
      c.v_type = FLOAT;
    }
    else{
      pinlog(ERROR,"parsing error, unknown token at '%s'\n", tk);
      return 2;
    }
    
    memcpy(cfg->values[i], &c, sizeof(c));
    i++;
    
    NEWTK:
    tk = strtok(NULL,"\n");
  }
  cfg->columns = i;
  
  pfree(data);
  pfree(cfg->filename);
  return 0;
}
ConfKV* search(Conf* cfg, char* key){
  for(int i = 0; i < cfg->columns; i++){
    if(strcmp(cfg->values[i]->k, key) == 0){
      return cfg->values[i];
    }
  }
  return NULL;
}

#endif