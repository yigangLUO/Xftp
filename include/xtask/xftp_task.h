#ifndef task_H
#define task_H

#include <event2/bufferevent.h>
#include <string>

#include "xtask_base.h"

// struct bufferevent;

class XFtpTask : public XTaskBase
{
public:
    XFtpTask() = default;
    ~XFtpTask();

    virtual void Parse(std::string, std::string) {}

    void ResCMD(std::string msg);

    void ConnectToPORT();

    void ClosePORT();

    void SendMsg(const std::string &data);
    void SendMsg(const char *data, size_t len);

    void SetCallBack(bufferevent *);

    bool Init()
    {
        return true;
    }

    virtual void Event(bufferevent *, short) {}
    virtual void Read(bufferevent *) {}
    virtual void Write(bufferevent *) {}

    std::string cur_dir_ = "/";
    std::string root_dir = "/";
    std::string ip_ = "";
    int port_ = 0;
    XFtpTask *task_ = nullptr;

protected:
    static void EventCallBack(bufferevent *, short, void *);
    static void ReadCallBack(bufferevent *, void *);
    static void WriteCallBack(bufferevent *, void *);

    bufferevent *buf_event_ = nullptr;
    FILE *file_ = nullptr;
};

#endif