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
protected:
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
protected:
  Ptr<Location> location_; // Associated engine object
  Ptr<ManagerImpl> manager_;
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
protected:
  virtual Ptr<Segment> segment() = 0;
  virtual void sourceIs(const string& v) = 0;
  virtual void returnSegmentIs(const string& v) = 0;
  Ptr<ManagerImpl> manager_;
};


class TruckSegmentRep : public SegmentRep {
public:
  TruckSegmentRep(const string& name, ManagerImpl *manager) :
    SegmentRep(name, manager) {};
protected:
  Ptr<Segment> segment() {return segment_;};
  void sourceIs(const string& v);
  void returnSegmentIs(const string& v);
  Ptr<TruckSegment> segment_;
};

class BoatSegmentRep : public SegmentRep {
public:
  BoatSegmentRep(const string& name, ManagerImpl *manager) :
    SegmentRep(name, manager) {};
protected:
  Ptr<Segment> segment() {return segment_;};
  void sourceIs(const string& v);
  void returnSegmentIs(const string& v);
  Ptr<BoatSegment> segment_;
};

class PlaneSegmentRep : public SegmentRep {
public:
  PlaneSegmentRep(const string& name, ManagerImpl *manager) :
    SegmentRep(name, manager) {};
protected:
  Ptr<Segment> segment() {return segment_;};
  void sourceIs(const string& v);
  void returnSegmentIs(const string& v);
  Ptr<PlaneSegment> segment_;
};


/* Stats */
class StatsRep : public Instance {
public:
  StatsRep(const string& name, ManagerImpl* manager) :
    Instance(name), manager_(manager) {};
  string attribute(const string& name);
  void attributeIs(const string& name, const string& v);
protected:
  Ptr<Stats> stats_;
  Ptr<ManagerImpl> manager_;
};


/* Connectivity */
class ConnRep : public Instance {
public:
  ConnRep(const string& name, ManagerImpl* manager) :
    Instance(name), manager_(manager) {};
  string attribute(const string& name);
  void attributeIs(const string& name, const string& v);
protected:
  Ptr<ManagerImpl> manager_;
};

/* Fleet */





/******************/
/* Implementation */
/******************/

/* Manager */
ManagerImpl::ManagerImpl() {
  // TODO Ptr<EngineManager> engine_ = new EngineManager();
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
  Ptr<Instance> instance = ManagerImpl::instance(name);
  if (instance != NULL) {
    cerr << "Attempt to instantiate existing instance";
    instance = NULL;
    return instance;
  }

  if (type == "Customer") {
    // TODO: add to engine
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
    instance = new TruckSegmentRep(name, this);
  } else if (type == "Boat segment") {
    instance = new BoatSegmentRep(name, this);
  } else if (type == "Plane segment") {
    instance = new PlaneSegmentRep(name, this);
  } else if (type == "Stats") {
    instance = new StatsRep(name, this);
  } else if (type == "Conn") {
  } else if (type == "Fleet") {
  } else {
    cerr << "Invalid instance type instantiation";
    instance = NULL;
    return instance;
  }

  instanceMap_[name] = instance;
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
    Ptr<Location> source = segment()->source();
    if (source == NULL) return "";
    return source->name();
  } else if (name == "length") {
    Mile length = segment()->length();
    ss << length.value();
    return ss.str();
  } else if (name == "return segment") {
    Ptr<Segment> returnSegment = segment()->returnSegment();
    if (returnSegment == NULL) return "";
    return returnSegment->name();
  } else if (name == "difficulty") {
    Difficulty difficulty = segment()->difficulty();
    ss << difficulty.value();
    return ss.str();
  } else if (name == "expedite support") {
    Segment::ExpeditedSupport support = segment()->expeditedSupport();
    if (support == Segment::yes_) {
      return "yes";
    } else {
      return "no";
    }
  } else {
    cerr << "Invalid attribute for segment";
    return "";
  }
}

void SegmentRep::attributeIs(const string& name, const string& v) {
  if (name == "source") {
    sourceIs(v);
  } else if (name == "length") {
    Mile length = atoi(v.c_str());
    segment()->lengthIs(length);
  } else if (name == "difficulty") {
    Difficulty difficulty = atoi(v.c_str());
    segment()->difficultyIs(difficulty);
  } else if (name == "return segment") {
    returnSegmentIs(v);
  } else if (name == "expedite support") {
    Segment::ExpeditedSupport support;
    if (v == "yes") {
      support = Segment::yes_;
    } else if (v == "no") {
      support = Segment::no_;
    } else {
      cerr << "Invalid value for expedited support";
    }
  } else {
    cerr << "Invalid attribute for segment";
  }
}

void TruckSegmentRep::sourceIs(const string& v) {
  Ptr<EngineManager> engine = manager_->engine();
  Ptr<TruckTerminal> terminal = engine->truckTerminal(v);
  if (terminal) { segment_->sourceIs(terminal); return; }
  Ptr<Customer> customer = engine->customer(v);
  if (customer) { segment_->sourceIs(customer); return; }
  Ptr<Port> port = engine->port(v);
  if (port) { segment_->sourceIs(port); return; }
  cerr << "Source does not exist";
}

void TruckSegmentRep::returnSegmentIs(const string& v) {
  Ptr<TruckSegment> returnSegment = manager_->engine()->truckSegment(v);
  if (returnSegment)
    segment_->returnSegmentIs(returnSegment);
  else
    cerr << "Return segment does not exist";
}

void BoatSegmentRep::sourceIs(const string& v) {
  Ptr<EngineManager> engine = manager_->engine();
  Ptr<BoatTerminal> terminal = engine->boatTerminal(v);
  if (terminal) { segment_->sourceIs(terminal); return; }
  Ptr<Customer> customer = engine->customer(v);
  if (customer) { segment_->sourceIs(customer); return; }
  Ptr<Port> port = engine->port(v);
  if (port) { segment_->sourceIs(port); return; }
  cerr << "Source does not exist";
}

void BoatSegmentRep::returnSegmentIs(const string& v) {
  Ptr<BoatSegment> returnSegment = manager_->engine()->boatSegment(v);
  if (returnSegment)
    segment_->returnSegmentIs(returnSegment);
  else
    cerr << "Return segment does not exist";
}

void PlaneSegmentRep::sourceIs(const string& v) {
  Ptr<EngineManager> engine = manager_->engine();
  Ptr<PlaneTerminal> terminal = engine->planeTerminal(v);
  if (terminal) { segment_->sourceIs(terminal); return; }
  Ptr<Customer> customer = engine->customer(v);
  if (customer) { segment_->sourceIs(customer); return; }
  Ptr<Port> port = engine->port(v);
  if (port) { segment_->sourceIs(port); return; }
  cerr << "Source does not exist";
}

void PlaneSegmentRep::returnSegmentIs(const string& v) {
  Ptr<PlaneSegment> returnSegment = manager_->engine()->planeSegment(v);
  if (returnSegment)
    segment_->returnSegmentIs(returnSegment);
  else
    cerr << "Return segment does not exist";
}



/* Stats */
string StatsRep::attribute(const string& name) {
  stringstream ss;

  if (name == "expedite percentage") {
    ss << stats_->expeditedPercentage();
  } else if (name == "Customer") {
    ss << stats_->customerCount();
  } else if (name == "Port") {
    ss << stats_->portCount();
  } else if (name == "Truck Terminal") {
    ss << stats_->truckTerminalCount();
  } else if (name == "Boat Terminal") {
    ss << stats_->boatTerminalCount();
  } else if (name == "Plane Terminal") {
    ss << stats_->planeTerminalCount();
  } else if (name == "Truck Segment") {
    ss << stats_->truckSegmentCount();
  } else if (name == "Boat Segment") {
    ss << stats_->boatSegmentCount();
  } else if (name == "Plane Segment") {
    ss << stats_->planeSegmentCount();
  } else {
    cerr << "Invlaid attribute for stats";
    return "";
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
  //while (std::getline(ss, item, " ")) {
  //  elems.push_back(item);
  //}
  return "";
};

void ConnRep::attributeIs(const string& name, const string& v) {

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
