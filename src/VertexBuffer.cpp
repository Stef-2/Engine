#include "VertexBuffer.h"

Engine::VertexBuffer::VertexBuffer()
{
    //ctor
}

Engine::VertexBuffer::~VertexBuffer()
{
    //dtor
}

Engine::VertexBuffer::VertexBuffer(const VertexBuffer& other)
{
    //copy ctor
}

Engine::VertexBuffer& Engine::VertexBuffer::operator=(const VertexBuffer& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
