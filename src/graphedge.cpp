#include "graphnode.h"
#include "graphedge.h"

GraphEdge::GraphEdge(int id)
{
    _id = id;
}

void GraphEdge::SetChildNode(GraphNode *childNode)
{
    _childNode = childNode;
}

void GraphEdge::SetParentNode(GraphNode *parentNode)
{
    _parentNode = parentNode;
}

// here we prefer emplace back and avoid to copy temporary
void GraphEdge::AddToken(const std::string& token)
{
    _keywords.emplace_back(token);
}