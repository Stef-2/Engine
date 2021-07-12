#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "glfw3.h"

namespace Engine
{

class VertexBuffer
{
    public:
        VertexBuffer();
        VertexBuffer(GLfloat* data);
        ~VertexBuffer();
        VertexBuffer(const VertexBuffer& other);
        VertexBuffer& operator=(const VertexBuffer& other);

    protected:

    private:
};

}
#endif // VERTEXBUFFER_H

