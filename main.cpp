#include <iostream>
#include "./log/LOG.h"

int main(int, char **)
{ 
    LOG::getInstance()->init("/home/hyde/log/smain.log",0);  
    return 0;
}
