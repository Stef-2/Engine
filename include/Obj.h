#ifndef OBJ_H
#define OBJ_H

#include "fstream"
#include "string"

class Obj
{
    public:
        Obj();
        Obj(char* fileName);

        void ReadData(char* fileName);

        ~Obj();
        Obj(const Obj& other);
        Obj& operator=(const Obj& other);

    protected:

    private:
};

#endif // OBJ_H
