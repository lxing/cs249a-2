#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
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

class EngineManager;

class Entity : public NamedInterface {
public:
  virtual void del() { }

protected:
  Entity(const string& name, Fwk::Ptr<EngineManager> em) :
      NamedInterface(name), em_(em) { }
  Fwk::Ptr<EngineManager> em_;
};


// Value types
class Mile : public Ordinal<Mile, double> {
public:
  Mile(double num) : Ordinal<Mile, double>(num) { 
    if (num < 0.0) throw "Mile out of range";
  }
};

class MilesPerHour : public Ordinal<MilesPerHour, double> {
public:
  MilesPerHour(double num) : Ordinal<MilesPerHour, double>(num) {
    if (num < 0.0) throw "Speed out of range";
  }
};

class Time : public Ordinal<Time, double> {
public:
  Time(double num) : Ordinal<Time, double>(num) { 
    if (num < 0.0) throw "Time out of range";
  }
};

class Capacity : public Nominal<Capacity, uint32_t> {
public:
  Capacity(uint32_t num) : Nominal<Capacity, uint32_t>(num) { }
};

class Difficulty : public Nominal<Difficulty, double> {
public:
  Difficulty(double num) : Nominal<Difficulty, double>(num) {
    if (num < 1.0 || num > 5.0) throw "Difficulty value out of range";
  }
};

class Dollar : public Ordinal<Dollar, double> {
public:
  Dollar(double num) : Ordinal<Dollar, double>(num) {
    if (num < 0.0) throw "Dollar value out of range";
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
  Fleet(const string& name, Fwk::Ptr<EngineManager> em) :
      Entity(name, em), speed_(0),
      capacity_(0), cost_(0) { }

private:
  MilesPerHour speed_;
  Capacity capacity_;
  Dollar cost_;
};

class BoatFleet : public Fleet {
public:
  static Ptr<BoatFleet> BoatFleetNew(
        const string& name, Fwk::Ptr<EngineManager> em) {
    Ptr<BoatFleet> m = new BoatFleet(name, em);
    return m;
  }

protected:
  BoatFleet(const string& name, Fwk::Ptr<EngineManager> em) :
      Fleet(name, em) { }
};

class PlaneFleet : public Fleet {
public:
  static Ptr<PlaneFleet> PlaneFleetNew(
        const string& name, Fwk::Ptr<EngineManager> em) {
    Ptr<PlaneFleet> m = new PlaneFleet(name, em);
    return m;
  }

protected:
  PlaneFleet(const string& name, Fwk::Ptr<EngineManager> em) :
      Fleet(name, em) { }
};

class TruckFleet : public Fleet {
public:
  static Ptr<TruckFleet> TruckFleetNew(
        const string& name, Fwk::Ptr<EngineManager> em) {
    Ptr<TruckFleet> m = new TruckFleet(name, em);
    return m;
  }

protected:
  TruckFleet(const string& name, Fwk::Ptr<EngineManager> em) :
      Fleet(name, em) {}
};

class Segment;
class Path;

// Locations
class Location : public Entity {
public:
  // TODO(rhau) onSegment, which adds a segment as a callback from SegmentNew
  virtual void del();

  virtual Ptr<Segment> segment(uint32_t _i) {
    Ptr<Segment> s = (_i < segments_.size()) ? segments_[_i] : NULL;
    return s;
  }

  virtual std::vector<Ptr<Segment> > segments() {
    return segments_;
  }

  virtual void segmentIs(Ptr<Segment> s) {
    segments_.push_back(s);
  }

  virtual void segmentDel(Ptr<Segment> s);

protected:
  Location(const string& name, Fwk::Ptr<EngineManager> em) : Entity(name, em) {}
  virtual ~Location() {}

private:
  std::vector<Ptr<Segment> > segments_;
};


class Terminal : public Location {
protected:
  Terminal(const string& name, Fwk::Ptr<EngineManager> em) :
      Location(name, em) {}
  ~Terminal() {}
};

// Terminals
class BoatTerminal : public Terminal {
public:
  static Ptr<BoatTerminal> BoatTerminalNew(
        const string& name, Fwk::Ptr<EngineManager> em) {
    Ptr<BoatTerminal> m = new BoatTerminal(name, em);
    return m;
  }

protected:
  BoatTerminal(const string& name, Fwk::Ptr<EngineManager> em) :
      Terminal(name, em) {}
  ~BoatTerminal() {}
};

class TruckTerminal : public Terminal {
public:
  static Ptr<TruckTerminal> TruckTerminalNew(
        const string& name, Fwk::Ptr<EngineManager> em) {
    Ptr<TruckTerminal> m = new TruckTerminal(name, em);
    return m;
  }

protected:
  TruckTerminal(const string& name, Fwk::Ptr<EngineManager> em) :
      Terminal(name, em) {}
  ~TruckTerminal() {}
};

class PlaneTerminal : public Terminal {
public:
  static Ptr<PlaneTerminal> PlaneTerminalNew(
        const string& name, Fwk::Ptr<EngineManager> em) {
    Ptr<PlaneTerminal> m = new PlaneTerminal(name, em);
    return m;
  }

protected:
  PlaneTerminal(const string& name, Fwk::Ptr<EngineManager> em) :
      Terminal(name, em) {}
  ~PlaneTerminal() {}
};

class Customer : public Location {
public:
  static Ptr<Customer> CustomerNew(
        const string& name, Fwk::Ptr<EngineManager> em) {
    Ptr<Customer> m = new Customer(name, em);
    return m;
  }

protected:
  Customer(const string& name, Fwk::Ptr<EngineManager> em) :
      Location(name, em) {}
  ~Customer() {}
};

class Port : public Location {
public:
  static Ptr<Port> PortNew(const string& name, Fwk::Ptr<EngineManager> em) {
    Ptr<Port> m = new Port(name, em);
    return m;
  }

protected:
  Port(const string& name, Fwk::Ptr<EngineManager> em) : Location(name, em) {}
  ~Port() {}
};

class EngineManager;

// Segments
class Segment : public Entity {
public:
  virtual void del();

  // returns the global name of the source location
  Ptr<Location> source() { return source_; }
  void sourceIs(Ptr<Location> _loc);
  Ptr<Location> destination();

  Mile length() { return length_; }
  void lengthIs(Mile _length) { length_ = _length; }

  Fwk::Ptr<Segment> returnSegment() { return returnSegment_; }
  void returnSegmentIs(Ptr<Segment> returnSegment);

  Difficulty difficulty() { return difficulty_; }
  void difficultyIs(Difficulty _difficulty) { difficulty_ = _difficulty; }

  enum ExpeditedSupport {
    yes_ = 0,
    no_ = 1
  };

  ExpeditedSupport expeditedSupport() { return expeditedSupport_; };
  void expeditedSupportIs(ExpeditedSupport _expeditedSupport);

  Dollar cost(EngineManager* manager, ExpeditedSupport expedited);
  virtual Time time(EngineManager* manager, ExpeditedSupport expedited) = 0;

protected:
  Segment(const string& name, Fwk::Ptr<EngineManager> em) :
      Entity(name, em), name_(name), length_(0), difficulty_(1.0),
      expeditedSupport_(no_) { }
  virtual ~Segment() {};


private:
  string name_;
  Ptr<Location> source_;
  Mile length_;
  Ptr<Segment> returnSegment_;
  Difficulty difficulty_;
  ExpeditedSupport expeditedSupport_;
};

class Path : public PtrInterface<Path> {
public:
  Path() : pathCost_(0), pathLength_(0), pathTime_(0) { }
  ~Path() { }

  Dollar cost() { return pathCost_; }
  Mile length() { return pathLength_; }
  Time time() { return pathTime_; }
  Segment::ExpeditedSupport expeditedSupport() { return expeditedSupport_; }

  string tostring();

  static Fwk::Ptr<Path> copy(Fwk::Ptr<Path> path);

  void addSegment(Fwk::Ptr<Segment> segment, Dollar segmentCost, Mile length,
        Time time) {
    pathCost_ = pathCost_.value() + segmentCost.value();
    pathLength_ = pathLength_.value() + length.value();
    pathTime_ = pathTime_.value() + time.value();
    segment_.push_back(segment);
  }

  std::vector<Fwk::Ptr<Segment> > segments() {
    return segment_;
  }

private:
  Segment::ExpeditedSupport expeditedSupport_;
  Dollar pathCost_;
  Mile pathLength_;
  Time pathTime_;
  std::vector<Fwk::Ptr<Segment> > segment_;
};

class BoatSegment : public Segment {
public:
  static Ptr<BoatSegment> BoatSegmentNew(
      const string& name, Fwk::Ptr<EngineManager> em) {
    Ptr<BoatSegment> m = new BoatSegment(name, em);
    return m;
  }

  void sourceIs(Ptr<BoatTerminal> _loc) { Segment::sourceIs(_loc); }
  void sourceIs(Ptr<Customer> _loc) { Segment::sourceIs(_loc); }
  void sourceIs(Ptr<Port> _loc) { Segment::sourceIs(_loc); }

  Time time(EngineManager* manager, ExpeditedSupport expedited);

protected:
  BoatSegment(const string& name, Fwk::Ptr<EngineManager> em) :
      Segment(name, em) {};
  ~BoatSegment() {};
};

class TruckSegment : public Segment {
public:
  static Ptr<TruckSegment> TruckSegmentNew(
      const string& name, Fwk::Ptr<EngineManager> em) {
    Ptr<TruckSegment> m = new TruckSegment(name, em);
    return m;
  }
  void sourceIs(Ptr<TruckTerminal> _loc) { Segment::sourceIs(_loc); }
  void sourceIs(Ptr<Customer> _loc) { Segment::sourceIs(_loc); }
  void sourceIs(Ptr<Port> _loc) { Segment::sourceIs(_loc); }

  Time time(EngineManager* manager, ExpeditedSupport expedited);

protected:
  TruckSegment(
      const string& name, Fwk::Ptr<EngineManager> em) : Segment(name, em) {};
  ~TruckSegment() {};
};

class PlaneSegment : public Segment {
public:
  static Ptr<PlaneSegment> PlaneSegmentNew(
      const string& name, Fwk::Ptr<EngineManager> em) {
    Ptr<PlaneSegment> m = new PlaneSegment(name, em);
    return m;
  }
  void sourceIs(Ptr<PlaneTerminal> _loc) { Segment::sourceIs(_loc); }
  void sourceIs(Ptr<Customer> _loc) { Segment::sourceIs(_loc); }
  void sourceIs(Ptr<Port> _loc) { Segment::sourceIs(_loc); }

  Time time(EngineManager* manager, ExpeditedSupport expedited);

protected:
  PlaneSegment(
      const string& name, Fwk::Ptr<EngineManager> em) : Segment(name, em) {};
  ~PlaneSegment() {};
};

class Stats;

// TODO(rhau) make the enginemanager a notifier
class EngineManager : public Fwk::PtrInterface<EngineManager> {
public:
  EngineManager();
  ~EngineManager();

  Ptr<Entity> entity(string _name);
  Ptr<Location> location(string _name);
  void entityDel(string _name);

  class Notifiee;

  void notifieeIs(Fwk::Ptr<Notifiee> notifiee) {
    notifiee_.push_back(notifiee);
  }

  Ptr<Stats> stats() { return stats_; }

  // Fleet Attributes
  Ptr<BoatFleet> boatFleet() { return boatFleet_; }
  void boatFleetIs(Ptr<BoatFleet> _boatFleet) { boatFleet_ = _boatFleet; }

  Ptr<PlaneFleet> planeFleet() { return planeFleet_; }
  void planeFleetIs(Ptr<PlaneFleet> _planeFleet) { planeFleet_ = _planeFleet; }

  Ptr<TruckFleet> truckFleet() { return truckFleet_; }
  void truckFleetIs(Ptr<TruckFleet> _truckFleet) { truckFleet_ = _truckFleet; }

  // Location Attributes
  Ptr<Customer> customer(string _name);
  void customerIs(Ptr<Customer> _customer);
  void customerDel(Ptr<Customer> _customer);

  Ptr<Port> port(string _name);
  void portIs(Ptr<Port> port);
  void portDel(Ptr<Port> port);;

  Ptr<BoatTerminal> boatTerminal(string _name);
  void boatTerminalIs(Ptr<BoatTerminal> _boatTerminal);
  void boatTerminalDel(Ptr<BoatTerminal> _boatTerminal);

  Ptr<TruckTerminal> truckTerminal(string _name);
  void truckTerminalIs(Ptr<TruckTerminal> _truckTerminal);
  void truckTerminalDel(Ptr<TruckTerminal> _truckTerminal);

  Ptr<PlaneTerminal> planeTerminal(string _name);
  void planeTerminalIs(Ptr<PlaneTerminal> _planeTerminal);
  void planeTerminalDel(Ptr<PlaneTerminal> _planeTerminal);

  // Segment Attributes
  Ptr<BoatSegment> boatSegment(string _name);
  void boatSegmentIs(Ptr<BoatSegment> _boatSegment);
  void boatSegmentDel(Ptr<BoatSegment> _boatSegment);

  Ptr<TruckSegment> truckSegment(string _name);
  void truckSegmentIs(Ptr<TruckSegment> _truckSegment);
  void truckSegmentDel(Ptr<TruckSegment> _truckSegment);

  Ptr<PlaneSegment> planeSegment(string _name);
  void planeSegmentIs(Ptr<PlaneSegment> _planeSegment);
  void planeSegmentDel(Ptr<PlaneSegment> _planeSegment);

  // Connectivity Attributes
  std::vector<Fwk::Ptr<Path> > connect(Fwk::Ptr<Location> start,
      Fwk::Ptr<Location> end);
  std::vector<Fwk::Ptr<Path> > explore(
      Fwk::Ptr<Location> start, Mile _distance, Dollar _cost, Time _time,
      Segment::ExpeditedSupport _expedited);

  // Notifiee
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
    virtual void onExpeditedSegmentIs() = 0;

    virtual void onCustomerDel() = 0;
    virtual void onPortDel() = 0;
    virtual void onBoatTerminalDel() = 0;
    virtual void onTruckTerminalDel() = 0;
    virtual void onPlaneTerminalDel() = 0;
    virtual void onBoatSegmentDel() = 0;
    virtual void onTruckSegmentDel() = 0;
    virtual void onPlaneSegmentDel() = 0;
    virtual void onExpeditedSegmentDel() = 0;
  };

protected:
  std::vector<Fwk::Ptr<Path> > connectImpl(
    Fwk::Ptr<Location> start, Fwk::Ptr<Location> end,
    Segment::ExpeditedSupport expedited);

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
  void onExpeditedSegmentIs();

  void onCustomerDel();
  void onPortDel();
  void onBoatTerminalDel();
  void onTruckTerminalDel();
  void onPlaneTerminalDel();
  void onBoatSegmentDel();
  void onTruckSegmentDel();
  void onPlaneSegmentDel();
  void onExpeditedSegmentDel();

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
  void boatSegmentCountInc(int delta) { boatSegmentCount_ += delta; }
  uint32_t truckSegmentCount() {return truckSegmentCount_; }
  void truckSegmentCountInc(int delta) { truckSegmentCount_ += delta; }
  uint32_t planeSegmentCount() { return planeSegmentCount_; }
  void planeSegmentCountInc(int delta) { planeSegmentCount_ += delta; }

  uint32_t expeditedSegmentCount() { return expeditedSegmentCount_; }
  void expeditedSegmentCountInc(int delta) { expeditedSegmentCount_ += delta; }

  double expeditedPercentage() {
    return (double)expeditedSegmentCount_ /
        (boatSegmentCount_ + truckSegmentCount_ + planeSegmentCount_) * 100.0;
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
