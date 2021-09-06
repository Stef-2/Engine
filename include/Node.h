#ifndef NODE_H
#define NODE_H

#include "glm/glm.hpp"

#include "string"
#include "vector"

namespace Engine
{

    // scene Node, containing basic transformation and arbitrary templated data
    // implemented as a hierarchal structure with each node holding information about its parent and children
    template <typename T>
    class Node
    {
    public:
        Node();
        Node(std::string name, glm::mat4 transform);
        Node(std::string name, glm::mat4 transform, T data);
        Node(Node* parent, std::vector<Node*> children, std::string name, glm::mat4 transform, bool isLeaf);

        Node* GetParent();
        std::vector<Node*>& GetChildren();
        std::vector<T>& GetData();
        std::string GetName();
        glm::mat4& GetTransform();

        void SetParent(Node* parent);
        void SetChildren(std::vector<Node*> vector);
        void SetName(std::string name);
        void SetTransform(glm::mat4 transform);
        void AddChild(Node* child);
        void AddData(T data);

    private:
        Node* parent;
        std::vector<Node*> children;
        std::string name;

        glm::mat4 transform;

        std::vector<T> data;
    };

    // templated class, need to define member function implementations in this file

    template <typename T>
    Node<T>::Node()
    {
        this->parent = {};
        this->children = {};
        this->name = {};
        this->transform = {};
        this->data = {};
    }

    template <typename T>
    Node<T>::Node(std::string name, glm::mat4 transform)
    {
        this->parent = {};
        this->children = {};
        this->name = name;
        this->transform = transform;
        this->data = {};
    }

    template <typename T>
    Node<T>::Node(std::string name, glm::mat4 transform, T data)
    {
        this->parent = {};
        this->children = {};
        this->name = name;
        this->transform = transform;
        this->data.push_back(data);
    }

    template <typename T>
    Node<T>::Node(Node* parent, std::vector<Node*> children, std::string name, glm::mat4 transform, bool isLeaf)
    {
        this->parent = parent;
        this->children = children;
        this->name = name;
        this->transform = transform;
        this->data = {};
    }

    template <typename T>
    Engine::Node<T>* Engine::Node<T>::GetParent()
    {
        return this->parent;
    }

    template <typename T>
    std::vector<Engine::Node<T>*>& Engine::Node<T>::GetChildren()
    {
        return this->children;
    }

    template <typename T>
    std::string Engine::Node<T>::GetName()
    {
        return this->name;
    }

    template <typename T>
    glm::mat4& Engine::Node<T>::GetTransform()
    {
        return this->transform;
    }

    template <typename T>
    std::vector<T>& Engine::Node<T>::GetData()
    {
        return this->data;
    }

    template <typename T>
    void Engine::Node<T>::SetParent(Node* parent)
    {
        this->parent = parent;
    }

    template <typename T>
    void Engine::Node<T>::SetChildren(std::vector<Node*> children)
    {
        this->children = children;
    }

    template <typename T>
    void Engine::Node<T>::SetName(std::string name)
    {
        this->name = name;
    }

    template <typename T>
    void Engine::Node<T>::SetTransform(glm::mat4 transform)
    {
        this->transform = transform;
    }

    template <typename T>
    void Engine::Node<T>::AddData(T data)
    {
        this->data.push_back(data);
    }

    template <typename T>
    void Engine::Node<T>::AddChild(Node* child)
    {
        this->children.push_back(child);
    }
}

#endif //  NODE_H
