#ifndef _version_hpp_
#define _version_hpp_

#include <info.hpp>

#define _STR(a) _XSTR(a)
#define _XSTR(a) #a

#ifdef SOURCELUA_VERSION_EXTRA
# define SOURCELUA_VERSION \
    _STR(SOURCELUA_VERSION_MAJOR) "." _STR(SOURCELUA_VERSION_MINOR) "." \
    _STR(SOURCELUA_VERSION_MINOR) "-" SOURCELUA_VERSION_EXTRA
#else
# define SOURCELUA_VERSION \
    _STR(SOURCELUA_VERSION_MAJOR) "." _STR(SOURCELUA_VERSION_MINOR) "." \
    _STR(SOURCELUA_VERSION_MINOR)
#endif /* SOURCELUA_VERSION_EXTRA */

#endif /* _version_hpp_ */