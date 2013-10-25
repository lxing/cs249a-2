#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <vector>

#include "Instance.h"
#include "NamedInterface.h"
#include "Nominal.h"
#include "Ptr.h"

using Fwk::NamedInterface;

namespace Shipping {

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

class Segment;

// Create your rep/engine interface here.
class Location : public NamedInterface {
public:
  Location(string _name);
  virtual ~Location();

  void segment(int _i);
  void segmentIs(string _name);

private:
  std::vector<Ptr<Segment> > segments_;
};

class Customer : public Location {

};

class Port : public Location {

};

class Terminal : public Location {

};

class BoatTerminal : public Terminal {

};

class TruckTerminal : public Terminal {

};

class PlaneTerminal : public Terminal {

};


class Segment : public NamedInterface {
public:
  Segment(string _name);
  virtual ~Segment();

  string source(string _name);
  virtual void sourceIs(string _name, Ptr<Location> _loc);

  Mile length();
  void lengthIs(Mile _length);

  string returnSegment();
  void returnSegmentIs(string _name);

  Difficulty difficulty();
  void difficultyIs(Difficulty _difficulty);

  enum ExpeditedSupport {
    yes_ = 0,
    no_ = 1
  };

  ExpeditedSupport expediteSupport();
  void expediteSupportIs(ExpeditedSupport _expedited_support);

private:
  string source_;
  Mile length_;
  string returnSegment_;
  Difficulty difficulty_;
  ExpeditedSupport expedited_support_;
};

class BoatSegment : public Segment {
public:
  void sourceIs(string _name, Ptr<Location> _loc);
};

class TruckSegment : public Segment {
public:
  void sourceIs(string _name, Ptr<Location> _loc);
};

class PlaneSegment : public Segment {
public:
  void sourceIs(string _name, Ptr<Location> _loc);
};


// TODO: Change this to a reactor
class Stats : public NamedInterface {};
 

} /* end namespace */

#endif
