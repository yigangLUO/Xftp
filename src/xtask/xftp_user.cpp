#include "xftp_user.h"
#include "xlog.h"

void XFtpUser::Parse(std::string, std::string)
{
    xlog("At XFtpUser::Parse");
    ResCMD("230 Login successful.");
}