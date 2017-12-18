#include "module.h"

Module::Module(uint16_t ID, Driver *d) :
    ID(ID),
    busDriver(d)
{

}

Module::~Module()
{

}

