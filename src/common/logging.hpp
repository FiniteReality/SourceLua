#ifndef _logging_hpp_
#define _logging_hpp_

#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <source/tier0/dbg.hpp>
#include <common/version.hpp>

namespace SourceLua
{
enum class LogLevel
{
    Debug,
    Information,
    Warning,
    Error,
    Critical
};

template <LogLevel L, typename... TArgs>
void LogMessage(std::string fmt, TArgs... args)
{
    std::string line_prefix {"[" SOURCELUA_NAME " "};

    switch (L)
    {
        case LogLevel::Debug:
            line_prefix.append("DEBUG] ");
            break;

        case LogLevel::Information:
            line_prefix.append(" INFO] ");
            break;

        case LogLevel::Warning:
            line_prefix.append(" WARN] ");
            break;

        case LogLevel::Error:
            line_prefix.append("ERROR] ");
            break;

        case LogLevel::Critical:
            line_prefix.append(" CRIT] ");
            break;
    }

    if (fmt.back() == '\n')
        fmt.pop_back();

    std::ostringstream output;
    std::string::size_type i = 0;

    while (i != std::string::npos)
    {
        auto newi = fmt.find("\n", i+1);
        if (fmt[i] == '\n')
            i++;

        output << line_prefix << fmt.substr(i, newi-i) << '\n';
        i = newi;
    }

    // we need to do it this way so that the buffer isn't deallocated
    std::string msg = output.str();
    Msg(msg.c_str(), args...);
}
}

#endif /* _logging_hpp_ */
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
