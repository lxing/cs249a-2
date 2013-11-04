#include <stdlib.h>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include "Instance.h"
#include "Engine.h"

namespace Shipping {

using namespace std;

/************************/
/* Internal Rep Classes */
/************************/

/* Manager */
class ManagerImpl : public Instance::Manager {
public:
  ManagerImpl();

  Ptr<Instance> instanceNew(const string& name, const string& type);
  Ptr<Instance> instance(const string& name);
  void instanceDel(const string& name);
  Ptr<EngineManager> engine() { return engine_; };


private:
  map<string,Ptr<Instance> > instanceMap_;
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
  Ptr<Location> location_; // Associated engine object
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
  string attribute(const string& name);
  void attributeIs(const string& name, const string& v);
private:
  Ptr<Segment> segment_;
  Ptr<Manager> manager_;
};


/* Stats */
class StatsRep : public Instance {
public:
  StatsRep(const string& name, ManagerImpl* manager) :
    Instance(name), manager_(manager) {};
  string attribute(const string& name);
  void attributeIs(const string& name, const string& v);
private:
  Ptr<Stats> stats_;
  Ptr<Manager> manager_;
};

/* Connectivity */
class ConnRep : public Instance {
public:
  ConnRep(const string& name, ManagerImpl* manager) :
    Instance(name), manager_(manager) {};
  string attribute(const string& name);
  void attributeIs(const string& name, const string& v);
private:
  Ptr<Conn> conn_;
  Ptr<Manager> manager_;
};

/* Fleet */

/******************/
/* Implementation */
/******************/

/* Manager */
ManagerImpl::ManagerImpl() {
  Ptr<EngineManager> engine_ = new EngineManager();
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
  // TODO: Check for existing instance

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

  if (instance != NULL) instanceMap_[name] = instance;
  return instance;
}

Ptr<Instance> ManagerImpl::instance(const string& name) {
  map<string,Ptr<Instance> >::const_iterator t = instanceMap_.find(name);
  return t == instanceMap_.end() ? NULL : (*t).second;
}

void ManagerImpl::instanceDel(const string& name) {
}


/* Locations */
string LocationRep::attribute(const string& name) {
  int i = segmentNumber(name);
  Ptr<Segment> segment = location_->segment(i);
  if (segment == NULL) return "";
  return segment->name();
}

void LocationRep::attributeIs(const string& name, const string& v) {
  // Locations are read-only
  cerr << "Invalid attribue for location";
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


/* Segments */
string SegmentRep::attribute(const string& name) {
  stringstream ss;

  if (name == "source") {
    Ptr<Location> source = segment_->source();
    if (source == NULL) return "";
    return source->name();
  } else if (name == "length") {
    Mile length = segment_->length();
    ss << length.value();
    return ss.str();
  } else if (name == "return segment") {
    Ptr<Segment> returnSegment = segment_->returnSegment();
    if (returnSegment == NULL) return "";
    return returnSegment->name();
  } else if (name == "difficulty") {
    Difficulty difficulty = segment_->difficulty();
    ss << difficulty.value();
    return ss.str();
  } else if (name == "expedite support") {
    Segment::ExpeditedSupport support = segment_->expediteSupport();
    if (support == Segment::yes_) {
      return "yes";
    } else {
      return "no";
    }
  }
}

void SegmentRep::attributeIs(const string& name, const string& v) {
  // TODO
  return;
}


/* Stats */
string StatsRep::attribute(const string& name) {
  stringstream ss;

  if (name == "expedite percentage") {
    ss << stats_->expeditePercentage();
  } else {
    ss << stats_->count(name);
    // TODO: error if invalid entity type (bubble up)
  }

  return ss.str();
}

void StatsRep::attributeIs(const string& name, const string& v) {
  // Stats are read-only
  cerr << "Invalid attribute for stats";
}


/* Connectivity */
string ConnRep::attribute(const string& name) {
  stringstream ss(name);
  string item;
  vector<string> elems;
  while (getline(ss, item, " ")) {
    elems.push_back(item);
  }

  if (elems.empty )

};

void ConnRep::attributeIs(const string& name, string& v) {

};

/* Fleet */


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
