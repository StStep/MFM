/*                                              -*- mode:C++ -*-
  fail.h Support for meaningful runtime deaths when necessary
  Copyright (C) 2014 The Regents of the University of New Mexico.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
  USA
*/

/**
  \file fail.h Support for meaningful runtime deaths when necessary
  \author David H. Ackley.
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef FAIL_H
#define FAIL_H

#define FAIL(code) MFMFailHere(__FILE__,__LINE__,MFM_FAIL_CODE_REASON_##code)

extern "C" void MFMFailHere(const char * file, const int line, const int code) __attribute__ ((noreturn));
extern "C" const char * MFMFailCodeReason(int failCode) ;

#define XX(a) MFM_FAIL_CODE_REASON_##a,
enum MFMFailureCodes{
  ZERO_UNUSED = 0,
#include "failCodes.h"
#undef XX
  MAX_FAILURE
};

#endif  /* FAIL_H */
