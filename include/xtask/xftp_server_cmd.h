#ifndef XFTP_SERVER_CMD_H
#define XFTP_SERVER_CMD_H

#include <unordered_map>

#include "xftp_task.h"

class XFtpServerCMD : public XFtpTask
{
public:
    XFtpServerCMD() = default;
    ~XFtpServerCMD();

    /// @brief 注册命令处理task，不需要考虑线程安全，调用时未分发到线程
    /// @param cmd 
    /// @param task 
    void Register(std::string &cmd, XFtpTask *task);

    void Event(bufferevent *buf_event, short events) override;
    void Read(bufferevent *buf_event) override;
private:
    std::unordered_map<std::string, XFtpTask *> cmd_map_;
    std::unordered_map<XFtpTask *, int> tasks_id_map_;
}

#endif