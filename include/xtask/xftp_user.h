#ifndef XFTP_USER_H
#define XFTP_USER_H

class XFtpUser : public XFtpTask
{
public:
    XFtpUser() = default;
    ~XFtpUser() = default;

    void Parse(std::string, std::string) override;
}

#endif