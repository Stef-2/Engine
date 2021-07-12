#include "Obj.h"

Obj::Obj()
{
    //ctor
}

Obj::Obj(char* fileName)
{
    ReadData(fileName);
}

void Obj::ReadData(char* fileName)
{
    std::ifstream input(fileName, std::ifstream::in);

    input.close();
}

Obj::~Obj()
{
    //dtor
}

Obj::Obj(const Obj& other)
{
    //copy ctor
}

Obj& Obj::operator=(const Obj& rhs)
{
    if (this == &rhs) return *this;
    return *this;
}
