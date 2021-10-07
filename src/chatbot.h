#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>
#include <string>
#include <memory>
#include <utility>

class GraphNode; // forward declaration
class ChatLogic; // forward declaration


class ChatBot
{
private:
    ///
    /// Here we prefer using a smart pointer, since the image is owned by the chatbox.
    ///
    std::unique_ptr<wxBitmap> _image; // avatar image

    ///
    /// Here we prefer to initialized all the variable
    GraphNode *_currentNode{nullptr};
    GraphNode *_rootNode{nullptr};
    ChatLogic *_chatLogic{nullptr};

    // proprietary functions
    int ComputeLevenshteinDistance(std::string s1, std::string s2);
    /// We've added this for copy constructors. It frees stuffs and load copies.
    void Cleanup();

public:
    // In this class there's no need of defining default constructor and destructor body
    // we keep constructor as default and destructor since from requiriments we need print.

    ChatBot() = default;                     // constructor WITHOUT memory allocation
    /// filename is the name of the image
    ChatBot(const std::string& filename); // constructor WITH memory allocation
    // copy 
    ChatBot(const ChatBot& source);
    ChatBot& operator=(const ChatBot& source); 
    // move
    ChatBot(ChatBot&& source) noexcept;
    ChatBot& operator=(ChatBot&& source) noexcept;  
    
    ~ChatBot();
    // getters / setters
    void SetCurrentNode(GraphNode *node);
    void SetRootNode(GraphNode *rootNode) { _rootNode = rootNode; }
    void SetChatLogicHandle(ChatLogic *chatLogic) { _chatLogic = chatLogic; }
    ChatLogic* GetChatLogicHandle() { return _chatLogic; }
    wxBitmap *GetImageHandle() { return _image.get(); }

    // communication
    void ReceiveMessageFromUser(std::string message);
    private:
};

#endif /* CHATBOT_H_ */
