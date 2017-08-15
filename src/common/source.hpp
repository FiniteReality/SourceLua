#ifndef _source_hpp_
#define _source_hpp_

/*
 * Common includes for Source server plugins
 */

#ifdef GNUC
// NOTE: this is only defined because the source SDK isn't valid C++11
#pragma GCC system_header
#endif /* GNUC */

#include <convar.h>
#include <eiface.h>
#include <engine/iserverplugin.h>
#include <igameevents.h>
#include <tier1/tier1.h>
#include <tier2/tier2.h>

#endif /* _source_hpp_ */