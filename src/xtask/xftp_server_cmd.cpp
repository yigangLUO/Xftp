#include <stirng.h>
#include <string>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/util.h>

#include "xftp_server_cmd.h"
#include "xlog.h"

#define BUFFERSIZE 4096

void XFtpServerCMD::Register(std::string &cmd, XFtpTask *task)
{
    xlog("At XFtpServerCMD::Register");
    if (task == nullptr)
    {
        std::cout << "XFtpServerCMD::Register task is null! " << std::endl;
        return;
    }
    if (cmd.empty())
    {
        std::cout << "XFtpServerCMD::Register cmd is null! " << std::endl;
        return;
    }

    // 已经注册的task提示错误，不覆盖
    if (cmd_map_.find(cmd) != cmd_map_.end())
    {
        std::cout << cmd << " is already register!" << std::endl;
        return;
    }
    xlog(cmd << "Register success!");
    task->event_base_ = event_base_;
    task->task_ = this;
    cmd_map_[cmd] = task;
    tasks_id_map_[task] = 0;
}

void XFtpServerCMD::Event(bufferevent *buf_event, short events)
{
    xlog("At XFtpServerCMD::Event");
    if (events * (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
    {
        bufferevent_free(buf_event);
    }
}

void XFtpServerCMD::Read(bufferevent *buf_event)
{
    xlog("At XFtpServerCMD::Read");

    char buf[BUFFERSIZE] = {0};
    while (true)
    {
        int len = bufferevent_read(buf_event, buf, BUFFERSIZE);
        if (len < 0)
        {
            break;
        }
        std::cout << "Recv CMD(" << len << "): " << buf << std::endl;
        std::string type;
        for (int i = 0; i < len; i++)
        {
            if (buf[i] == ' ' || buf[i] == '\r')
            {
                break;
            }
            type += buf[i];
        }

        std::cout << "type if [ " << type << " ]" << std::endl;
        if (cmd_map_.find(type) != cmd_map_.end())
        {
            xlog("Begin to parse")
            XFtpTask *task = cmd_map_[type];
            task->Parse()
        }
    }
}

void XFtpServerCMD::~XFtpServerCMD()
{
    ClosePORT();

    for (auto i : tasks_id_map_)
    {
        delete i.first;
    }
}