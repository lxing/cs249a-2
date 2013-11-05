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
  stats_ = new Shipping::Stats("stats");
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
  std::map<string, Fwk::Ptr<Customer> >::iterator customerIter;
  customerIter = customerMap_.find(_customer->name());

  if (_customer == NULL) return;
  if (customerIter != customerMap_.end()) return;

  customerMap_[_customer->name()] = _customer;

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onCustomerIs();
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
  std::map<string, Fwk::Ptr<BoatTerminal> >::iterator boatTerminalIter;
  boatTerminalIter = boatTerminalMap_.find(_boatTerminal->name());

  if (_boatTerminal == NULL) return;
  if (boatTerminalIter != boatTerminalMap_.end()) return;

  boatTerminalMap_[_boatTerminal->name()] = _boatTerminal;

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onBoatTerminalIs();
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
  std::map<string, Fwk::Ptr<PlaneTerminal> >::iterator planeTerminalIter;
  planeTerminalIter = planeTerminalMap_.find(_planeTerminal->name());

  if (_planeTerminal == NULL) return;
  if (planeTerminalIter != planeTerminalMap_.end()) return;

  planeTerminalMap_[_planeTerminal->name()] = _planeTerminal;

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onPlaneTerminalIs();
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


void EngineManager::boatSegmentIs(Fwk::Ptr<BoatSegment> _boatSegment) {
  std::map<string, Fwk::Ptr<BoatSegment> >::iterator boatSegmentIter;
  boatSegmentIter = boatSegmentMap_.find(_boatSegment->name());

  if (_boatSegment == NULL) return;
  if (boatSegmentIter != boatSegmentMap_.end()) return;

  boatSegmentMap_[_boatSegment->name()] = _boatSegment;

  for (uint32_t i=0; i<notifiee_.size(); i++) {
    notifiee_[i]->onBoatSegmentIs();
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

void Stats::onTruckTerminalIs() { truckSegmentCountInc(1); }

void Stats::onPlaneTerminalIs() { planeTerminalCountInc(1); }

void Stats::onBoatSegmentIs() { boatSegmentCountInc(1); }

void Stats::onTruckSegmentIs() { truckSegmentCountInc(1); }

void Stats::onPlaneSegmentIs() { planeSegmentCountInc(1); }

void Stats::onCustomerDel() { customerCountInc(-1); }

void Stats::onPortDel() { portCountInc(-1); }

void Stats::onBoatTerminalDel() { boatTerminalCountInc(-1); }

void Stats::onTruckTerminalDel() { truckTerminalCountInc(-1); }

void Stats::onPlaneTerminalDel() { planeTerminalCountInc(-1); }

void Stats::onBoatSegmentDel() { boatSegmentCountInc(-1); }

void Stats::onTruckSegmentDel() { truckSegmentCountInc(-1); }

void Stats::onPlaneSegmentDel() { planeSegmentCountInc(-1); }
