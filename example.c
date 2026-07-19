#include <pinmem/pinmem.h>

#include "include/pinconf/pinconf.h"
int main() {
  Conf conf;
  if (parse("example.pconf", &conf)) return 1;

  for (int i = 0; i < conf.columns; i++) {
    pfree(conf.values[i]->k);
    pfree(conf.values[i]->v);
    pfree(conf.values[i]);
  }
  pfree(conf.filename);
  return 0;
}
