#include "Engine.h"

using namespace Shipping;

Path Location::path(Location start, Location end) {
  // TODO(rhau)
  Path path;
  return path;
}

std::vector<Path> Shipping::Location::connectivity(
  Location _root, Mile _distance, Dollar _cost) {
  // TODO(rhau)
  std::vector<Path> paths;
  return paths;
}

EngineManager::EngineManager() {
  // stats_ = new Shipping::Stats();
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

void EngineManager::customerIs(Fwk::Ptr<Customer> _customer) {

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


void EngineManager::portIs(Fwk::Ptr<Port>) {

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

void EngineManager::boatTerminalIs(Fwk::Ptr<BoatTerminal> _boatTerminal) {

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

void EngineManager::truckTerminalIs(Fwk::Ptr<TruckTerminal> _truckTerminal) {

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

void EngineManager::planeTerminalIs(Fwk::Ptr<PlaneTerminal> _planeTerminal) {

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


void EngineManager::boatSegmentIs(Fwk::Ptr<BoatSegment> _boatSegment) {

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
