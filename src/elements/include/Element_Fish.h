/*                                              -*- mode:C++ -*-
  Element_Fish.h An element that acts similarly to a Wa-Tor fish
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
  \file Element_Fish.h An element that acts similarly to a Wa-Tor fish
  \author David H. Ackley.
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ELEMENT_FISH_H
#define ELEMENT_FISH_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "itype.h"
#include "Atom.h"
#include "AbstractElement_WaPat.h"
#include "WindowScanner.h"

namespace MFM
{

  template <class EC>
  class Element_Fish : public AbstractElement_WaPat<EC>
  {
    // Short names for params
    typedef typename EC::ATOM_CONFIG AC;
    typedef typename AC::ATOM_TYPE T;
    enum {
      R = EC::EVENT_WINDOW_RADIUS,

      INITIAL_DEFAULT_BIRTH_AGE = 20
    };

    ElementParameterS32<EC> m_fishBirthAge;
    ElementParameterBool<EC> m_fishEvolve;

  public:
    enum
    {
      ELEMENT_VERSION = 2
    };

    static Element_Fish THE_INSTANCE;
    static const u32 TYPE()
    {
      return THE_INSTANCE.GetType();
    }

    Element_Fish() :
      AbstractElement_WaPat<EC>(MFM_UUID_FOR("Fish", ELEMENT_VERSION)),
      m_fishBirthAge(this, "age", "Birth Age",
                     "Number of events for a fish to mature",
                     1, INITIAL_DEFAULT_BIRTH_AGE, 100/*, 1*/),
      m_fishEvolve(this, "evo", "Evolvable fish",
                   "Is fish birth age set by mutable genes?",
                   false)
    {
      Element<EC>::SetAtomicSymbol("Fi");
      Element<EC>::SetName("Fish");
    }

    virtual const T & GetDefaultAtom() const
    {
      static T defaultAtom(TYPE(),0,0,0);

      this->SetBirthAge(defaultAtom, (u32) m_fishBirthAge.GetValue());
      this->SetCurrentAge(defaultAtom, 0);

      return defaultAtom;
    }

    virtual u32 GetElementColor() const
    {
      return 0xffbbbbaa;
    }

    // Non-diffusable
    virtual u32 Diffusability(EventWindow<EC> & ew, SPoint nowAt, SPoint maybeAt) const
    {
      return nowAt.Equals(maybeAt)?COMPLETE_DIFFUSABILITY:0;
    }

    virtual u32 PercentMovable(const T& you,
                               const T& me, const SPoint& offset) const
    {
      return 0;
    }

    virtual void Behavior(EventWindow<EC>& window) const
    {
      T self = window.GetCenterAtomSym();
      SPoint emptyRel;
      u32 emptyCount = 0;
      u32 age = this->GetCurrentAge(self);
      WindowScanner<EC> scanner(window);

      // Don't use genetic birth age (yet): bool reproable = age >= this->GetBirthAge(self);
      bool reproable = age >= (u32) m_fishBirthAge.GetValue();

      if (!reproable)
      {
        this->SetCurrentAge(self, 1 + age);
      }

      emptyCount = scanner.FindEmptyInVonNeumann(emptyRel);

      if (emptyCount > 0)  // Can move
      {
        if (reproable)     // and leave kid behind
        {
          this->SetCurrentAge(self,0);
          window.SetCenterAtomSym(self);
        }
        else               // or leave empty behind
        {
          window.SetCenterAtomSym(Element_Empty<EC>::THE_INSTANCE.GetDefaultAtom());
        }
        window.SetRelativeAtomSym(emptyRel,self); // move or repro
      }
      else                 // Can't move
      {
        if (!reproable)    // But if immature
        {
          window.SetCenterAtomSym(self);  // Can age
        }
      }
    }
  };

  template <class EC>
  Element_Fish<EC> Element_Fish<EC>::THE_INSTANCE;

}

#endif /* ELEMENT_FISH_H */
