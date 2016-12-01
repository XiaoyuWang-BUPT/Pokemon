#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <thread>

class Helper
{
private:
    std::string recvStrHelper = "";
    std::string sendStrHelper = "";
public:
    Helper() {}
    ~Helper() {}
    std::string getRecvStrHelper() { return recvStrHelper; }
    std::string getSendStrHelper() { return sendStrHelper; }
    void setRecvStrHelper(std::string recvStr) { recvStrHelper = recvStr; }
    void setSendStrHelper(std::string sendStr) { sendStrHelper = sendStr; }
};

#endif // HELPER_H
