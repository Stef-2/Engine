#include "Node.h"

Engine::Node::Node()
{
    this->parent = {};
    this->children = {};
    this->name = {};
    this->transform = {};
    this->animatedTransform = glm::mat4(1.0f);
}

Engine::Node::Node(std::string name, glm::mat4 transform)
{
    this->parent = {};
    this->children = {};
    this->name = name;
    this->transform = transform;
    this->animatedTransform = glm::mat4(1.0f);
}

Engine::Node::Node(Node* parent, std::vector<Node*> children, std::string name, glm::mat4 transform)
{
    this->parent = parent;
    this->children = children;
    this->name = name;
    this->transform = transform;
    this->animatedTransform = glm::mat4(1.0f);
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

glm::mat4 Engine::Node::GetInheritedTransform()
{
    glm::mat4 transform = glm::mat4(1.0f);

    Engine::Node* node = this;

    //traverse the node tree up to the root and collect the transformations
    while (node->GetParent()) {
        node = node->GetParent();
        //if (node->name == "RootNode") break;
        glm::mat4 inherited = node->GetTransform();
        //if (inherited != glm::mat4(0.0f))
            transform = inherited * transform;
    }

    return transform;
}

glm::mat4 Engine::Node::GetGlobalTransform()
{
    return this->GetInheritedTransform() * this->transform;
}

glm::mat4 Engine::Node::GetAnimatedTransform()
{
    return this->animatedTransform;
}

glm::mat4 Engine::Node::GetGlobalAnimatedTransform()
{
    glm::mat4 transform = glm::mat4(1.0f);

    Engine::Node* node = this;

    //traverse the node tree up to the root and collect the transformations
    while (node->GetParent()) {
        node = node->GetParent();
        //if (node->name == "RootNode") break;
        glm::mat4 inherited = node->GetAnimatedTransform();
        //if (inherited != glm::mat4(0.0f))
        transform = inherited * transform;
    }

    return transform * this->animatedTransform;
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

void Engine::Node::SetAnimatedTransform(glm::mat4 transform)
{
    this->animatedTransform = transform;
}

void Engine::Node::AddChild(Node* child)
{
    this->children.push_back(child);
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

Engine::Node* Engine::Node::FindNode(std::string name)
{
    std::vector<Node*> nodes = this->GetTreeNodes();

    for (size_t i = 0; i < nodes.size(); i++)
        if (nodes.at(i)->GetName() == name)
            return nodes.at(i);

    return nullptr;
}