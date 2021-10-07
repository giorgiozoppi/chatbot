#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>
#include <exception>
#include <vector>
#include "graphnode.h"
#include "graphedge.h"
#include "chatbot.h"
#include "chatlogic.h"

// constructor WITH memory allocation
ChatBot::ChatBot(const std::string &filename)
{
    std::cout << "ChatBot Constructor" << std::endl;
    _chatLogic = nullptr;
    _rootNode = nullptr;
    _currentNode = nullptr;
    // load image into heap memory
    _image = std::make_unique<wxBitmap>(filename, wxBITMAP_TYPE_PNG);
}
ChatBot::ChatBot(const ChatBot &source)
{
    std::cout << "ChatBot Constructor" << std::endl;
    _chatLogic = source._chatLogic;
    _chatLogic->SetChatbotHandle(this);
    _rootNode = source._rootNode;
    _currentNode = source._currentNode;
    _image.reset(new wxBitmap(*source._image));
}
ChatBot &ChatBot::operator=(const ChatBot &source)
{
    if (this != &source)
    {
        std::cout << "ChatBot Copy Assignment Operator" << std::endl;
        _chatLogic = source._chatLogic;
        _chatLogic->SetChatbotHandle(this);
        _rootNode = source._rootNode;
        _currentNode = source._currentNode;
        _image.reset(new wxBitmap(*source._image));
    }
    return *this;
}
ChatBot::ChatBot(ChatBot &&source) noexcept
{
    std::cout << "ChatBot Move Constructor" << std::endl;
    /// It's cleaner to use exchange, less code.
    /// hint taken by Move Semantics talk by David Olsen: https://www.youtube.com/watch?v=ZG59Bqo7qX4
    _chatLogic = std::exchange(source._chatLogic, nullptr);
    _chatLogic->SetChatbotHandle(this);
    _rootNode = std::exchange(source._rootNode, nullptr);
    _currentNode = std::exchange(source._currentNode, nullptr);
    _image = std::move(source._image);
}
ChatBot &ChatBot::operator=(ChatBot &&source) noexcept
{

    if (this != &source)
    {

        std::cout << "ChatBot Move assignement operator" << std::endl;
        /// It's cleaner to use exchange, less code.
        /// hint taken by Move Semantics talk by David Olsen: https://www.youtube.com/watch?v=ZG59Bqo7qX4
        _chatLogic = std::exchange(source._chatLogic, nullptr);
        _chatLogic->SetChatbotHandle(this);
        _rootNode = std::exchange(source._rootNode, nullptr);
        _currentNode = std::exchange(source._currentNode, nullptr);
        _image = std::move(source._image);
    }
    return *this;
}

ChatBot::~ChatBot()
{
    std::cout << "ChatBot Destructor" << std::endl;
}

void ChatBot::ReceiveMessageFromUser(std::string message)
{
    // loop over all edges and keywords and compute Levenshtein distance to query
    typedef std::pair<GraphEdge *, int> EdgeDist;
    std::vector<EdgeDist> levDists; // format is <ptr,levDist>

    for (size_t i = 0; i < _currentNode->GetNumberOfChildEdges(); ++i)
    {
        auto edge = _currentNode->GetChildEdgeAtIndex(i);
        for (auto keyword : edge->GetKeywords())
        {
            EdgeDist ed{edge, ComputeLevenshteinDistance(keyword, message)};
            levDists.push_back(ed);
        }
    }

    // select best fitting edge to proceed along
    GraphNode *newNode;
    if (levDists.size() > 0)
    {
        // sort in ascending order of Levenshtein distance (best fit is at the top)
        std::sort(levDists.begin(), levDists.end(), [](const EdgeDist &a, const EdgeDist &b)
                  { return a.second < b.second; });
        newNode = levDists.at(0).first->GetChildNode(); // after sorting the best edge is at first position
    }
    else
    {
        // go back to root node
        newNode = _rootNode;
    }

    // tell current node to move chatbot to new node
    _currentNode->MoveChatbotToNewNode(newNode);
}

void ChatBot::SetCurrentNode(GraphNode *node)
{

    _currentNode = node;
    // select a random node answer (if several answers should exist)
    std::vector<std::string> answers = _currentNode->GetAnswers();
    std::mt19937 generator(int(std::time(0)));
    std::uniform_int_distribution<int> dis(0, answers.size() - 1);
    std::string answer = answers.at(dis(generator));
    // send selected node answer to user
    _chatLogic->SendMessageToUser(answer);
}

int ChatBot::ComputeLevenshteinDistance(std::string s1, std::string s2)
{
    // convert both strings to upper-case before comparing
    std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
    std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);

    // compute Levenshtein distance measure between both strings
    const size_t m(s1.size());
    const size_t n(s2.size());

    if (m == 0)
        return n;
    if (n == 0)
        return m;

    /// we prefer to use std::vector<size_t>.
    // it's safer use a vector then allocate a dynamic one.
    std::vector<size_t> costs(n+1);
    {
        int num{0};
        std::generate(std::begin(costs), std::end(costs), [&num](){ return num++; });
    }
    size_t i = 0;
    for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i)
    {
        costs[0] = i + 1;
        size_t corner = i;

        size_t j = 0;
        for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j)
        {
            size_t upper = costs[j + 1];
            if (*it1 == *it2)
            {
                costs[j + 1] = corner;
            }
            else
            {
                size_t t(upper < corner ? upper : corner);
                costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
            }

            corner = upper;
        }
    }
    // i guess here you want type promotion.
    int result = costs[n];
    return result;
}