#ifndef NODE_H
#define NODE_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "functional"
#include "iostream"
#include "string"
#include "vector"

namespace Engine
{
    // keyframe struct for vector (position, scale) types
    struct VectorKeyFrame
    {
        glm::vec3 value;
        double timestamp;
    };

    // keyframe struct for quaternion (rotation) types
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

        // utility function for tree traversal and managment

        // returns the root node of this tree
        Node<T>* GetRootNode();
        // returns the parent of this node
        Node<T>* GetParent();
        // returns the children of this node
        std::vector<Node<T>*>& GetChildren();
        // returns all nodes of this tree
        std::vector<Node<T>*>& GetTreeNodes();
        // returns the leave nodes of this tree
        std::vector<Node<T>*>& GetTreeLeafNodes();
        // returns all nodes above this one
        std::vector<Node<T>*>& GetNodesAbove();
        // returns all nodes below this one
        std::vector<Node<T>*>& GetNodesBelow();
        // returns a chain of nodes from this node to the root
        std::vector<Node<T>*>& GetPathToRoot();
        // recursively deletes everything above this node, making this node the new root
        void DeleteAbove();
        // recursively deletes everything below this node, making this node a leaf
        void DeleteBelow();

        // functions operating on this->node
        std::vector<T>& GetData();
        std::string GetName();
        glm::mat4& GetTransform();

        // animation keyframes
        std::vector<VectorKeyFrame>& GetPositionKeyFrames();
        std::vector<QuaternionKeyFrame>& GetRotationKeyFrames();
        std::vector<VectorKeyFrame>& GetScaleKeyFrames();

        void SetParent(Node<T>* parent);
        void SetChildren(std::vector<Node<T>*> vector);
        void SetName(std::string name);
        void SetTransform(glm::mat4 transform);
        void AddChild(Node<T>* child);
        void AddData(T data);

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
    std::vector<Node<T>*>& Engine::Node<T>::GetTreeNodes()
    {
        // node container
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

        traverseDown(this->GetRootNode());

        return nodes;
    }

    template <typename T>
    std::vector<Node<T>*>& Engine::Node<T>::GetTreeLeafNodes()
    {
        // node container
        std::vector<Engine::Node<T>*> nodes;

        // recursive lambda that is going to traverse the node tree to the bottom and collect nodes without children into a vector
        std::function<void(Engine::Node<T>*)>
            traverseDown = [&nodes, &traverseDown](Engine::Node<T>* node)
        {
            for (size_t i = 0; i < node->GetChildren().size(); i++)
            {
                // check if the node is a leaf
                if (!node->GetChildren().at(i)->GetChildren().size())
                    nodes.push_back(node->GetChildren().at(i));
                
                traverseDown(node->GetChildren().at(i));
            }
        };

        traverseDown(this->GetRootNode());

        return nodes;
    }

    template <typename T>
    std::vector<Node<T>*>& Engine::Node<T>::GetNodesAbove()
    {
        // declare root as volatile so the silly compiler doesn't optimize it away
        volatile auto root = this->GetRootNode();

        // node container
        std::vector<Engine::Node<T>*> nodes;

        // recursive lambda that is going to traverse the node tree top to bottom -
        // and collect everything into a vector except for this->node and our children
        std::function<void(Engine::Node<T>*)>
            traverseDown = [this, &nodes, &traverseDown](Engine::Node<T>* node)
        {
            for (size_t i = 0; i < node->GetChildren().size(); i++)
            {
                // make sure we're not adding ourselves to the delete vector
                if (node->GetChildren().at(i) != this) {
                    nodes.push_back(node->GetChildren().at(i));
                    traverseDown(node->GetChildren().at(i));
                }
            }
        };

        traverseDown(root);

        return nodes;
    }

    template <typename T>
    std::vector<Node<T>*>& Engine::Node<T>::GetNodesBelow()
    {
        // node container
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

        return nodes;
    }

    template <typename T>
    std::vector<Node<T>*>& Engine::Node<T>::GetPathToRoot()
    {
        auto node = this;

        // node container
        std::vector<Engine::Node<T>*> nodes;

        // traverse the tree from this->node to the root
        while (node->GetParent())
        {
            node = node->GetParent();
            nodes.push_back(node);
        }

        return nodes;
    }

    template <typename T>
    void Engine::Node<T>::DeleteBelow()
    {
        // a simple container for nodes scheduled to be deleted
        auto killVector = this->GetNodesBelow();

        // delete everything inside
        for (size_t i = 0; i < killVector.size(); i++)
            delete killVector.at(i);

        // remove any references to this node's children to cover up the tracks
        this->GetChildren().assign(this->GetChildren().size(), nullptr);
        this->GetChildren().clear();
    }

    template <typename T>
    void Engine::Node<T>::DeleteAbove()
    {
        // declare root as volatile so the silly compiler doesn't optimize it away
        volatile auto root = this->GetRootNode();

        // get all nodes above this one
        auto killVector = this->GetNodesAbove();

        // delete everything inside
        for (size_t i = 0; i < killVector.size(); i++)
            delete killVector.at(i);
        
        // remove any references to this node's parent to cover up the tracks
        this->parent = nullptr;

        // delete the original root
        delete root;

        // usurp the throne and set ourselves as the new root
        *root = *this;
    }
}

#endif //  NODE_H
