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

        // animation keyframes
        std::vector<VectorKeyFrame>& GetPositionKeyFrames();
        std::vector<QuaternionKeyFrame>& GetRotationKeyFrames();
        std::vector<VectorKeyFrame>& GetScaleKeyFrames();

        // returns the transformations inherited by all parents, without the local one
        glm::mat4 GetInheritedTransforms();
        // returns this->transform multiplied by those of all parent nodes
        glm::mat4 GetGlobalTransform();
        // returns the final global transform matrix for this node at a given time
        glm::mat4 GetGlobalTransform(double timeOffset);

        void SetParent(Node* parent);
        void SetChildren(std::vector<Node*> vector);
        void SetName(std::string name);
        void SetTransform(glm::mat4 transform);
        void AddChild(Node* child);

        void SetPositionKeyFrames(std::vector<VectorKeyFrame> positions);
        void SetRotationKeyFrames(std::vector<QuaternionKeyFrame> rotations);
        void SetScaleKeyFrames(std::vector<VectorKeyFrame> scales);

    private:
        Node* parent;
        std::vector<Node*> children;

        std::string name;

        glm::mat4 transform;

        std::vector<VectorKeyFrame> positionKeyFrames;
        std::vector<QuaternionKeyFrame> rotationKeyFrames;
        std::vector<VectorKeyFrame> scaleKeyFrames;
    };
}

#endif //  NODE_H
