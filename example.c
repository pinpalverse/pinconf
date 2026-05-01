#include "pinconf.h"

int main(){
    Conf conf;
    parse("example.pconf", &conf);


    for (int i = 0; i < conf.columns; i++)
    {
        free(conf.values[i]->k);
        free(conf.values[i]->v);
        free(conf.values[i]);
    }
    return 0;
}