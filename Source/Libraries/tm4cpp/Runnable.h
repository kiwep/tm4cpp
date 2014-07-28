/*
 * Runnable.h
 *
 *  Created on: 2014.07.27.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

namespace tm4cpp
{
  class Runnable
  {
    public:
      virtual void runLoop() =0;
      virtual ~Runnable() {};
  };
}
