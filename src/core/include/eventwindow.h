#ifndef EVENTWINDOW_H      /* -*- C++ -*- */
#define EVENTWINDOW_H

#include "point.h"
#include "itype.h"

template <class T,u32 R>
class Tile;

template <class T,u32 R>
class EventWindow
{
private:

  typedef u32 (* StateFunction )(T* atom);

  u8 m_neighborConnections;
  
  u32 m_atomCount;

  u32 m_tileWidth;

  T* m_atoms;

  Point<int> m_center;

public:

  EventWindow(Point<int>& center, T* atoms, u32 tileWidth, u8 neighborConnections);

  EventWindow() { }

  ~EventWindow() { }

  u32 GetAtomCount()
  {
    return m_atomCount;
  }

  T& GetCenterAtom();

  T& GetRelativeAtom(Point<int>& offset);

  bool SetRelativeAtom(Point<int>& offset, T atom, StateFunction f, s32* atomCounts);

  void SwapAtoms(Point<int>& locA, Point<int>& locB);

  void FillCenter(Point<int>& out);
  
};

#include "eventwindow.tcc"

#endif /*EVENTWINDOW_H*/
