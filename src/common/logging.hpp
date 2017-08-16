#ifndef _logging_hpp_
#define _logging_hpp_

/*
 * Logging abstractions
 */

#include <string>

#include <common/source.hpp>
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
    constexpr inline void LogMessage(const char* s, TArgs... args)
    {
        std::string line_prefix{"[" SOURCELUA_NAME " "};
        switch(L)
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

        std::string msg{line_prefix};
        msg.append(s);

        std::string::size_type prefix_len = line_prefix.length();
        std::string::size_type offset = 0;
        std::string::size_type i = msg.find("\n", offset);
        while (i != std::string::npos && i < msg.length() - 1)
        {
            msg.insert(i + 1, line_prefix);
            offset += i + prefix_len + 1;
            i = msg.find("\n", offset);
        }

        Msg(msg.c_str(), args...);
    }
}

#endif /* _logging_hpp_ */