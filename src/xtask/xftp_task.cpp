#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
#include <string.h>

#include "xftp_task.h"
#include "xlog.h"

void XFtpTask::ConnectToPORT()
{
    xlog("XFtpTask::ConnectToPORT() : ");
    if (xftp_task_->ip_.empty() || xftp_task_->port_ <= 0 || !xftp_task_->event_base_)
    {
        std::cout << " ConnectToPORT failed!!!" << std::endl;
        return;
    }
    if (buf_event_ == nullptr)
    {
        bufferevent_free(buf_event_);
        buf_event_ = nullptr;
    }

    buf_event_ = bufferevent_socket_new(xftp_task_->event_base_, -1, BEV_OPT_CLOSE_ON_FREE);
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    evutil_inet_pton(AF_INET, xftp_task_->ip_.c_str(), &sin.sin_addr.s_addr);
    sin.sin_port = htons(xftp_task_->port_);

    SetCallBack(buf_event_);

    timeval time = {60, 0};
    bufferevent_set_timeouts(buf_event_, &time, nullptr);

    bufferevent_socket_connect(buf_event_, (sockaddr *)&sin, sizeof(sin));
}

void XFtpTask::ClosePORT()
{
    if (buf_event_)
    {
        bufferevent_free(buf_event_);
        buf_event_ = nullptr;
    }
    if (file_)
    {
        fclose(file_);
        file_ = nullptr;
    }
}

void XFtpTask::SetCallBack(bufferevent *buf_event)
{
    bufferevent_setcb(buf_event, ReadCallBack, WriteCallBack, EventCallBack, this);

    bufferevent_enable(buf_event, EV_READ | EV_WRITE);
}

void XFtpTask::SendMsg(const std::string &data)
{
    xlog("At XFtpTask::SendMsg");
    SendMsg(data.c_str(), data.size());
}

void XFtpTask::SendMsg(const char *data, size_t len)
{
    xlog("At XFtpTask::SendMsg");
    std::cout << data;
    std::cout << len;
    if (len == 0)
    {
        return;
    }
    if (buf_event_)
    {
        bufferevent_write(buf_event_, data, len);
    }
}

void XFtpTask::ResCMD(std::string msg)
{
    xlog("At XFtpTask::ResCMD");
    if (!xftp_task_ || !xftp_task_->buf_event_)
    {
        return;
    }

    std::cout << "ResCMD : " << msg << std::endl;
    if (msg[msg.size() - 1] != '\n')
    {
        msg += "\r\n";
    }
    bufferevent_write(xftp_task_->buf_event_, msg.c_str(), msg.size());
}

void XFtpTask::ReadCallBack(bufferevent *buf_event, void *arg)
{
    XFtpTask *task = (XFtpTask *)arg;
    task->Read(buf_event);
}

void XFtpTask::WriteCallBack(bufferevent *buf_event, void *arg)
{
    XFtpTask *task = (XFtpTask *)arg;
    task->Write(buf_event);
}

void XFtpTask::EventCallBack(bufferevent *buf_event, short events, void *arg)
{
    XFtpTask *task = (XFtpTask *)arg;
    task->Event(buf_event);
}

XFtpTask::~XFtpTask()
{
    ClosePORT();
}