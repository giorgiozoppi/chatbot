#ifndef CHATLOGIC_H_
#define CHATLOGIC_H_

#include <vector>
#include <string>
#include "chatgui.h"


// forward declarations
class ChatBot;
class GraphEdge;
class GraphNode;

class ChatLogic
{
private:
    //// STUDENT CODE
    ////

    // we own the nodes.
    // here the temptation to make edges shared was strong since we pass edges everywhere
    // but we've decided to pass just raw pointer and keep the ownership not shared for two reason:
    // 1. we'll make in a way that we move the graphedges ownership to each source node.
    // The rationale is that when we delete  the source, the edge to the destination makes no sense.
    // 2. performance will suffer a lot and we might have cycle to break.
    std::vector<std::unique_ptr<GraphNode>> _nodes;
    std::vector<std::unique_ptr<GraphEdge>> _edges;

    // data handles (not owned)
    GraphNode *_currentNode;
    ChatBot *_chatBot;
    ChatBotPanelDialog *_panelDialog;
    std::string _fileName;

    // proprietary type definitions
    typedef std::vector<std::pair<std::string, std::string>> tokenlist;

    // proprietary functions
    template <typename T>
    void AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element);

public:
    // constructor / destructor
    ChatLogic();
    ~ChatLogic();

    // getter / setter
    void SetPanelDialogHandle(ChatBotPanelDialog *panelDialog);
    void SetChatbotHandle(ChatBot *chatbot);

    // proprietary functions
    void LoadAnswerGraphFromFile(std::string filename);
    void SendMessageToChatbot(std::string message);
    void SendMessageToUser(std::string message);
    wxBitmap *GetImageFromChatbot();
};

#endif /* CHATLOGIC_H_ */