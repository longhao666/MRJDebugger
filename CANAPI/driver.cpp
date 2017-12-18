#include "driver.h"

Driver::Driver() : errorTxt("No Error")
{

}

Driver::~Driver()
{

}

void Driver::GetErrorText(std::string &Buffer)
{
    Buffer = errorTxt;
}
