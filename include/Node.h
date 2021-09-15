#ifndef NODE_H
#define NODE_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "functional"
#include "iostream"
#include "string"
#include "vector"
#include "tuple"

namespace Engine
{

    // scene Node, containing basic transformation and animated transformation data
    // implemented as a hierarchical data structure with each node holding information about its parent and children

    class Node
    {
    public:
        Node();
        Node(std::string name, glm::mat4 transform);
        Node(Node* parent, std::vector<Node*> children, std::string name, glm::mat4 transform);

        // utility function for tree traversal and management

        // returns the root node of this tree
        Node* GetRootNode();
        // returns the parent of this node
        Node* GetParent();
        // find a node in a tree with a given name, assumed to be unqiue
        Node* FindNode(std::string name);
        // returns the children of this node
        std::vector<Node*>& GetChildren();
        // returns all nodes of this tree
        std::vector<Node*>& GetTreeNodes();
        // returns the leave nodes of this tree
        std::vector<Node*>& GetTreeLeafNodes();
        // returns all nodes above this one
        std::vector<Node*>& GetNodesAbove();
        // returns all nodes below this one
        std::vector<Node*>& GetNodesBelow();
        // returns a chain of nodes from this node to the root
        std::vector<Node*>& GetPathToRoot();
        // recursively deletes everything above this node, making this node the new root
        void DeleteAbove();
        // recursively deletes everything below this node, making this node a leaf
        void DeleteBelow();

        // functions operating on this->node
        std::string GetName();

        glm::mat4& GetTransform();

        // returns the transformations inherited from parents, without the local one
        glm::mat4 GetInheritedTransform();

        // returns this->transform multiplied by those of all parent nodes
        glm::mat4 GetGlobalTransform();

        // returns the final global transform matrix for this node at a given time
        glm::mat4 GetGlobalAnimatedTransform();

        // returns the calcualted animated transform at a given time
        glm::mat4 GetAnimatedTransform();

        void SetParent(Node* parent);
        void SetChildren(std::vector<Node*> vector);
        void SetName(std::string name);
        void SetTransform(glm::mat4 transform);
        void SetAnimatedTransform(glm::mat4 transform);
        void AddChild(Node* child);

    private:
        Node* parent;
        std::vector<Node*> children;

        std::string name;

        glm::mat4 transform;
        glm::mat4 animatedTransform;
    };
}

#endif //  NODE_H
