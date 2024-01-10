#ifndef XTASKBASE_H
#define XTASKBASE_H

class XTaskBase
{
public:
    XTaskBase() = default;
    ~XTaskBase() = default;

    virtual bool Init() = 0;

    int sock_id_ = 0;
    int thread_id_ = 0;
    struct event_base *event_base_ = nullptr;
};

#endif