#include "spi.hpp"
#include <iostream>

int main(void){
    unsigned char dat[10];
    for(unsigned int i=0;i<10;i++){
        dat[i]=i;
    }
    SpiOpenPort(0,0,8,8000000);
    SpiWriteAndRead(0,dat,10);
    for(unsigned int i=0;i<10;i++){
        std::cout<<dat[i]<<std::endl;
    }
    SpiClosePort(0);
}
