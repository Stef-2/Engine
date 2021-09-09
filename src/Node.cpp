#include "Node.h"

Engine::Node::Node()
{
    this->parent = {};
    this->children = {};
    this->name = {};
    this->transform = {};

    this->positionKeyFrames = {};
    this->rotationKeyFrames = {};
    this->scaleKeyFrames = {};
}

Engine::Node::Node(std::string name, glm::mat4 transform)
{
    this->parent = {};
    this->children = {};
    this->name = name;
    this->transform = transform;

    this->positionKeyFrames = {};
    this->rotationKeyFrames = {};
    this->scaleKeyFrames = {};
}

Engine::Node::Node(Node* parent, std::vector<Node*> children, std::string name, glm::mat4 transform)
{
    this->parent = parent;
    this->children = children;
    this->name = name;
    this->transform = transform;

    this->positionKeyFrames = {};
    this->rotationKeyFrames = {};
    this->scaleKeyFrames = {};
}

Engine::Node* Engine::Node::GetParent()
{
    return this->parent;
}

std::vector<Engine::Node*>& Engine::Node::GetChildren()
{
    return this->children;
}

std::string Engine::Node::GetName()
{
    return this->name;
}

glm::mat4& Engine::Node::GetTransform()
{
    return this->transform;
}

std::vector<Engine::VectorKeyFrame>& Engine::Node::GetPositionKeyFrames()
{
    return this->positionKeyFrames;
}

std::vector<Engine::QuaternionKeyFrame>& Engine::Node::GetRotationKeyFrames()
{
    return this->rotationKeyFrames;
}

std::vector<Engine::VectorKeyFrame>& Engine::Node::GetScaleKeyFrames()
{
    return this->scaleKeyFrames;
}

glm::mat4 Engine::Node::GetInheritedTransforms()
{
    glm::mat4 transform = glm::mat4(1.0f);

    Engine::Node* node = this;

    //traverse the node tree up to the root and collect the transformations
    while (node->GetParent()) {
        node = node->GetParent();
        transform = node->GetTransform() * transform;
    }

    return transform;
}

glm::mat4 Engine::Node::GetGlobalTransform()
{
    return this->GetInheritedTransforms() * this->transform;
}

glm::mat4 Engine::Node::GetGlobalTransform(double timeOffset)
{
    // build up the inherited transforms
    glm::mat4 transform = this->GetInheritedTransforms();

    //vectors to be used for final transforms
    glm::vec3 scale;
    glm::quat rotate;
    glm::vec3 translate;

    // utility lambda that finds the normalized scale between the two frames based on current time
    auto KeyFrameScale = [&timeOffset](double before, double after) -> float
    {
        float middle = timeOffset - before;
        float frameDelta = after - before;

        return middle / frameDelta;
    };

    // go through all keyframes and find the ones that match the given time offset
    // transform the inherited matrix using keyframed values
    // order of transformations matters
    // the correct order is Scale, Rotate, Translate (SRT)

    // find and interpolate scaling
    for (size_t i = 0; i < this->scaleKeyFrames.size() - 1; i++)
    {
        // check the range we're in
        if (this->scaleKeyFrames.at(i).timestamp < timeOffset > this->scaleKeyFrames.at(i + 1).timestamp) {

            // pass time values to the lambda for normalization and scaling and do a linear interpolation based on it
            float scaleFactor = KeyFrameScale(this->scaleKeyFrames.at(i).timestamp, this->scaleKeyFrames.at(i + 1).timestamp);
            scale = glm::mix(this->scaleKeyFrames.at(i).value, this->scaleKeyFrames.at(i + 1).value, scaleFactor);
        }
    }

    // find and interpolate rotation
    for (size_t i = 0; i < this->rotationKeyFrames.size() - 1; i++)
    {
        // check the range we're in
        if (this->rotationKeyFrames.at(i).timestamp < timeOffset > this->rotationKeyFrames.at(i + 1).timestamp) {

            // pass time values to the lambda for normalization and scaling and do a spherical linear interpolation based on it
            float scaleFactor = KeyFrameScale(this->rotationKeyFrames.at(i).timestamp, this->rotationKeyFrames.at(i + 1).timestamp);
            rotate = glm::slerp(this->rotationKeyFrames.at(i).value, this->rotationKeyFrames.at(i + 1).value, scaleFactor);
        }
    }

    //find and interpolate translation
    for (size_t i = 0; i < this->positionKeyFrames.size() - 1; i++)
    {
        // check the range we're in
        if (this->positionKeyFrames.at(i).timestamp < timeOffset > this->positionKeyFrames.at(i + 1).timestamp) {

            // pass time values to the lambda for normalization and scaling and do a linear interpolation based on it
            float scaleFactor = KeyFrameScale(this->positionKeyFrames.at(i).timestamp, this->positionKeyFrames.at(i + 1).timestamp);
            translate = glm::mix(this->positionKeyFrames.at(i).value, this->positionKeyFrames.at(i + 1).value, scaleFactor);
        }
    }

    // transform the inherited matrix in the appropriate order
    transform = glm::scale(transform, scale);
    transform = transform * glm::mat4_cast(rotate);
    transform = glm::translate(transform, translate);

    return transform;
}

void Engine::Node::SetParent(Node* parent)
{
    this->parent = parent;
}

void Engine::Node::SetChildren(std::vector<Node*> children)
{
    this->children = children;
}

void Engine::Node::SetName(std::string name)
{
    this->name = name;
}

void Engine::Node::SetTransform(glm::mat4 transform)
{
    this->transform = transform;
}

void Engine::Node::AddChild(Node* child)
{
    this->children.push_back(child);
}

void Engine::Node::SetPositionKeyFrames(std::vector<VectorKeyFrame> positions)
{
    this->positionKeyFrames = positions;
}

void Engine::Node::SetRotationKeyFrames(std::vector<QuaternionKeyFrame> rotations)
{
    this->rotationKeyFrames = rotations;
}

void Engine::Node::SetScaleKeyFrames(std::vector<VectorKeyFrame> scales)
{
    this->scaleKeyFrames = scales;
}

Engine::Node* Engine::Node::GetRootNode()
{
    Engine::Node* node = this;

    while (node->GetParent())
        node = node->GetParent();

    return node;
}

std::vector<Engine::Node*>& Engine::Node::GetTreeNodes()
{
    // node container
    std::vector<Engine::Node*> nodes;

    // recursive lambda that is going to traverse the node tree to the bottom and collect everything into a vector
    std::function<void(Engine::Node*)>
        traverseDown = [&nodes, &traverseDown](Engine::Node* node)
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

std::vector<Engine::Node*>& Engine::Node::GetTreeLeafNodes()
{
    // node container
    std::vector<Engine::Node*> nodes;

    // recursive lambda that is going to traverse the node tree to the bottom and collect nodes without children into a vector
    std::function<void(Engine::Node*)>
        traverseDown = [&nodes, &traverseDown](Engine::Node* node)
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

std::vector<Engine::Node*>& Engine::Node::GetNodesAbove()
{
    // declare root as volatile so the silly compiler doesn't optimize it away
    volatile auto root = this->GetRootNode();

    // node container
    std::vector<Engine::Node*> nodes;

    // recursive lambda that is going to traverse the node tree top to bottom -
    // and collect everything into a vector except for this->node and our children
    std::function<void(Engine::Node*)>
        traverseDown = [this, &nodes, &traverseDown](Engine::Node* node)
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

std::vector<Engine::Node*>& Engine::Node::GetNodesBelow()
{
    // node container
    std::vector<Engine::Node*> nodes;

    // recursive lambda that is going to traverse the node tree to the bottom and collect everything into a vector
    std::function<void(Engine::Node*)>
        traverseDown = [&nodes, &traverseDown](Engine::Node* node)
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

std::vector<Engine::Node*>& Engine::Node::GetPathToRoot()
{
    auto node = this;

    // node container
    std::vector<Engine::Node*> nodes;

    // traverse the tree from this->node to the root
    while (node->GetParent())
    {
        node = node->GetParent();
        nodes.push_back(node);
    }

    return nodes;
}

void Engine::Node::DeleteBelow()
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

void Engine::Node::DeleteAbove()
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
