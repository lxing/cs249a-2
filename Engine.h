#ifndef ENGINE_H
#define ENGINE_H

#include <typeinfo>

#ifdef _MSC_VER
  #include "vs_stdint.h"
#else
  #include <stdint.h>
#endif

#include <string>
#include <vector>

#include "Instance.h"
#include "NamedInterface.h"
#include "Nominal.h"
#include "Ptr.h"

using Fwk::NamedInterface;

namespace Shipping {

class Entity : public NamedInterface {
protected:
  Entity(const string& name) : NamedInterface(name) { }
};

class Mile : public Nominal<Mile, unsigned int> {
public:
  Mile(unsigned int num) : Nominal<Mile, unsigned int>(num) {

  }
};

class Difficulty : public Nominal<Difficulty, float> {
public:
  Difficulty(float num) : Nominal<Difficulty, float>(num) {
    if (num < 1.0 || num > 5.0) {
      throw "Difficulty value out of range";
    }
  }
};

class Dollar : public Nominal<Dollar, float> {
public:
  Dollar(float num) : Nominal<Dollar, float>(num) {
    if (num < 0.0) {
      throw "Dollar value out of range";
    }
  }
};

class Fleet : public Entity {
public:
  virtual void milesPerHourIs(Mile _milesPerHour) {
    milesPerHour_ = _milesPerHour;
  }
  virtual Mile milesPerHour() {
    return milesPerHour_;
  }
  virtual void capacityIs(uint32_t _capacity) {
    capacity_ = _capacity;
  }
  virtual uint32_t capacity() {
    return capacity_;
  }
  virtual void costPerMileIs(Dollar _costPerMile) {
    costPerMile_ = _costPerMile;
  }
  virtual void costPerMile() {
    return costPerMile_;
  }

protected:
  Fleet(const string& name) : Entity(name), milesPerHour_(0),
      capacity_(0), costPerMile_(0) { }

private:
  Mile milesPerHour_;
  uint32_t capacity_;
  Dollar costPerMile_;
};

class BoatFleet : public Fleet {
public:
  static Fwk::Ptr<BoatFleet> BoatFleetNew(const string& name) {
    Fwk::Ptr<BoatFleet> m = new BoatFleet(name);
    return m;
  }

protected:
  BoatFleet(const string& name) : Fleet(name) { }
};

class PlaneFleet : public Fleet { 
public:
  static Fwk::Ptr<PlaneFleet> PlaneFleetNew(const string& name) {
    Fwk::Ptr<PlaneFleet> m = new PlaneFleet(name);
    return m;
  }

protected:
  PlaneFleet(const string& name) : Fleet(name) { }
};

class TruckFleet : public Fleet {
public:
  static Fwk::Ptr<TruckFleet> TruckFleetNew(const string& name) {
    Fwk::Ptr<TruckFleet> m = new TruckFleet(name);
    return m;
  }

protected:
  TruckFleet(const string& name) : Fleet(name) {}
};

class Segment;

class Path : public NamedInterface {
public:
  string tostring(); 

private:
  std::vector<Entity> path_;
};

// Create your rep/engine interface here.
class Location : public Entity {
public:
  virtual Segment segment(int _i) {
    return segments[_i];
  }

  virtual void segmentIs(Fwk::Ptr<Segment> s) {
    segments.push_back(s);
  }

  Path path(Location _start, Location _end);
  std::vector<Path> connectivity(Location _root, Mile _distance, Dollar _cost);

protected:
  Location(const string& name) : Entity(name) { }
  virtual ~Location();

private:
  std::vector<Ptr<Segment> > segments_;
};


// Locations
class Customer : public Location {
public:
  static Fwk::Ptr<Customer> CustomerNew(const string& name) {
    Fwk::Ptr<Customer> m = new Customer(name);
    return m;
  }

protected:
  Customer(const string& name) : Location(name) {}
  ~Customer() {}
};

class Port : public Location {
public:
  static Fwk::Ptr<Port> PortNew(const string& name) {
    Fwk::Ptr<Port> m = new Port(name);
    return m;
  }

protected:
  Port(const string& name) : Location(name) {}
  ~Port() {}
};

class Terminal : public Location {
protected:
  Terminal(const string& name) : Location(name) {}
  ~Terminal() {}
};

// Terminals
class BoatTerminal : public Terminal {
public:
  static Fwk::Ptr<BoatTerminal> BoatTerminalNew(const string& name) {
    Fwk::Ptr<BoatTerminal> m = new BoatTerminal(name);
    return m;
  }

protected:
  BoatTerminal(const string& name) : Terminal(name) {}
  ~BoatTerminal() {}
};

class TruckTerminal : public Terminal {
public:
  static Fwk::Ptr<TruckTerminal> TruckTerminalNew(const string& name) {
    Fwk::Ptr<TruckTerminal> m = new TruckTerminal(name);
    return m;
  }

protected:
  TruckTerminal(const string& name) : Terminal(name) {}
  ~TruckTerminal() {}
};

class PlaneTerminal : public Terminal {
public:
  static Fwk::Ptr<PlaneTerminal> PlaneTerminalNew(const string& name) {
    Fwk::Ptr<PlaneTerminal> m = new PlaneTerminal(name);
    return m;
  }

protected:
  PlaneTerminal(const string& name) : Terminal(name) {}
  ~PlaneTerminal() {}
};

class Segment : public Entity {
public:
  // returns the global name of the source location
  string source() {
    return source_;
  }
  virtual void sourceIs(string _name, Ptr<Location> _loc);

  Mile length();
  void lengthIs(Mile _length);

  Segment returnSegment();
  void returnSegmentIs(string _name);

  Difficulty difficulty();
  void difficultyIs(Difficulty _difficulty);

  enum ExpeditedSupport {
    yes_ = 0,
    no_ = 1
  };

  ExpeditedSupport expediteSupport();
  void expediteSupportIs(ExpeditedSupport _expedited_support);

protected:
  Segment(const string& name) : Entity(name), name_(name), length_(0),
      difficulty_(0), expeditedSupport_(no_) { }
  virtual ~Segment();

private:
  string name_;
  string source_;
  Mile length_;
  string returnSegment_;
  Difficulty difficulty_;
  ExpeditedSupport expeditedSupport_;
};


// Segments
class BoatSegment : public Segment {
public:
  static Fwk::Ptr<BoatSegment> BoatSegmentNew(const string& name) {
    Fwk::Ptr<BoatSegment> m = new BoatSegment(name);
    return m;
  }
  void sourceIs(string _name, Ptr<Location> _loc);

protected:
  BoatSegment(const string& name) : Segment(name) {};
  ~BoatSegment() {};
};

class TruckSegment : public Segment {
public:
  static Fwk::Ptr<TruckSegment> TruckSegmentNew(const string& name) {
    Fwk::Ptr<TruckSegment> m = new TruckSegment(name);
    return m;
  }
  void sourceIs(string _name, Ptr<Location> _loc);

protected: 
  TruckSegment(const string& name) : Segment(name) {};
  ~TruckSegment() {};
};

class PlaneSegment : public Segment {
public:
  static Fwk::Ptr<PlaneSegment> PlaneSegmentNew(const string& name) {
    Fwk::Ptr<PlaneSegment> m = new PlaneSegment(name);
    return m;
  }
  void sourceIs(string , Ptr<Location> _loc);

protected:
  PlaneSegment(const string& name) : Segment(name) {};
  ~PlaneSegment() {};
};

// TODO: Change this to a notifiee
class Stats : public Entity {
public:
  uint32_t customerCount() { return customerCount_; }
  uint32_t customerCountInc() { customerCount_++; }
  uint32_t portCount() { return portCount_; }
  uint32_t portCountInc() { portCount_++; }

  uint32_t boatTerminalCount() { return boatTerminalCount_; }
  uint32_t boatTerminalCountInc() { boatTerminalCount_++; }
  uint32_t truckTerminalCount() { return truckTerminalCount_; }
  uint32_t truckTerminalCountInc() { truckTerminalCount_++; }
  uint32_t planeTerminalCount() { return planeTerminalCount_; }
  uint32_t planeTerminalCountInc() { planeTerminalCount_++; }

  uint32_t boatSegmentCount() { return boatSegmentCount_; }
  uint32_t boatSegmentCountInc() { boatTerminalCount_++; }
  uint32_t truckSegmentCount() {return truckSegmentCount_; }
  uint32_t truckSegmentCountInc() { truckSegmentCount_++; }
  uint32_t planeSegmentCount() { return planeSegmentCount_; }
  uint32_t planeSegmentCountInc() { planeSegmentCount_++; }

  float expeditedPercentage() {
    return (float)expeditedSegmentCount_ /
        (boatTerminalCount_ + truckSegmentCount_ + planeSegmentCount_) * 100.0;
  }

private:
  uint32_t customerCount_;
  uint32_t portCount_;

  uint32_t boatTerminalCount_;
  uint32_t truckTerminalCount_;
  uint32_t planeTerminalCount_;

  uint32_t expeditedSegmentCount_;
  uint32_t boatSegmentCount_;
  uint32_t truckSegmentCount_;
  uint32_t planeSegmentCount_;
};
 
 // TODO(rhau) make the enginemanager a notifier
class EngineManager : public Fwk::PtrInterface<EngineManager> {
public:
  EngineManager();
  ~EngineManager();

  void boatFleetIs();
  Fwk::Ptr<BoatFleet> boatFleet(string _name);
  void planeFleetIs();
  Fwk::Ptr<PlaneFleet> planeFleet(string _name);
  void truckFleetIs();
  Fwk::Ptr<TruckFleet> truckFleet(string _name);

  void customerLocationIs();
  Fwk::Ptr<Customer> customerLocation(string _name);
  void portLocationIs();
  Fwk::Ptr<Port> portLocation(string _name);
  void boatTerminalLocationIs();
  Fwk::Ptr<BoatTerminal> boatTerminalLocation(string _name);
  void truckTerminalLocationIs();
  Fwk::Ptr<TruckTerminal> truckTerminalLocation(string _name);
  void planeTerminalLocationIs();
  Fwk::Ptr<PlaneTerminal> planeTerminalLocation(string _name);

  void boatSegmentIs();
  Fwk::Ptr<BoatSegment> boatSegment(string _name);
  void truckSegmentIs();
  Fwk::Ptr<TruckSegment> truckegment(string _name);
  void planeSegmentIs();
  Fwk::Ptr<PlaneSegment> planeSegment(string _name);

private:
  Fwk::Ptr<BoatFleet> boatFleetMap_;
  Fwk::Ptr<PlaneFleet> planeFleetMap_;
  Fwk::Ptr<TruckFleet> truckFleetMap_;

  std::map<string, Customer> customerLocationMap_;
  std::map<string, Port> portLocationMap_;
  std::map<string, BoatTerminal> boatTerminalLocationMap_;
  std::map<string, TruckTerminal> truckTerminalLocationMap_;
  std::map<string, PlaneTerminal> planeTerminalLocationMap_;

  std::map<string, BoatSegment> boatSegmentMap_;
  std::map<string, TruckSegment> truckSegmentMap_;
  std::map<string, PlaneSegment> planeSegmentMap_;
};

} /* end namespace */

#endif
