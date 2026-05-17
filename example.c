#include "pinconf.h"
#include "pinmem/pinmem.h"
int main(){
    Conf conf;
    parse("example.pconf", &conf);


    for (int i = 0; i < conf.columns; i++)
    {
        pfree(conf.values[i]->k);
        pfree(conf.values[i]->v);
        pfree(conf.values[i]);
    }
    return 0;
}