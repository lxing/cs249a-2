#ifndef ENGINE_H
#define ENGINE_H

#include <typeinfo>

#ifdef _MSC_VER
  #include "vs_stdint.h"
#else
  #include <stdint.h>
#endif

#include <map>
#include <string>
#include <vector>

#include "Instance.h"
#include "NamedInterface.h"
#include "Nominal.h"
#include "Ptr.h"
#include "PtrInterface.h"

using Fwk::NamedInterface;
using Fwk::PtrInterface;

namespace Shipping {

class Entity : public NamedInterface {
protected:
  Entity(const string& name) : NamedInterface(name) { }
};


// Value types
class Mile : public Ordinal<Mile, uint32_t> {
public:
  Mile(uint32_t num) : Ordinal<Mile, uint32_t>(num) { }
};

class MilesPerHour : public Ordinal<MilesPerHour, double> {
public:
  MilesPerHour(double num) : Ordinal<MilesPerHour, double>(num) {
    if (num < 0.0) {
      throw "Speed out of range";
    }
  }
};

class Time : public Ordinal<Time, uint32_t> {
public:
  Time(uint32_t num) : Ordinal<Time, uint32_t>(num) { }
};

class Capacity : public Nominal<Capacity, uint32_t> {
public:
  Capacity(uint32_t num) : Nominal<Capacity, uint32_t>(num) { }
};

class Difficulty : public Nominal<Difficulty, double> {
public:
  Difficulty(double num) : Nominal<Difficulty, double>(num) {
    if (num < 1.0 || num > 5.0) {
      throw "Difficulty value out of range";
    }
  }
};

class Dollar : public Nominal<Dollar, double> {
public:
  Dollar(double num) : Nominal<Dollar, double>(num) {
    if (num < 0.0) {
      throw "Dollar value out of range";
    }
  }
};

class Fleet : public Entity {
public:

  virtual void speedIs(MilesPerHour _speed) { speed_ = _speed; }
  virtual MilesPerHour speed() { return speed_; }
  virtual void capacityIs(Capacity _capacity) { capacity_ = _capacity; }
  virtual Capacity capacity() { return capacity_; }
  virtual void costIs(Dollar _cost) { cost_ = _cost; }
  virtual Dollar cost() { return cost_; }

protected:
  Fleet(const string& name) : Entity(name), speed_(0),
      capacity_(0), cost_(0) { }

private:
  MilesPerHour speed_;
  Capacity capacity_;
  Dollar cost_;
};

class BoatFleet : public Fleet {
public:
  static Ptr<BoatFleet> BoatFleetNew(const string& name) {
    Ptr<BoatFleet> m = new BoatFleet(name);
    return m;
  }

protected:
  BoatFleet(const string& name) : Fleet(name) { }
};

class PlaneFleet : public Fleet {
public:
  static Ptr<PlaneFleet> PlaneFleetNew(const string& name) {
    Ptr<PlaneFleet> m = new PlaneFleet(name);
    return m;
  }

protected:
  PlaneFleet(const string& name) : Fleet(name) { }
};

class TruckFleet : public Fleet {
public:
  static Ptr<TruckFleet> TruckFleetNew(const string& name) {
    Ptr<TruckFleet> m = new TruckFleet(name);
    return m;
  }

protected:
  TruckFleet(const string& name) : Fleet(name) {}
};

class Segment;
class Path;

// Locations
class Location : public Entity {
public:
  // TODO(rhau) onSegment, which adds a segment as a callback from SegmentNew

  virtual Ptr<Segment> segment(int _i) {
    return segments_[_i];
  }

  virtual std::vector<Ptr<Segment> > segments() {
    return segments_;
  }  

  virtual void segmentIs(Ptr<Segment> s) {
    segments_.push_back(s);
  }

protected:
  Location(const string& name) : Entity(name) {}
  virtual ~Location() {}

private:
  std::vector<Ptr<Segment> > segments_;
};

class Path : public PtrInterface<Path> {
public:
  Path() : pathCost(0), pathLength(0) { }
  ~Path() { }

  Dollar cost() { return pathCost; }
  Mile length() { return pathLength; }

  string tostring();
  
  static Fwk::Ptr<Path> copy(Fwk::Ptr<Path> path);

  void addSegment(Fwk::Ptr<Segment> segment, Dollar segmentCost, Mile length) {
    pathCost += segmentCost;
    pathLength += length; 
    segment_.push_back(segment);
  }

  std::vector<Fwk::Ptr<Segment> > segments() {
    return segment_;
  }

private:
  Dollar pathCost;
  Mile pathLength;
  std::vector<Fwk::Ptr<Segment> > segment_;
};

class Terminal : public Location {
protected:
  Terminal(const string& name) : Location(name) {}
  ~Terminal() {}
};

// Terminals
class BoatTerminal : public Terminal {
public:
  static Ptr<BoatTerminal> BoatTerminalNew(const string& name) {
    Ptr<BoatTerminal> m = new BoatTerminal(name);
    return m;
  }

protected:
  BoatTerminal(const string& name) : Terminal(name) {}
  ~BoatTerminal() {}
};

class TruckTerminal : public Terminal {
public:
  static Ptr<TruckTerminal> TruckTerminalNew(const string& name) {
    Ptr<TruckTerminal> m = new TruckTerminal(name);
    return m;
  }

protected:
  TruckTerminal(const string& name) : Terminal(name) {}
  ~TruckTerminal() {}
};

class PlaneTerminal : public Terminal {
public:
  static Ptr<PlaneTerminal> PlaneTerminalNew(const string& name) {
    Ptr<PlaneTerminal> m = new PlaneTerminal(name);
    return m;
  }

protected:
  PlaneTerminal(const string& name) : Terminal(name) {}
  ~PlaneTerminal() {}
};

class Customer : public Location {
public:
  static Ptr<Customer> CustomerNew(const string& name) {
    Ptr<Customer> m = new Customer(name);
    return m;
  }

protected:
  Customer(const string& name) : Location(name) {}
  ~Customer() {}
};

class Port : public Location {
public:
  static Ptr<Port> PortNew(const string& name) {
    Ptr<Port> m = new Port(name);
    return m;
  }

protected:
  Port(const string& name) : Location(name) {}
  ~Port() {}
};

class EngineManager;

// Segments
class Segment : public Entity {
public:
  // returns the global name of the source location
  Ptr<Location> source() { return source_; }

  Mile length() { return length_; }
  void lengthIs(Mile _length) { length_ = _length; }

  Ptr<Segment> returnSegment() { return returnSegment_; }
  void returnSegmentIs(Ptr<Segment> returnSegment) { returnSegment_ = returnSegment; }

  Difficulty difficulty() { return difficulty_; }
  void difficultyIs(Difficulty _difficulty) { difficulty_ = _difficulty; }

  enum ExpeditedSupport {
    yes_ = 0,
    no_ = 1
  };

  ExpeditedSupport expeditedSupport() { return expeditedSupport_; };
  void expeditedSupportIs(ExpeditedSupport _expeditedSupport) {
    expeditedSupport_ = _expeditedSupport;
  }

  virtual Dollar cost(EngineManager* manager) = 0;

protected:
  Segment(const string& name) : Entity(name), name_(name), length_(0),
      difficulty_(0), expeditedSupport_(no_) { }
  virtual ~Segment() {};

  void sourceIsImpl(Ptr<Location> _loc) { source_ = _loc; }

private:
  string name_;
  Ptr<Location> source_;
  Mile length_;
  Ptr<Segment> returnSegment_;
  Difficulty difficulty_;
  ExpeditedSupport expeditedSupport_;
};

class BoatSegment : public Segment {
public:
  static Ptr<BoatSegment> BoatSegmentNew(const string& name) {
    Ptr<BoatSegment> m = new BoatSegment(name);
    return m;
  }

  void sourceIs(Ptr<BoatTerminal> _loc) { sourceIsImpl(_loc); }
  void sourceIs(Ptr<Customer> _loc) { sourceIsImpl(_loc); }
  void sourceIs(Ptr<Port> _loc) { sourceIsImpl(_loc); }

  Dollar cost(EngineManager* manager);

protected:
  BoatSegment(const string& name) : Segment(name) {};
  ~BoatSegment() {};
};

class TruckSegment : public Segment {
public:
  static Ptr<TruckSegment> TruckSegmentNew(const string& name) {
    Ptr<TruckSegment> m = new TruckSegment(name);
    return m;
  }
  void sourceIs(Ptr<TruckTerminal> _loc) { sourceIsImpl(_loc); }
  void sourceIs(Ptr<Customer> _loc) { sourceIsImpl(_loc); }
  void sourceIs(Ptr<Port> _loc) { sourceIsImpl(_loc); }

  Dollar cost(EngineManager* manager);

protected:
  TruckSegment(const string& name) : Segment(name) {};
  ~TruckSegment() {};
};

class PlaneSegment : public Segment {
public:
  static Ptr<PlaneSegment> PlaneSegmentNew(const string& name) {
    Ptr<PlaneSegment> m = new PlaneSegment(name);
    return m;
  }
  void sourceIs(Ptr<PlaneTerminal> _loc) { sourceIsImpl(_loc); }
  void sourceIs(Ptr<Customer> _loc) { sourceIsImpl(_loc); }
  void sourceIs(Ptr<Port> _loc) { sourceIsImpl(_loc); }

  Dollar cost(EngineManager* manager);

protected:
  PlaneSegment(const string& name) : Segment(name) {};
  ~PlaneSegment() {};
};

class Stats;

// TODO(rhau) make the enginemanager a notifier
class EngineManager : public Fwk::PtrInterface<EngineManager> {
public:
  EngineManager();
  ~EngineManager();

  Ptr<Entity> entity(string _name);

  Ptr<Stats> stats() { return stats_; }

  void boatFleetIs(Ptr<BoatFleet> _boatFleet) { boatFleet_ = _boatFleet; }
  Ptr<BoatFleet> boatFleet() { return boatFleet_; }
  void planeFleetIs(Ptr<PlaneFleet> _planeFleet) { planeFleet_ = _planeFleet; }
  Ptr<PlaneFleet> planeFleet() { return planeFleet_; }
  void truckFleetIs(Ptr<TruckFleet> _truckFleet) { truckFleet_ = _truckFleet; }
  Ptr<TruckFleet> truckFleet() { return truckFleet_; }

  void customerIs(Ptr<Customer> _customer);
  Ptr<Customer> customer(string _name);
  void portIs(Ptr<Port> port);
  Ptr<Port> port(string _name);
  void boatTerminalIs(Ptr<BoatTerminal> _boatTerminal);
  Ptr<BoatTerminal> boatTerminal(string _name);
  void truckTerminalIs(Ptr<TruckTerminal> _truckTerminal);
  Ptr<TruckTerminal> truckTerminal(string _name);
  void planeTerminalIs(Ptr<PlaneTerminal> _planeTerminal);
  Ptr<PlaneTerminal> planeTerminal(string _name);

  void boatSegmentIs(Ptr<BoatSegment> _boatSegment);
  Ptr<BoatSegment> boatSegment(string _name);
  void truckSegmentIs(Ptr<TruckSegment> _truckSegment);
  Ptr<TruckSegment> truckSegment(string _name);
  void planeSegmentIs(Ptr<PlaneSegment> _planeSegment);
  Ptr<PlaneSegment> planeSegment(string _name);

  Fwk::Ptr<Path> path(Fwk::Ptr<Location> start, Fwk::Ptr<Location> end);
  std::vector<Path> connectivity(
      Fwk::Ptr<Location> start, Mile _distance, Dollar _cost);

  class Notifiee : public Fwk::NamedInterface::Notifiee{
  public:
    virtual void onCustomerIs() = 0;
    virtual void onPortIs() = 0;
    virtual void onBoatTerminalIs() = 0;
    virtual void onTruckTerminalIs() = 0;
    virtual void onPlaneTerminalIs() = 0;
    virtual void onBoatSegmentIs() = 0;
    virtual void onTruckSegmentIs() = 0;
    virtual void onPlaneSegmentIs() = 0;

    virtual void onCustomerDel() = 0;
    virtual void onPortDel() = 0;
    virtual void onBoatTerminalDel() = 0;
    virtual void onTruckTerminalDel() = 0;
    virtual void onPlaneTerminalDel() = 0;
    virtual void onBoatSegmentDel() = 0;
    virtual void onTruckSegmentDel() = 0;
    virtual void onPlaneSegmentDel() = 0;
  };

private:
  Ptr<Stats> stats_;
  std::vector<Ptr<Notifiee> > notifiee_;

  // set which contains the used names of engine objects
  std::map<string, Ptr<Entity> > entityMap_;

  Ptr<BoatFleet> boatFleet_;
  Ptr<PlaneFleet> planeFleet_;
  Ptr<TruckFleet> truckFleet_;

  // hash maps which back the terminal locations
  std::map<string, Ptr<Customer> > customerMap_;
  std::map<string, Ptr<Port> > portMap_;
  std::map<string, Ptr<BoatTerminal> > boatTerminalMap_;
  std::map<string, Ptr<TruckTerminal> > truckTerminalMap_;
  std::map<string, Ptr<PlaneTerminal> > planeTerminalMap_;

  std::map<string, Ptr<BoatSegment> > boatSegmentMap_;
  std::map<string, Ptr<TruckSegment> > truckSegmentMap_;
  std::map<string, Ptr<PlaneSegment> > planeSegmentMap_;
};

// TODO: Change this to a notifiee
class Stats : public EngineManager::Notifiee {
public:
  Stats(const string& name);
  ~Stats();

  void onCustomerIs();
  void onPortIs();
  void onBoatTerminalIs();
  void onTruckTerminalIs();
  void onPlaneTerminalIs();
  void onBoatSegmentIs();
  void onTruckSegmentIs();
  void onPlaneSegmentIs();

  void onCustomerDel();
  void onPortDel();
  void onBoatTerminalDel();
  void onTruckTerminalDel();
  void onPlaneTerminalDel();
  void onBoatSegmentDel();
  void onTruckSegmentDel();
  void onPlaneSegmentDel();

  uint32_t customerCount() { return customerCount_; }
  void customerCountInc(int delta) { customerCount_ += delta; }
  uint32_t portCount() { return portCount_; }
  void portCountInc(int delta) { portCount_ += delta; }

  uint32_t boatTerminalCount() { return boatTerminalCount_; }
  void boatTerminalCountInc(int delta) { boatTerminalCount_ += delta; }
  uint32_t truckTerminalCount() { return truckTerminalCount_; }
  void truckTerminalCountInc(int delta) { truckTerminalCount_ += delta; }
  uint32_t planeTerminalCount() { return planeTerminalCount_; }
  void planeTerminalCountInc(int delta) { planeTerminalCount_ += delta; }

  uint32_t boatSegmentCount() { return boatSegmentCount_; }
  void boatSegmentCountInc(int delta) { boatTerminalCount_ += delta; }
  uint32_t truckSegmentCount() {return truckSegmentCount_; }
  void truckSegmentCountInc(int delta) { truckSegmentCount_ += delta; }
  uint32_t planeSegmentCount() { return planeSegmentCount_; }
  void planeSegmentCountInc(int delta) { planeSegmentCount_ += delta; }

  double expeditedPercentage() {
    return (double)expeditedSegmentCount_ /
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

} /* end namespace */

#endif
