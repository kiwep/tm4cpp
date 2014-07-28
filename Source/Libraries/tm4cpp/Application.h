/*
 * Application.h
 *
 *  Created on: 2014.07.27.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

//#include "Runnable.h"
//#include "../helpers/FixedPointerArray.h"
//
//namespace tm4cpp
//{
//
//  class Application
//  {
//    private:
//      helpers::FixedPointerArray<Runnable, 32> _handlers;
//
//    public:
//      void addToRunLoop(Runnable *runnable);
//      void removeFromRunLoop(Runnable *runnable);
//      void _runHandlers();
//  };
//
//
//  inline void Application::addToRunLoop(Runnable *runnable)
//  {
//    _handlers.add(runnable);
//  }
//
//  inline void Application::removeFromRunLoop(Runnable *runnable)
//  {
//    _handlers.remove(runnable);
//  }
//
//  inline void Application::_runHandlers()
//  {
//    uint8_t s = _handlers.size();
//    for (uint8_t i = 0; i < s; i++) {
//      _handlers[i]->run();
//    }
//  }
//
//}
