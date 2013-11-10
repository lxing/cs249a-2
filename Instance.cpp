#include <stdlib.h>
#include <iomanip> // Output decimal precision
#include <iostream>
#include <limits> // Value limits
#include <map>
#include <sstream>
#include <vector>
#include "Instance.h"
#include "Engine.h"

#define PRECISION 2

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
protected:
  map<string,Ptr<Instance> > instanceMap_;
  Ptr<EngineManager> engine_;
};

/* Locations */
class LocationRep : public Instance {
public:
  LocationRep(const string& name, Ptr<EngineManager> engine) :
      Instance(name), engine_(engine) {};
  string attribute(const string& name);
  void attributeIs(const string& name, const string& v);
  void engineObjIs(const Ptr<Location> _location) { location_ = _location; };
protected:
  Ptr<Location> location_; // Associated engine object
  Ptr<EngineManager> engine_;
  int segmentNumber(const string& name);
};

class CustomerRep : public LocationRep {
public:
  CustomerRep(const string& name, Ptr<EngineManager> engine) :
    LocationRep(name, engine) {};
};

class PortRep : public LocationRep {
public:
  PortRep(const string& name, Ptr<EngineManager> engine) :
    LocationRep(name, engine) {};
};

class TruckTerminalRep : public LocationRep {
public:
  TruckTerminalRep(const string& name, Ptr<EngineManager> engine) :
    LocationRep(name, engine) {};
};

class BoatTerminalRep : public LocationRep {
public:
  BoatTerminalRep(const string& name, Ptr<EngineManager> engine) :
    LocationRep(name, engine) {};
};

class PlaneTerminalRep : public LocationRep {
public:
  PlaneTerminalRep(const string& name, Ptr<EngineManager> engine) :
    LocationRep(name, engine) {};
};


/* Segments */
class SegmentRep : public Instance {
public:
  SegmentRep(const string& name, Ptr<EngineManager> engine) :
    Instance(name), engine_(engine) {};
  string attribute(const string& name);
  void attributeIs(const string& name, const string& v);
protected:
  virtual Ptr<Segment> segment() = 0;
  virtual void sourceIs(const string& v) = 0;
  virtual void returnSegmentIs(const string& v) = 0;
  Ptr<EngineManager> engine_;
};


class TruckSegmentRep : public SegmentRep {
public:
  TruckSegmentRep(const string& name, Ptr<EngineManager> engine) :
    SegmentRep(name, engine) {};
  void engineObjIs(const Ptr<TruckSegment> _segment) { segment_ = _segment; };
protected:
  Ptr<Segment> segment() {return segment_;};
  void sourceIs(const string& v);
  void returnSegmentIs(const string& v);
  Ptr<TruckSegment> segment_;
};

class BoatSegmentRep : public SegmentRep {
public:
  BoatSegmentRep(const string& name, Ptr<EngineManager> engine) :
    SegmentRep(name, engine) {};
  void engineObjIs(const Ptr<BoatSegment> _segment) { segment_ = _segment; };
protected:
  Ptr<Segment> segment() {return segment_;};
  void sourceIs(const string& v);
  void returnSegmentIs(const string& v);
  Ptr<BoatSegment> segment_;
};

class PlaneSegmentRep : public SegmentRep {
public:
  PlaneSegmentRep(const string& name, Ptr<EngineManager> engine) :
    SegmentRep(name, engine) {};
  void engineObjIs(const Ptr<PlaneSegment> _segment) { segment_ = _segment; };
protected:
  Ptr<Segment> segment() {return segment_;};
  void sourceIs(const string& v);
  void returnSegmentIs(const string& v);
  Ptr<PlaneSegment> segment_;
};


/* Stats */
class StatsRep : public Instance {
public:
  StatsRep(const string& name, Ptr<EngineManager> engine) :
    Instance(name), engine_(engine) {};
  string attribute(const string& name);
  void attributeIs(const string& name, const string& v);
  void engineObjIs(const Ptr<Stats> _stats) { stats_ = _stats; };
protected:
  Ptr<Stats> stats_;
  Ptr<EngineManager> engine_;
};


/* Connectivity */
class ConnRep : public Instance {
public:
  ConnRep(const string& name, Ptr<EngineManager> engine) :
    Instance(name), engine_(engine) {};
  string attribute(const string& name);
  void attributeIs(const string& name, const string& v);
protected:
  string pathString(Ptr<Path> path);
  Ptr<EngineManager> engine_;
};

/* Fleet */
class FleetRep : public Instance {
public:
  FleetRep(const string& name, Ptr<EngineManager> engine) :
    Instance(name), engine_(engine) {};
  string attribute(const string& name);
  void attributeIs(const string& name, const string& v);

  enum FleetType {
    truck_ = 0,
    boat_ = 1,
    plane_ = 2,
    nullType_ = 3
  };

  enum FleetMode {
    speed_ = 0,
    cost_ = 1,
    capacity_ = 2,
    nullMode_ = 3
  };

  void truckFleetIs(const Ptr<TruckFleet> _truckFleet) { truckFleet_ = _truckFleet; };
  void boatFleetIs(const Ptr<BoatFleet> _boatFleet) { boatFleet_ = _boatFleet; };
  void planeFleetIs(const Ptr<PlaneFleet> _planeFleet) { planeFleet_ = _planeFleet; };
protected:
  void parsedInput(const string& name, FleetType& type, FleetMode& mode);
  Ptr<TruckFleet> truckFleet_;
  Ptr<BoatFleet> boatFleet_;
  Ptr<PlaneFleet> planeFleet_;
  Ptr<EngineManager> engine_;
};



/******************/
/* Implementation */
/******************/

/* Manager */
ManagerImpl::ManagerImpl() {
  engine_ = new EngineManager();
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
  Ptr<Instance> instance = ManagerImpl::instance(name);
  if (instance != NULL) {
    cerr << "Attempt to instantiate existing instance";
    instance = NULL;
    return instance;
  }

  if (type == "Customer") {
    Ptr<CustomerRep> rep = new CustomerRep(name, engine_);
    Ptr<Customer> customer = Customer::CustomerNew(name, engine_);
    engine_->customerIs(customer);
    rep->engineObjIs(customer);
    instance = rep;
  } else if (type == "Port") {
    Ptr<PortRep> rep = new PortRep(name, engine_);
    Ptr<Port> port = Port::PortNew(name, engine_);
    engine_->portIs(port);
    rep->engineObjIs(port);
    instance = rep;
  } else if (type == "Truck terminal") {
    Ptr<TruckTerminalRep> rep = new TruckTerminalRep(name, engine_);
    Ptr<TruckTerminal> terminal = TruckTerminal::TruckTerminalNew(name, engine_);
    engine_->truckTerminalIs(terminal);
    rep->engineObjIs(terminal);
    instance = rep;
  } else if (type == "Boat terminal") {
    Ptr<BoatTerminalRep> rep = new BoatTerminalRep(name, engine_);
    Ptr<BoatTerminal> terminal = BoatTerminal::BoatTerminalNew(name, engine_);
    engine_->boatTerminalIs(terminal);
    rep->engineObjIs(terminal);
    instance = rep;
  } else if (type == "Plane terminal") {
    Ptr<PlaneTerminalRep> rep = new PlaneTerminalRep(name, engine_);
    Ptr<PlaneTerminal> terminal = PlaneTerminal::PlaneTerminalNew(name, engine_);
    engine_->planeTerminalIs(terminal);
    rep->engineObjIs(terminal);
    instance = rep;
  } else if (type == "Truck segment") {
    Ptr<TruckSegmentRep> rep = new TruckSegmentRep(name, engine_);
    Ptr<TruckSegment> segment = TruckSegment::TruckSegmentNew(name, engine_);
    engine_->truckSegmentIs(segment);
    rep->engineObjIs(segment);
    instance = rep;
  } else if (type == "Boat segment") {
    Ptr<BoatSegmentRep> rep = new BoatSegmentRep(name, engine_);
    Ptr<BoatSegment> segment = BoatSegment::BoatSegmentNew(name, engine_);
    engine_->boatSegmentIs(segment);
    rep->engineObjIs(segment);
    instance = rep;
  } else if (type == "Plane segment") {
    Ptr<PlaneSegmentRep> rep = new PlaneSegmentRep(name, engine_);
    Ptr<PlaneSegment> segment = PlaneSegment::PlaneSegmentNew(name, engine_);
    engine_->planeSegmentIs(segment);
    rep->engineObjIs(segment);
    instance = rep;
  } else if (type == "Stats") {
    Ptr<StatsRep> rep = new StatsRep(name, engine_);
    Ptr<Stats> stats = engine_->stats();
    rep->engineObjIs(stats);
    instance = rep;
  } else if (type == "Conn") {
    instance = new ConnRep(name, engine_);
    // No need to register with the engine layer; connectivity
    // is exposed via locations
  } else if (type == "Fleet") {
    Ptr<FleetRep> rep = new FleetRep(name, engine_);

    Ptr<TruckFleet> truckFleet = TruckFleet::TruckFleetNew(name, engine_);
    engine_->truckFleetIs(truckFleet);
    rep->truckFleetIs(truckFleet);

    Ptr<BoatFleet> boatFleet = BoatFleet::BoatFleetNew(name, engine_);
    engine_->boatFleetIs(boatFleet);
    rep->boatFleetIs(boatFleet);

    Ptr<PlaneFleet> planeFleet = PlaneFleet::PlaneFleetNew(name, engine_);
    engine_->planeFleetIs(planeFleet);
    rep->planeFleetIs(planeFleet);

    instance = rep;
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
  return t == instanceMap_.end() ? NULL : t->second;
}

void ManagerImpl::instanceDel(const string& name) {
  engine_->entityDel(name);
  instanceMap_.erase(name);
}

/* Locations */
string LocationRep::attribute(const string& name) {
  int i = segmentNumber(name);
  if (i < 1) return ""; // Segments are 1-indexed in the rep layer
  Ptr<Segment> segment = location_->segment(i - 1);
  return (segment == NULL) ? "" : segment->name();
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
  ss << fixed << setprecision(PRECISION);

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
    Mile length = atof(v.c_str());
    segment()->lengthIs(length);
  } else if (name == "difficulty") {
    Difficulty difficulty = atof(v.c_str());
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
    segment()->expeditedSupportIs(support);
  } else {
    cerr << "Invalid attribute for segment";
  }
}

void TruckSegmentRep::sourceIs(const string& v) {
  Ptr<TruckTerminal> terminal = engine_->truckTerminal(v);
  if (terminal) { segment_->sourceIs(terminal); return; }
  Ptr<Customer> customer = engine_->customer(v);
  if (customer) { segment_->sourceIs(customer); return; }
  Ptr<Port> port = engine_->port(v);
  if (port) { segment_->sourceIs(port); return; }
  cerr << "Invalid source";
}

void TruckSegmentRep::returnSegmentIs(const string& v) {
  Ptr<TruckSegment> returnSegment = engine_->truckSegment(v);
  if (returnSegment)
    segment_->returnSegmentIs(returnSegment);
  else
    cerr << "Invalid return segment";
}

void BoatSegmentRep::sourceIs(const string& v) {
  Ptr<BoatTerminal> terminal = engine_->boatTerminal(v);
  if (terminal) { segment_->sourceIs(terminal); return; }
  Ptr<Customer> customer = engine_->customer(v);
  if (customer) { segment_->sourceIs(customer); return; }
  Ptr<Port> port = engine_->port(v);
  if (port) { segment_->sourceIs(port); return; }
  cerr << "Invalid source";
}

void BoatSegmentRep::returnSegmentIs(const string& v) {
  Ptr<BoatSegment> returnSegment = engine_->boatSegment(v);
  if (returnSegment)
    segment_->returnSegmentIs(returnSegment);
  else
    cerr << "Invalid return segment";
}

void PlaneSegmentRep::sourceIs(const string& v) {
  Ptr<PlaneTerminal> terminal = engine_->planeTerminal(v);
  if (terminal) { segment_->sourceIs(terminal); return; }
  Ptr<Customer> customer = engine_->customer(v);
  if (customer) { segment_->sourceIs(customer); return; }
  Ptr<Port> port = engine_->port(v);
  if (port) { segment_->sourceIs(port); return; }
  cerr << "Invalid source";
}

void PlaneSegmentRep::returnSegmentIs(const string& v) {
  Ptr<PlaneSegment> returnSegment = engine_->planeSegment(v);
  if (returnSegment)
    segment_->returnSegmentIs(returnSegment);
  else
    cerr << "Invalid return segment";
}



/* Stats */
string StatsRep::attribute(const string& name) {
  stringstream ss;
  ss << fixed << setprecision(PRECISION);

  if (name == "expedite percentage") {
    ss << stats_->expeditedPercentage();
  } else if (name == "Customer") {
    ss << stats_->customerCount();
  } else if (name == "Port") {
    ss << stats_->portCount();
  } else if (name == "Truck terminal") {
    ss << stats_->truckTerminalCount();
  } else if (name == "Boat terminal") {
    ss << stats_->boatTerminalCount();
  } else if (name == "Plane terminal") {
    ss << stats_->planeTerminalCount();
  } else if (name == "Truck segment") {
    ss << stats_->truckSegmentCount();
  } else if (name == "Boat segment") {
    ss << stats_->boatSegmentCount();
  } else if (name == "Plane segment") {
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
  stringstream iss(name);
  string token;
  vector<string> tokens;
  while (getline(iss, token, ' ')) { tokens.push_back(token); }

  if (tokens.size() < 3 || tokens[2] != ":") {
    cerr << "Invalid attribute for connectivity";
    return "";
  }

  Ptr<Location> src = engine_->location(tokens[1]);
  Segment::ExpeditedSupport support = Segment::no_;
  stringstream oss;
  oss << fixed << setprecision(PRECISION);

  if (src != NULL && tokens[0] == "explore") {
    // Initialize by default with no constraints
    float infinity = numeric_limits<float>::infinity();
    Mile maxDist(infinity);
    Dollar maxCost(infinity);
    Time maxTime(infinity);

    for (unsigned int attr = 3; attr < tokens.size(); attr++) {
      string attrName = tokens[attr];
      if (attrName == "expedited") support = Segment::yes_;
      else if (attrName == "distance") {
        maxDist = atof(tokens[++attr].c_str());
      } else if (attrName == "cost") {
        maxCost = atof(tokens[++attr].c_str());
      } else if (attrName == "time") {
        maxTime = atof(tokens[++attr].c_str());
      }
    }

    vector<Ptr<Path> > paths = engine_->explore(
      src, maxDist, maxCost, maxTime, support);
    for (unsigned int i = 0; i < paths.size(); i++) {
      oss << pathString(paths[i]) << endl;
    }
  } else if (src != NULL && tokens[0] == "connect") {
    Ptr<Location> dst = engine_->location(tokens[3]);
    vector<Ptr<Path> > paths = engine_->connect(src, dst);
    Ptr<Path> path;
    for (unsigned int i = 0; i < paths.size(); i++) {
      path = paths[i];
      support = path->expeditedSupport();
      oss << path->cost().value() << " ";
      oss << path->time().value() << " ";
      oss << (support == Segment::no_ ? "no" : "yes") << "; ";;
      oss << pathString(paths[i]) << endl;
    }
  }

  return oss.str();
};

void ConnRep::attributeIs(const string& name, const string& v) {
  // Connections are read-only
  cerr << "Invalid attribute for connection";
};

string ConnRep::pathString(Ptr<Path> path) {
  stringstream ss;
  ss << fixed << setprecision(2);
  vector<Ptr<Segment> > segments = path->segments();
  for (unsigned int i = 0; i < segments.size(); i++) {
    Ptr<Segment> seg = segments[i];
    if (i > 0) ss << " ";
    ss << seg->source()->name();
    ss << "(" << seg->name() << ":" << seg->length().value();
    ss << ":" << seg->returnSegment()->name() << ")";
  }
  ss << " " << path->destination()->name();
  return ss.str();
};


/* Fleet */
string FleetRep::attribute(const string& name) {
  FleetType type;
  FleetMode mode;
  parsedInput(name, type, mode);

  Ptr<Fleet> fleet;
  switch (type) {
    case truck_: fleet = engine_->truckFleet(); break;
    case boat_: fleet = engine_->boatFleet(); break;
    case plane_: fleet = engine_->planeFleet(); break;
    default: cerr << "Invalid attribute for fleet"; return "";
  }

  stringstream ss;
  ss << fixed << std::setprecision(PRECISION);

  switch (mode) {
    case speed_: ss << fleet->speed().value(); break;
    case capacity_: ss << fleet->capacity().value(); break;
    case cost_: ss << fleet->cost().value() ; break;
    default: cerr << "Invalid attribute for fleet";
  }

  return ss.str();
};

void FleetRep::attributeIs(const string& name, const string& v) {
  FleetType type;
  FleetMode mode;
  parsedInput(name, type, mode);

  Ptr<Fleet> fleet;
  switch (type) {
    case truck_: fleet = engine_->truckFleet(); break;
    case boat_: fleet = engine_->boatFleet(); break;
    case plane_: fleet = engine_->planeFleet(); break;
    default: cerr << "Invalid attribute for fleet"; return;
  }

  MilesPerHour speed(atof(v.c_str()));
  Capacity capacity(atoi(v.c_str()));
  Dollar cost(atof(v.c_str()));
  switch (mode) {
    case speed_: fleet->speedIs(speed); break;
    case capacity_: fleet->capacityIs(capacity); break;
    case cost_: fleet->costIs(cost); break;
    default: cerr << "Invalid attribute for fleet";
  }
};

void FleetRep::parsedInput(const string& name, FleetType& type, FleetMode& mode) {
  stringstream ss(name);
  string token;

  getline(ss, token, ',');
  if (token == "Truck") type = truck_;
  else if (token == "Boat") type = boat_;
  else if (token == "Plane") type = plane_;
  else type = nullType_;

  getline(ss, token);
  if (token == " speed") mode = speed_;
  else if (token == " cost") mode = cost_;
  else if (token == " capacity") mode = capacity_;
  else mode = nullMode_;
}

}/* End namespace Shipping */



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
