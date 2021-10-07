#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <vector>
#include <string>
#include <memory>
#include "chatbot.h"


// forward declarations
class GraphEdge;

class GraphNode
{
private:
    
    /**
     * @brief Chield Edges. The idea here is that we take the exclusive ownership for child edges.
     * This make sense because when a node dies it remove all its edges towards the childern. In a graph we've 
     * Node Source <--> Destination Node.
     * This create a waterfall, such that whether sequentially i remove all nodes from 1 to N, don't need to be responsible
     * for the edges pointed to me because the source node will be. 
     */
    std::vector<std::unique_ptr<GraphEdge>> _childEdges;  // edges to subsequent nodes
    // data handles (not owned)
    std::vector<GraphEdge *> _parentEdges; // edges to preceding nodes 
    ChatBot _chatBot;
    int _id;
    std::vector<std::string> _answers;

public:
    // constructor / destructor
    GraphNode(int id);
    ~GraphNode() = default;
    GraphNode(const GraphNode& nodes);
    GraphNode& operator=(const GraphNode& nodes);
    GraphNode(GraphNode&& nodes) noexcept;
    GraphNode& operator=(GraphNode&& nodes) noexcept;
    
    

    // getter / setter
    int GetID() const { return _id; }
    size_t GetNumberOfChildEdges() const { return _childEdges.size(); }
    GraphEdge* GetChildEdgeAtIndex(int index);
    std::vector<std::string> GetAnswers() { return _answers; }
    int GetNumberOfParents() const { return _parentEdges.size(); }

    // proprietary functions
    void AddToken(const std::string& token); // add answers to list
    void AddEdgeToParentNode(GraphEdge* edge);
    void AddEdgeToChildNode(std::unique_ptr<GraphEdge>&& edge);
    void MoveChatbotHere(ChatBot chatbot);
    void MoveChatbotToNewNode(GraphNode *newNode);
    private:
    void DeepCopy(const GraphNode& node);

};

#endif /* GRAPHNODE_H_ */