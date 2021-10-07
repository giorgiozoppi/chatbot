#include "graphedge.h"
#include "graphnode.h"
#include <algorithm>
#include <iterator>
#include <utility>
#include "chatlogic.h"

GraphNode::GraphNode(int id)
{
    _id = id;
}
GraphNode::GraphNode(const GraphNode &node)
{
    DeepCopy(node);
}

GraphNode::GraphNode(GraphNode &&node) noexcept
{
    _childEdges = std::move(node._childEdges);
    _parentEdges = std::move(node._parentEdges);
    _chatBot = std::move(node._chatBot);
    _id = std::move(node._id);
    _answers = std::move(node._answers);
}
GraphNode &GraphNode::operator=(const GraphNode &node)
{
    if (this != &node)
    {
        DeepCopy(node);
    }
    return *this;
}
GraphNode &GraphNode::operator=(GraphNode &&node) noexcept
{
    if (this != &node)
    {
        _childEdges = std::move(node._childEdges);
        _parentEdges = std::move(node._parentEdges);
        _chatBot = std::move(node._chatBot);
        _id = std::move(node._id);
        _answers = std::move(node._answers);
    }
    return *this;
}
void GraphNode::AddToken(const std::string &token)
{
    // more efficient.
    _answers.emplace_back(token);
}
void GraphNode::AddEdgeToParentNode(GraphEdge *edge)
{
    _parentEdges.push_back(edge);
}

void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> &&edge)
{
    _childEdges.emplace_back(std::move(edge));
}


/*
* Here we've three unnecessary copies but they allow us to understand and meet the requirements.
* Basically we could avoid copies, passing as rvalues reference and always move since move is just a static cast.
*/
void GraphNode::MoveChatbotHere(ChatBot chatbot)
{
    _chatBot = std::move(chatbot);
    _chatBot.SetCurrentNode(this);
}
void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{

    if (newNode != this)
    {
        /*
         * Here we've three unnecessary copies but thet allow us to understand and meet the requirements.
         * Basically we could avoid copies, passing as rvalues.
         */

        auto chat(_chatBot);
        newNode->MoveChatbotHere(std::move(chat));
    }
}
GraphEdge *GraphNode::GetChildEdgeAtIndex(int index)
{
    return _childEdges[index].get();
}
void GraphNode::DeepCopy(const GraphNode &node)
{
    std::for_each(std::begin(node._childEdges), std::end(node._childEdges), [this](const auto &edgePtr)
                  { _childEdges.emplace_back(std::make_unique<GraphEdge>(*edgePtr)); });
    _parentEdges = node._parentEdges;
    _chatBot = node._chatBot;
    _id = node._id;
    _answers = node._answers;
}