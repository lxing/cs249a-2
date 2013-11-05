// Copyright (c) 1993-2007 David R. Cheriton, all rights reserved.
// PtrInterface.h

#ifndef NAMEDINTERFACE_H
#define NAMEDINTERFACE_H

#include "PtrInterface.h"


namespace Fwk {

class NamedInterface : public PtrInterface<NamedInterface>
{
public:
  string name() const { return name_; }

  class Notifiee : public PtrInterface<Notifiee> { };

protected:
  NamedInterface(const string& name) : name_(name) { }

private:
  string name_;
};

}

#endif
