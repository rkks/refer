/******************************************************************************
 ******************************************************************************
 * Copyright (c) 2007 MatchPoint, All rights reserved
 *
 * File    :    AssertDefines.h
 * Desc    :    
 * Author  :    nico
 *****************************************************************************/

#ifndef __ASSERT_DEFINES_H__
# define __ASSERT_DEFINES_H__

/*****************************************************************************/
/* Macro and Constants definition                                            */
/*****************************************************************************/
# if defined(_WIN32)
#  define FASSERT(b, s) Assert(b, __FUNCSIG__, s)
# elif defined(__GNUC__)
#  define FASSERT(b, s) Assert(b, __PRETTY_FUNCTION__, s)
# else
#  define FASSERT(b, s)  ASSERT(b, "Unknown", s)
# endif /* _WIN32 */

#endif /* __ASSSERT_DEFINES_H__ */
