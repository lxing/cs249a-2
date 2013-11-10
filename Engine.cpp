#include <assert.h>
#include <queue>
#include <set>

#include "Engine.h"

using namespace Shipping;

void Location::del() {
  // iterate over all of the constituent segments, calling delete on each one
  for (uint32_t i=0; i<segments_.size(); i++) {
    Ptr<Segment> segment = segments_[i];
    Ptr<Location> location = NULL;
    segment->sourceIs(location);
  }
}

void Location::segmentDel(Ptr<Segment> s) {
  for (uint32_t i=0; i<segments_.size(); i++) {
    if (s->name() == segments_[i]->name()) {
      segments_.erase(segments_.begin() + i);
      break;
    }
  }
}

Ptr<Location> Segment::destination() {
  Ptr<Location> dest = NULL;
  if (returnSegment_ != NULL) dest = returnSegment_->source();
  return dest;
}

void Segment::del() {
  // 1. dels itself from its source's list of segments
  Fwk::Ptr<Segment> seg = this;
  source_->segmentDel(seg);

  // 2. dels itself from the returnSegment's returnSegment field
  seg = NULL;
  if (returnSegment_ != NULL) returnSegment_->returnSegmentIs(seg);
}

void Segment::returnSegmentIs(Ptr<Segment> _returnSegment) {
  // 1. set internal return segment
  // TODO: better semantics
  returnSegment_ = _returnSegment;

  // 2. set return segment of returnSegment
  if (_returnSegment != NULL &&
      _returnSegment->returnSegment() == NULL) {
    // If the return segment of our returnSegment does not point to us,
    // we create a pointer to ourself and call returnSegmentIs on our 
    // return segment.
    Fwk::Ptr<Shipping::Segment> segment = this;
    _returnSegment->returnSegmentIs(segment);
  }
}

void Segment::sourceIs(Ptr<Location> loc) {
  Fwk::Ptr<Shipping::Segment> seg = this;
  if (source_ != NULL) source_->segmentDel(seg); // Remove from the old source
  source_ = loc;
  if (loc != NULL) loc->segmentIs(seg);
}

void Segment::expeditedSupportIs(ExpeditedSupport _expeditedSupport) {
  if (_expeditedSupport == expeditedSupport_) {
    // ensures idempotency
    return;
  }
  if (expeditedSupport_ == Segment::yes_) {
    em_->stats()->onExpeditedSegmentDel();
  }

  if (expeditedSupport_ == Segment::no_) {
    em_->stats()->onExpeditedSegmentIs();
  }

  expeditedSupport_ = _expeditedSupport;
}

Dollar BoatSegment::cost(EngineManager* manager, ExpeditedSupport expedited) {
  Dollar cost(length().value() * difficulty().value() *
    manager->boatFleet()->cost().value());
  if (expedited == yes_) cost = cost.value() * expeditedCostMultiplier;
  return cost;
}

Dollar TruckSegment::cost(EngineManager* manager, ExpeditedSupport expedited) {
  Dollar cost(length().value() * difficulty().value() *
    manager->truckFleet()->cost().value());
  if (expedited == yes_) cost = cost.value() * expeditedCostMultiplier;
  return cost;
}

Dollar PlaneSegment::cost(EngineManager* manager, ExpeditedSupport expedited) {
  Dollar cost(length().value() * difficulty().value() *
    manager->planeFleet()->cost().value());
  if (expedited == yes_) cost = cost.value() * expeditedCostMultiplier;
  return cost;
}

Fwk::Ptr<Path> Path::copy(Fwk::Ptr<Path> path) {
  Fwk::Ptr<Path> copyPath = new Path();
  copyPath->expeditedSupportIs(path->expeditedSupport());
  copyPath->segment_ = path->segment_;
  return copyPath;
}

Time BoatSegment::time(Shipping::EngineManager* manager, ExpeditedSupport expedited) {
  Time t(length().value() / manager->boatFleet()->speed().value());
  if (expedited == yes_) t = t.value() / expeditedSpeedMultiplier; 
  return t;
}

Time TruckSegment::time(Shipping::EngineManager* manager, ExpeditedSupport expedited) {
  Time t(length().value() / manager->truckFleet()->speed().value());
  if (expedited == yes_) t = t.value() / expeditedSpeedMultiplier; 
  return t;
}

Time PlaneSegment::time(Shipping::EngineManager* manager, ExpeditedSupport expedited) {
  Time t(length().value() / manager->planeFleet()->speed().value());
  if (expedited == yes_) t = t.value() / expeditedSpeedMultiplier; 
  return t;
}

EngineManager::EngineManager() {
  stats_ = new Shipping::Stats("stats");
  notifieeIs(stats_);
}

EngineManager::~EngineManager() {
  
}

Fwk::Ptr<Entity> EngineManager::entity(string _name) {
  std::map<string, Fwk::Ptr<Entity> >::iterator entityIter;
  entityIter = entityMap_.find(_name);
  if (entityIter == entityMap_.end())  {
    // if the name is not contained, we return null
    Fwk::Ptr<Entity> entityPtr = NULL;
    return entityPtr;  
  } else {
    // otherwise return the entity pointer in the map
    return entityIter->second;
  }
}

Fwk::Ptr<Location> EngineManager::location(string _name) {
  Fwk::Ptr<Location> loc = customer(_name);
  if (loc != NULL) return loc;

  loc = port(_name);
  if (loc != NULL) return loc;

  loc = boatTerminal(_name);
  if (loc != NULL) return loc;

  loc = truckTerminal(_name);
  if (loc != NULL) return loc;

  loc = planeTerminal(_name);
  if (loc != NULL) return loc;

  loc = NULL;
  return loc;
}

void EngineManager::entityDel(string _name) {
  // remove from global entity map
  // remove from specific type map
  // call del() function on entity
  Fwk::Ptr<Entity> entity = NULL;
  if ((entity = customer(_name)) != NULL) {
    Fwk::Ptr<Customer> c = customer(_name);
    customerDel(c);
  } else if ((entity = port(_name)) != NULL) {
    Fwk::Ptr<Port> p = port(_name);
    portDel(p);
  } else if ((entity = boatTerminal(_name)) != NULL) {
    Fwk::Ptr<BoatTerminal> bt = boatTerminal(_name);
    boatTerminalDel(bt);
  } else if ((entity = truckTerminal(_name)) != NULL) {
    Fwk::Ptr<TruckTerminal> tt = truckTerminal(_name);
    truckTerminalDel(tt);
  } else if ((entity = planeTerminal(_name)) != NULL) {
    Fwk::Ptr<PlaneTerminal> pt = planeTerminal(_name);
    planeTerminalDel(pt);
  } else if ((entity = boatSegment(_name)) != NULL) {
    Fwk::Ptr<BoatSegment> bs = boatSegment(_name);
    boatSegmentDel(bs);
  } else if ((entity = truckSegment(_name)) != NULL) {
    Fwk::Ptr<TruckSegment> ts = truckSegment(_name);
    truckSegmentDel(ts);
  } else if ((entity = planeSegment(_name)) != NULL) {
    Fwk::Ptr<PlaneSegment> ps = planeSegment(_name);
    planeSegmentDel(ps);
  }

  if (entity != NULL) {
    entityMap_.erase(_name);
  }
}

Fwk::Ptr<Customer> EngineManager::customer(string _name) {
  std::map<string, Fwk::Ptr<Customer> >::iterator customerIter;
  customerIter = customerMap_.find(_name);

  if (customerIter == customerMap_.end()) {
    Fwk::Ptr<Customer> ptr = NULL;
    return ptr;
  } else {
    return customerIter->second;
  }
}

void EngineManager::customerIs(Fwk::Ptr<Customer> _customer) {
  std::map<string, Fwk::Ptr<Customer> >::iterator customerIter;
  customerIter = customerMap_.find(_customer->name());

  if (_customer == NULL) return;
  if (customerIter != customerMap_.end()) return;

  customerMap_[_customer->name()] = _customer;

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onCustomerIs();
  }
}

void EngineManager::customerDel(Fwk::Ptr<Customer> _customer) {
  _customer->del();

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onCustomerDel();
  }
}

Fwk::Ptr<Port> EngineManager::port(string _name) {
  std::map<string, Fwk::Ptr<Port> >::iterator portIter;
  portIter = portMap_.find(_name);

  if (portIter == portMap_.end()) {
    Fwk::Ptr<Port> ptr = NULL;
    return ptr;
  } else {
    return portIter->second;
  }
}

void EngineManager::portIs(Fwk::Ptr<Port> _port) {
  std::map<string, Fwk::Ptr<Port> >::iterator portIter;
  portIter = portMap_.find(_port->name());

  if (_port == NULL) return;
  if (portIter != portMap_.end()) return;

  portMap_[_port->name()] = _port;

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onPortIs();
  }
}

void EngineManager::portDel(Fwk::Ptr<Port> _port) {
  _port->del();

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onPortDel();
  }
}

Fwk::Ptr<BoatTerminal> EngineManager::boatTerminal(string _name) {
  std::map<string, Fwk::Ptr<BoatTerminal> >::iterator boatTerminalIter;
  boatTerminalIter = boatTerminalMap_.find(_name);

  if (boatTerminalIter == boatTerminalMap_.end()) {
    Fwk::Ptr<BoatTerminal> ptr = NULL;
    return ptr;
  } else {
    return boatTerminalIter->second;
  }
}

void EngineManager::boatTerminalIs(Fwk::Ptr<BoatTerminal> _boatTerminal) {
  std::map<string, Fwk::Ptr<BoatTerminal> >::iterator boatTerminalIter;
  boatTerminalIter = boatTerminalMap_.find(_boatTerminal->name());

  if (_boatTerminal == NULL) return;
  if (boatTerminalIter != boatTerminalMap_.end()) return;

  boatTerminalMap_[_boatTerminal->name()] = _boatTerminal;

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onBoatTerminalIs();
  }
}

void EngineManager::boatTerminalDel(Fwk::Ptr<BoatTerminal> _boatTerminal) {
  _boatTerminal->del();

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onBoatTerminalDel();
  }
}

Fwk::Ptr<TruckTerminal> EngineManager::truckTerminal(string _name) {
  std::map<string, Fwk::Ptr<TruckTerminal> >::iterator truckTerminalIter;
  truckTerminalIter = truckTerminalMap_.find(_name);

  if (truckTerminalIter == truckTerminalMap_.end()) {
    Fwk::Ptr<TruckTerminal> ptr = NULL;
    return ptr;
  } else {
    return truckTerminalIter->second;
  }
}

void EngineManager::truckTerminalIs(Fwk::Ptr<TruckTerminal> _truckTerminal) {
  std::map<string, Fwk::Ptr<TruckTerminal> >::iterator truckTerminalIter;
  truckTerminalIter = truckTerminalMap_.find(_truckTerminal->name());

  if (_truckTerminal == NULL) return;
  if (truckTerminalIter != truckTerminalMap_.end()) return;

  truckTerminalMap_[_truckTerminal->name()] = _truckTerminal;

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onTruckTerminalIs();
  }
}

void EngineManager::truckTerminalDel(Fwk::Ptr<TruckTerminal> _truckTerminal) {
  _truckTerminal->del();

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onTruckTerminalDel();
  }
}

Fwk::Ptr<PlaneTerminal> EngineManager::planeTerminal(string _name) {
  std::map<string, Fwk::Ptr<PlaneTerminal> >::iterator planeTerminalIter;
  planeTerminalIter = planeTerminalMap_.find(_name);

  if (planeTerminalIter == planeTerminalMap_.end()) {
    Fwk::Ptr<PlaneTerminal> ptr = NULL;
    return ptr;
  } else {
    return planeTerminalIter->second;
  }
}

void EngineManager::planeTerminalIs(Fwk::Ptr<PlaneTerminal> _planeTerminal) {
  std::map<string, Fwk::Ptr<PlaneTerminal> >::iterator planeTerminalIter;
  planeTerminalIter = planeTerminalMap_.find(_planeTerminal->name());

  if (_planeTerminal == NULL) return;
  if (planeTerminalIter != planeTerminalMap_.end()) return;

  planeTerminalMap_[_planeTerminal->name()] = _planeTerminal;

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onPlaneTerminalIs();
  }
}

void EngineManager::planeTerminalDel(Fwk::Ptr<PlaneTerminal> _planeTerminal) {
  _planeTerminal->del();

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onPlaneTerminalDel();
  }
}

void EngineManager::boatSegmentIs(Fwk::Ptr<BoatSegment> _boatSegment) {
  std::map<string, Fwk::Ptr<BoatSegment> >::iterator boatSegmentIter;
  boatSegmentIter = boatSegmentMap_.find(_boatSegment->name());

  if (_boatSegment == NULL) return;
  if (boatSegmentIter != boatSegmentMap_.end()) return;

  boatSegmentMap_[_boatSegment->name()] = _boatSegment;

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onBoatSegmentIs();
    if (_boatSegment->expeditedSupport() == Shipping::Segment::yes_) {
      notifiee_[i]->onExpeditedSegmentIs();
    }
  }
}

void EngineManager::boatSegmentDel(Fwk::Ptr<BoatSegment> _boatSegment) {
  _boatSegment->del();

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onBoatSegmentDel();
    if (_boatSegment->expeditedSupport() == Shipping::Segment::yes_) {
      notifiee_[i]->onExpeditedSegmentDel();
    }
  }
}

Fwk::Ptr<BoatSegment> EngineManager::boatSegment(string _name) {
  std::map<string, Fwk::Ptr<BoatSegment> >::iterator boatSegmentIter;
  boatSegmentIter = boatSegmentMap_.find(_name);

  if (boatSegmentIter == boatSegmentMap_.end()) {
    Fwk::Ptr<BoatSegment> ptr = NULL;
    return ptr;
  } else {
    return boatSegmentIter->second;
  }
}

void EngineManager::truckSegmentIs(Fwk::Ptr<TruckSegment> _truckSegment) {
  std::map<string, Fwk::Ptr<TruckSegment> >::iterator truckSegmentIter;
  truckSegmentIter = truckSegmentMap_.find(_truckSegment->name());

  if (_truckSegment == NULL) return;
  if (truckSegmentIter != truckSegmentMap_.end()) return;

  truckSegmentMap_[_truckSegment->name()] = _truckSegment;

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onTruckSegmentIs();
    if (_truckSegment->expeditedSupport() == Shipping::Segment::yes_) {
      notifiee_[i]->onExpeditedSegmentIs();
    }
  }
}

void EngineManager::truckSegmentDel(Fwk::Ptr<TruckSegment> _truckSegment) {
  _truckSegment->del();

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onTruckSegmentDel();
    if (_truckSegment->expeditedSupport() == Shipping::Segment::yes_) {
      notifiee_[i]->onExpeditedSegmentDel();
    }
  }
}

Fwk::Ptr<TruckSegment> EngineManager::truckSegment(string _name) {
  std::map<string, Fwk::Ptr<TruckSegment> >::iterator truckSegmentIter;
  truckSegmentIter = truckSegmentMap_.find(_name);

  if (truckSegmentIter == truckSegmentMap_.end()) {
    Fwk::Ptr<TruckSegment> ptr = NULL;
    return ptr;
  } else {
    return truckSegmentIter->second;
  }
}

void EngineManager::planeSegmentIs(Fwk::Ptr<PlaneSegment> _planeSegment) {
  std::map<string, Fwk::Ptr<PlaneSegment> >::iterator planeSegmentIter;
  planeSegmentIter = planeSegmentMap_.find(_planeSegment->name());

  if (_planeSegment == NULL) return;
  if (planeSegmentIter != planeSegmentMap_.end()) return;

  planeSegmentMap_[_planeSegment->name()] = _planeSegment;

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onPlaneSegmentIs();
    if (_planeSegment->expeditedSupport() == Shipping::Segment::yes_) {
      notifiee_[i]->onExpeditedSegmentIs();
    }
  }
}

void EngineManager::planeSegmentDel(Fwk::Ptr<PlaneSegment> _planeSegment) {
  _planeSegment->del();

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onPlaneSegmentDel();
    if (_planeSegment->expeditedSupport() == Shipping::Segment::yes_) {
      notifiee_[i]->onExpeditedSegmentDel();
    }
  }
}

Fwk::Ptr<PlaneSegment> EngineManager::planeSegment(string _name) {
  std::map<string, Fwk::Ptr<PlaneSegment> >::iterator planeSegmentIter;
  planeSegmentIter = planeSegmentMap_.find(_name);

  if (planeSegmentIter == planeSegmentMap_.end()) {
    Fwk::Ptr<PlaneSegment> ptr = NULL;
    return ptr;
  } else {
    return planeSegmentIter->second;
  }
}


std::vector<Fwk::Ptr<Path> > EngineManager::connect(
    Fwk::Ptr<Location> start, Fwk::Ptr<Location> end) {
  std::vector<Fwk::Ptr<Path> >  pathsWithExpedited =
      connectImpl(start, end, Segment::yes_);
  std::vector<Fwk::Ptr<Path> >  pathsWithOutExpedited =
      connectImpl(start, end, Segment::no_);

  for (uint32_t i=0; i<pathsWithExpedited.size(); i++) {
    pathsWithOutExpedited.push_back(pathsWithExpedited[i]);
  }

  return pathsWithOutExpedited;
}

std::vector<Fwk::Ptr<Path> > EngineManager::connectImpl(
    Fwk::Ptr<Location> start, Fwk::Ptr<Location> end,
    Segment::ExpeditedSupport expedited) {
  std::set<string> visitedLocs;
  std::vector<Fwk::Ptr<Path> > possiblePaths;

  // case where the start and end are same location
  if (start->name() == end->name()) return possiblePaths;
  visitedLocs.insert(start->name());

  std::queue<Fwk::Ptr<Path> > pathQueue;
  std::vector<Ptr<Segment> > startSegments = start->segments();
  // populate the queue with the segments of the start location
  for (uint32_t i=0; i<startSegments.size(); i++) {
    // we want to add segment if:
    // 1. expeditedSupport is yes_ and our segment has expedited support
    // 2. expeditedSupport is no_
    if ((expedited == Segment::yes_ &&
          startSegments[i]->expeditedSupport() == Segment::yes_) ||
          expedited == Segment::no_) {
      Fwk::Ptr<Path> startPath = new Path();
      startPath->expeditedSupportIs(expedited);

      Ptr<Segment> segment = startSegments[i];
      startPath->addSegment(segment,
                            segment->cost(this, expedited),
                            segment->length(),
                            segment->time(this, expedited));
      pathQueue.push(startPath);
    }
  }

  while (!pathQueue.empty()) {
    Fwk::Ptr<Path> path = pathQueue.front();
    pathQueue.pop();

    std::vector<Fwk::Ptr<Segment> > segments = path->segments();
    Ptr<Segment> currSegment = segments[segments.size()-1];
    // If the source of the return segment (nextLoc) matches our end,
    // then we found our path.

    Fwk::Ptr<Location> currLoc = currSegment->returnSegment()->source();
    if (currLoc->name() == end->name()) {
        // add path to list of possible paths
        possiblePaths.push_back(path);
    }
    visitedLocs.insert(currLoc->name());

    // Otherwise, we add all of the segments from the currLoc to copies of
    // the current path and continue our breadth first search.
    std::vector<Fwk::Ptr<Segment> > nextSegments = currLoc->segments();
    for (uint32_t i=0; i<nextSegments.size(); i++) {
      Ptr<Segment> nextSegment = nextSegments[i];
      Ptr<Location> nextLoc = nextSegment->returnSegment()->source();
      
      std::set<string>::iterator it = visitedLocs.find(nextLoc->name());
      if (it != visitedLocs.end()) {
        // if we have visited this location already, continue
        continue;
      }

      // we want to add segment if:
      // 1. expeditedSupport is yes_ and our segment has expedited support
      // 2. expeditedSupport is no_
      if ((expedited == Segment::yes_ &&
            nextSegment->expeditedSupport() == Segment::yes_) ||
            expedited == Segment::no_) {
        Fwk::Ptr<Path> newPath = Path::copy(path);
        newPath->addSegment(nextSegment, 0, 0, 0);
        Ptr<Segment> segment = segments[i];
        newPath->addSegment(nextSegment,
                            nextSegment->cost(this, expedited),
                            nextSegment->length(),
                            nextSegment->time(this, expedited));
        pathQueue.push(newPath);
      }
    }
  }

  return possiblePaths;
}

std::vector<Fwk::Ptr<Path> > EngineManager::explore(
    Fwk::Ptr<Location> start, Mile _distance, Dollar _cost, Time _time,
    Segment::ExpeditedSupport _expedited) {
  // BFS
  std::set<string> visitedLocs;
  std::vector<Fwk::Ptr<Path> > possiblePaths;
  std::queue<Fwk::Ptr<Path> > pathQueue;
  std::vector<Ptr<Segment> > startSegments = start->segments();

  // populate the queue with the segments of the start location
  visitedLocs.insert(start->name());
  for (uint32_t i=0; i<startSegments.size(); i++) {
    Fwk::Ptr<Path> startPath = new Path();
    startPath->expeditedSupportIs(_expedited);
    Ptr<Segment> startSegment = startSegments[i];

    Dollar segmentCost = startSegment->cost(this, _expedited);
    Time segmentTime = startSegment->time(this, _expedited);

    // check cost, distance, and time are under constraints
    if (segmentCost+startPath->cost() <= _cost &&
          segmentTime+startPath->time() <= _time &&
          startSegment->length()+startPath->length() <= _distance &&
          (_expedited == Segment::no_ || 
              (_expedited == Segment::yes_ &&
                  startSegment->expeditedSupport() == Segment::yes_))) {
      Fwk::Ptr<Location> nextLoc = startSegments[i]->returnSegment()->source();
      visitedLocs.insert(nextLoc->name());

      startPath->addSegment(startSegments[i],
                            segmentCost,
                            startSegment->length(),
                            segmentTime);
      possiblePaths.push_back(startPath);
      pathQueue.push(startPath);
    }
  }

  while (!pathQueue.empty()) {
    Fwk::Ptr<Path> path = pathQueue.front();
    pathQueue.pop();

    std::vector<Fwk::Ptr<Segment> > segments = path->segments();
    Ptr<Segment> currSegment = segments[segments.size()-1];
    Fwk::Ptr<Location> currLoc = currSegment->returnSegment()->source();

    std::vector<Fwk::Ptr<Segment> > nextSegments = currLoc->segments();
    for (uint32_t i=0; i<nextSegments.size(); i++) {
      Ptr<Segment> nextSegment = nextSegments[i];
      Ptr<Location> nextLoc = nextSegment->returnSegment()->source();

      std::set<string>::iterator it = visitedLocs.find(nextLoc->name());
      if (it != visitedLocs.end()) {
        // if we have visited this location already, continue
        continue;
      }

      Dollar segmentCost = nextSegment->cost(this, _expedited);
      Time segmentTime = nextSegment->time(this, _expedited);

      // check cost, distance, and time are under constraints
      if (segmentCost+path->cost() <= _cost &&
            segmentTime+path->time() <= _time &&
            nextSegment->length()+path->length() <= _distance &&
            (_expedited == Segment::no_ || 
              (_expedited == Segment::yes_ &&
                  nextSegment->expeditedSupport() == Segment::yes_))) {
        Fwk::Ptr<Path> newPath = Path::copy(path);
        visitedLocs.insert(nextLoc->name());
        newPath->addSegment(nextSegment,
                            segmentCost,
                            nextSegment->length(),
                            segmentTime);
        possiblePaths.push_back(newPath);
        pathQueue.push(newPath);
      }
    }
  }

  return possiblePaths;
}

Stats::Stats(const string& name) :
    customerCount_(0), portCount_(0), boatTerminalCount_(0), 
    truckTerminalCount_(0), planeTerminalCount_(0), expeditedSegmentCount_(0),
    boatSegmentCount_(0), truckSegmentCount_(0), planeSegmentCount_(0)
{

}

Stats::~Stats() { }

void Stats::onCustomerIs() { customerCountInc(1); }

void Stats::onPortIs() { portCountInc(1); }

void Stats::onBoatTerminalIs() { boatTerminalCountInc(1); }

void Stats::onTruckTerminalIs() { truckTerminalCountInc(1); }

void Stats::onPlaneTerminalIs() { planeTerminalCountInc(1); }

void Stats::onBoatSegmentIs() { boatSegmentCountInc(1); }

void Stats::onTruckSegmentIs() { truckSegmentCountInc(1); }

void Stats::onPlaneSegmentIs() { planeSegmentCountInc(1); }

void Stats::onExpeditedSegmentIs() { expeditedSegmentCountInc(1); }

void Stats::onCustomerDel() { customerCountInc(-1); }

void Stats::onPortDel() { portCountInc(-1); }

void Stats::onBoatTerminalDel() { boatTerminalCountInc(-1); }

void Stats::onTruckTerminalDel() { truckTerminalCountInc(-1); }

void Stats::onPlaneTerminalDel() { planeTerminalCountInc(-1); }

void Stats::onBoatSegmentDel() { boatSegmentCountInc(-1); }

void Stats::onTruckSegmentDel() { truckSegmentCountInc(-1); }

void Stats::onPlaneSegmentDel() { planeSegmentCountInc(-1); }

void Stats::onExpeditedSegmentDel() { expeditedSegmentCountInc(-1); }
