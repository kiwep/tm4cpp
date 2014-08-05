/*
 * FixedPointerArray.h
 *
 *  Created on: 2014.07.21.
 *      Author: Peter Kovacs ~ k'wep
 */

#pragma once

#include <stddef.h>

namespace tm4cpp
{

  namespace helpers
  {

    template<typename T, uint8_t S>
    class FixedPointerArray
    {
        T *_items[S];
        uint8_t _size;

      public:
        FixedPointerArray();
        void add(T *handler);
        void remove(T *handler);
        uint8_t size();
        T* operator[](const uint8_t index);
    };

    template<typename T, uint8_t S>
    inline FixedPointerArray<T, S>::FixedPointerArray()
    {
      _size = 0;
      uint8_t i;
      for (i = 0; i < S; i++) {
        _items[i] = NULL;
      }
    }

    template<typename T, uint8_t S>
    inline void FixedPointerArray<T, S>::add(T *handler)
    {
      if (_size < S) {
        _items[_size++] = handler;
      }
    }

    template<typename T, uint8_t S>
    inline void FixedPointerArray<T, S>::remove(T *handler)
    {
      uint8_t i;
      bool found = false;
      for (i = 0; i < S; i++) {
        if (!found) {
          if (_items[i] == handler) {
            found = true;
            _items[i] = NULL;
            _size--;
          }
        }
        else {
          T *t = _items[i];
          _items[i - 1] = t;
          if (t == NULL) {
            break;
          }
        }
      }
    }

    template<typename T, uint8_t S>
    inline uint8_t FixedPointerArray<T, S>::size()
    {
      return _size;
    }

    template<typename T, uint8_t S>
    inline T* FixedPointerArray<T, S>::operator[](const uint8_t index)
    {
      return _items[index];
    }

  }
}
