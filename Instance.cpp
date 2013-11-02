#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include "Instance.h"
#include "Engine.h"

namespace Shipping {

using namespace std;

/************************/
/* Internal Rep Classes */
/************************/

class ManagerImpl : public Instance::Manager {
public:
  ManagerImpl();

  Ptr<Instance> instanceNew(const string& name, const string& type);
  Ptr<Instance> instance(const string& name);
  void instanceDel(const string& name);
  Ptr<EngineManager> engine() { return engine_; };


private:
  map<string,Ptr<Instance> > instance_;
  Ptr<EngineManager> engine_;
};

/* Locations */
class LocationRep : public Instance {
public:
  LocationRep(const string& name, ManagerImpl* manager) :
      Instance(name), manager_(manager) {};

  string attribute(const string& name);
  void attributeIs(const string& name, const string& v);

private:
  Ptr<Location> location_;
  Ptr<Manager> manager_;
  int segmentNumber(const string& name);

};

class CustomerRep : public LocationRep {
public:
  CustomerRep(const string& name, ManagerImpl *manager) :
    LocationRep(name, manager) {};
};

class PortRep : public LocationRep {
public:
  PortRep(const string& name, ManagerImpl *manager) :
    LocationRep(name, manager) {};
};

class TruckTerminalRep : public LocationRep {
public:
  TruckTerminalRep(const string& name, ManagerImpl *manager) :
    LocationRep(name, manager) {};
};

class BoatTerminalRep : public LocationRep {
public:
  BoatTerminalRep(const string& name, ManagerImpl *manager) :
    LocationRep(name, manager) {};

};

class PlaneTerminalRep : public LocationRep {
public:
  PlaneTerminalRep(const string& name, ManagerImpl *manager) :
    LocationRep(name, manager) {};
};


/* Segments */
class SegmentRep : public Instance {
public:
  SegmentRep(const string& name, ManagerImpl* manager) :
    Instance(name), manager_(manager) {};
private:
    Ptr<Manager> manager_;
};

/******************/
/* Implementation */
/******************/

/* Locations */
ManagerImpl::ManagerImpl() {
    Ptr<EngineManager> engine_ = new EngineManager();
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
  Ptr<Instance> instance = NULL;
  if (type == "Customer") {
    instance = new CustomerRep(name, this);
  } else if (type == "Port") {
    instance = new PortRep(name, this);
  } else if (type == "Truck terminal") {
    instance = new TruckTerminalRep(name, this);
  } else if (type == "Boat terminal") {
    instance = new BoatTerminalRep(name, this);
  } else if (type == "Plane terminal") {
    instance = new PlaneTerminalRep(name, this);
  } else if (type == "Truck segment") {
  } else if (type == "Boat segment") {
  } else if (type == "Plane segment") {
  } else if (type == "Stats") {
  } else if (type == "Conn") {
  } else if (type == "Fleet") {
  }

  return instance;
}

Ptr<Instance> ManagerImpl::instance(const string& name) {
    map<string,Ptr<Instance> >::const_iterator t = instance_.find(name);

    return t == instance_.end() ? NULL : (*t).second;
}

void ManagerImpl::instanceDel(const string& name) {
}


string LocationRep::attribute(const string& name) {
  int i = segmentNumber(name);
  if (i != 0) {
      cout << "Tried to read interface " << i;
  }
  return "";
}


void LocationRep::attributeIs(const string& name, const string& v) {
  // LocationReps are read-only
}

static const string segmentStr = "segment";
static const int segmentStrlen = segmentStr.length();

int LocationRep::segmentNumber(const string& name) {
  if (name.substr(0, segmentStrlen) == segmentStr) {
      const char* t = name.c_str() + segmentStrlen;
      return atoi(t);
  }
  return 0;
}

} /* End namespace Shipping */

/*
 * This is the entry point for your library.
 * The client program will call this function to get a handle
 * on the Instance::Manager object, and from there will use
 * that object to interact with the middle layer (which will
 * in turn interact with the engine layer).
 */
Ptr<Instance::Manager> shippingInstanceManager() {
    return new Shipping::ManagerImpl();
}
