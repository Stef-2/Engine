#ifndef NODE_H
#define NODE_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "functional"
#include "string"
#include "vector"

namespace Engine
{
    // keyframe struct for vector types
    struct VectorKeyFrame
    {
        glm::vec3 value;
        double timestamp;
    };

    // keyframe struct for quaternion types
    struct QuaternionKeyFrame
    {
        glm::quat value;
        double timestamp;
    };

    // scene Node, containing basic transformation, animation keyframes and arbitrary templated data
    // implemented as a hierarchal structure with each node holding information about its parent and children
    template <typename T>
    class Node
    {
    public:
        Node<T>();
        Node<T>(std::string name, glm::mat4 transform);
        Node<T>(std::string name, glm::mat4 transform, T data);
        Node<T>(Node<T>* parent, std::vector<Node<T>*> children, std::string name, glm::mat4 transform);

        Node<T>* GetRootNode();
        Node<T>* GetParent();
        std::vector<Node<T>*>& GetChildren();
        std::vector<T>& GetData();
        std::string GetName();
        glm::mat4& GetTransform();

        std::vector<VectorKeyFrame>& GetPositionKeyFrames();
        std::vector<QuaternionKeyFrame>& GetRotationKeyFrames();
        std::vector<VectorKeyFrame>& GetScaleKeyFrames();

        void SetParent(Node<T>* parent);
        void SetChildren(std::vector<Node<T>*> vector);
        void SetName(std::string name);
        void SetTransform(glm::mat4 transform);
        void AddChild(Node<T>* child);
        void AddData(T data);

        // recursively deletes everything above this node, making this node the new root
        void DeleteAbove();
        // recursively deletes everything below this node, making this node a leaf
        void DeleteBelow();

        void SetPositionKeyFrames(std::vector<VectorKeyFrame> positions);
        void SetRotationKeyFrames(std::vector<QuaternionKeyFrame> rotations);
        void SetScaleKeyFrames(std::vector<VectorKeyFrame> scales);

    private:
        Node<T>* parent;
        std::vector<Node<T>*> children;
        std::string name;

        glm::mat4 transform;

        std::vector<VectorKeyFrame> positionKeyFrames;
        std::vector<QuaternionKeyFrame> rotationKeyFrames;
        std::vector<VectorKeyFrame> scaleKeyFrames;

        std::vector<T> data;
    };

    // templated class, member function implementations need to be defined in this file, below

    template <typename T>
    Node<T>::Node<T>()
    {
        this->parent = {};
        this->children = {};
        this->name = {};
        this->transform = {};
        this->data = {};

        this->positionKeyFrames = {};
        this->rotationKeyFrames = {};
        this->scaleKeyFrames = {};
    }

    template <typename T>
    Node<T>::Node<T>(std::string name, glm::mat4 transform)
    {
        this->parent = {};
        this->children = {};
        this->name = name;
        this->transform = transform;
        this->data = {};

        this->positionKeyFrames = {};
        this->rotationKeyFrames = {};
        this->scaleKeyFrames = {};
    }

    template <typename T>
    Node<T>::Node<T>(std::string name, glm::mat4 transform, T data)
    {
        this->parent = {};
        this->children = {};
        this->name = name;
        this->transform = transform;
        this->data.push_back(data);

        this->positionKeyFrames = {};
        this->rotationKeyFrames = {};
        this->scaleKeyFrames = {};
    }

    template <typename T>
    Node<T>::Node<T>(Node* parent, std::vector<Node*> children, std::string name, glm::mat4 transform)
    {
        this->parent = parent;
        this->children = children;
        this->name = name;
        this->transform = transform;
        this->data = {};

        this->positionKeyFrames = {};
        this->rotationKeyFrames = {};
        this->scaleKeyFrames = {};
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
    std::vector<VectorKeyFrame>& Engine::Node<T>::GetPositionKeyFrames()
    {
        return this->positionKeyFrames;
    }

    template <typename T>
    std::vector<QuaternionKeyFrame>& Engine::Node<T>::GetRotationKeyFrames()
    {
        return this->rotationKeyFrames;
    }

    template <typename T>
    std::vector<VectorKeyFrame>& Engine::Node<T>::GetScaleKeyFrames()
    {
        return this->scaleKeyFrames;
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

    template <typename T>
    void Engine::Node<T>::SetPositionKeyFrames(std::vector<VectorKeyFrame> positions)
    {
        this->positionKeyFrames = positions;
    }

    template <typename T>
    void Engine::Node<T>::SetRotationKeyFrames(std::vector<QuaternionKeyFrame> rotations)
    {
        this->rotationKeyFrames = rotations;
    }

    template <typename T>
    void Engine::Node<T>::SetScaleKeyFrames(std::vector<VectorKeyFrame> scales)
    {
        this->scaleKeyFrames = scales;
    }

    template <typename T>
    Node<T>* Engine::Node<T>::GetRootNode()
    {
        Engine::Node<T>* node = this;

        while (node->GetParent())
            node = node->GetParent();

        return node;
    }

    template <typename T>
    void Engine::Node<T>::DeleteBelow()
    {
        // a simple container for nodes
        std::vector<Engine::Node<T>*> nodes;

        // recursive lambda that is going to traverse the node tree to the bottom and collect everything into a vector
        std::function<void(Engine::Node<T>*)>
        traverseDown = [&nodes, &traverseDown](Engine::Node<T>* node)
        {
            for (size_t i = 0; i < node->GetChildren().size(); i++)
            {
                nodes.push_back(node->GetChildren().at(i));
                traverseDown(node->GetChildren().at(i));
            }
        };

        traverseDown(this);

        // delete everything inside
        for (size_t i = 0; i < nodes.size(); i++)
            delete nodes.at(i);

        // remove any references to original node's children to cover up the tracks
        this->GetChildren().assign(this->GetChildren().size(), nullptr);
        this->GetChildren().clear();
    }

    template <typename T>
    void Engine::Node<T>::DeleteAbove()
    {
        auto root = this->GetRootNode();

        // a simple container for nodes
        std::vector<Engine::Node<T>*> nodes;

        // recursive lambda that is going to traverse the node tree top to bottom -
        //and collect everything into a vector except for this->node and our children
        std::function<void(Engine::Node<T>*)>
        traverseDown = [this, &nodes, &traverseDown](Engine::Node<T>* node)
        {
            for (size_t i = 0; i < node->GetChildren().size(); i++)
            {
                if (node->GetChildren().at(i) != this) {
                    nodes.push_back(node->GetChildren().at(i));
                    traverseDown(node->GetChildren().at(i));
                }
            }
        };

        traverseDown(this);

        // delete everything inside
        for (size_t i = 0; i < nodes.size(); i++)
            delete nodes.at(i);

        // remove any references to original node's parent to cover up the tracks
        this->parent = nullptr;
    }
}

#endif //  NODE_H
