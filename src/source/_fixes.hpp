/*
 * NOTE: these "fixes" are necessary to allow the Source SDK to work with the
 * C++ standard library correctly. It is intentionally not include-guarded as
 * it is meant to be included multiple times, after a Source SDK include file
 * has been included.
 */

#undef max
#undef min
#undef nullptr
