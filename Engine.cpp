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

void EngineManager::customerLocationIs() {

}

Fwk::Ptr<Customer> EngineManager::customerLocation(string _name) {
  std::map<string, Fwk::Ptr<Customer> > customerIter;
  customerIter = customerLocationMap_.find(_name);

  if (customerIter == customerLocationMap_.end()) {
    Fwk::Ptr<Customer> ptr = NULL;
    return ptr;
  } else {
    return customerIter->second;
  }
}


void EngineManager::portLocationIs() {

}

Fwk::Ptr<Port> EngineManager::portLocation(string _name) {
  std::map<string, Fwk::Ptr<Port> > portIter;
  portIter = portLocationMap_.find(_name);

  if (portIter == portLocationMap_.end()) {
    Fwk::Ptr<port ptr = NULL;
    return ptr;
  } else {
    return portIter->second;
  }
}

void EngineManager::boatTerminalLocationIs() {

}

Fwk::Ptr<BoatTerminal> EngineManager::boatTerminalLocation(string _name) {
  std::map<string, Fwk::Ptr<BoatTerminal> > boatTerminalIter;
  boatIter = boatTerminalLocationMap_.find(_name);

  if (boatTerminalIter == boatTerminalLocationMap_.end()) {
    Fwk::Ptr<BoatTerminal ptr = NULL;
    return ptr;
  } else {
    return boatTerminalIter->second;
  }
}

void EngineManager::truckTerminalLocationIs() {

}

Fwk::Ptr<TruckTerminal> EngineManager::truckTerminalLocation(string _name) {
  std::map<string, Fwk::Ptr<TruckTerminal> > truckTerminalIter;
  truckTerminalIter = truckTerminalLocationMap_.find(_name);

  if (truckIter == truckTerminalLocationMap_.end()) {
    Fwk::Ptr<TruckTerminal> ptr = NULL;
    return ptr;
  } else {
    return truckTerminalIter->second;
  }
}

void EngineManager::planeTerminalLocationIs() {

}

Fwk::Ptr<PlaneTerminal> EngineManager::planeTerminalLocation(string _name) {
  std::map<string, Fwk::Ptr<PlaneTerminal> > planeTerminalIter;
  planeTerminalIter = planeTerminalLocationMap_.find(_name);

  if (planeTerminalIter == planeTerminalLocationMap_.end()) {
    Fwk::Ptr<PlaneTerminal> ptr = NULL;
    return ptr;
  } else {
    return planeTerminalIter->second;
  }
}


void EngineManager::boatSegmentIs() {

}

Fwk::Ptr<BoatSegment> EngineManager::boatSegment(string _name) {
  std::map<string, Fwk::Ptr<BoatSegment> > boatSegmentIter;
  boatSegmentIter = boatSegmentMap_.find(_name);

  if (PlaneTerminalIter == boatSegmentMap_.end()) {
    Fwk::Ptr<BoatSegment> ptr = NULL;
    return ptr;
  } else {
    return boatSegmentIter->second;
  }
}

void EngineManager::truckSegmentIs() {

}

Fwk::Ptr<TruckSegment> EngineManager::truckSegment(string _name) {
  std::map<string, Fwk::Ptr<TruckSegment> > truckSegmentIter;
  truckSegmentIter = boatSegmentMap_.find(_name);

  if (truckSegmentIter == truckSegmentMap_.end()) {
    Fwk::Ptr<TruckSegment> ptr = NULL;
    return ptr;
  } else {
    return truckSegmentIter->second;
  }
}

void EngineManager::planeSegmentIs() {

}

Fwk::Ptr<PlaneSegment> EngineManager::planeSegment(string _name) {
  std::map<string, Fwk::Ptr<PlaneSegment> > planeSegmentIter;
  planeSegmentIter = planeSegmentMap_.find(_name);

  if (planeSegmentIter == planeSegmentMap_.end()) {
    Fwk::Ptr<PlaneSegment> ptr = NULL;
    return ptr;
  } else {
    return planeSegmentIter->second;
  }
}
