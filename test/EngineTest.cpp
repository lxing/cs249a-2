#include <vector>

#include "gtest/gtest.h"
#include "../Engine.h"

#include <string>
#include <set>
#include <ostream>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/signal.h>
#include <boost/lexical_cast.hpp>
#include "Instance.h"

using namespace std;

/******************/
/* Initialization */
/******************/

class EngineTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    em = new Shipping::EngineManager();
    Ptr<Shipping::BoatFleet> bf =
        Shipping::BoatFleet::BoatFleetNew("BoatFleet", em);
    bf->speedIs(100);
    bf->capacityIs(100);
    bf->costIs(100);
    em->boatFleetIs(bf);

    Ptr<Shipping::TruckFleet> tf =
        Shipping::TruckFleet::TruckFleetNew("TruckFleet", em);
    tf->speedIs(150);
    tf->capacityIs(150);
    tf->costIs(150);
    em->truckFleetIs(tf);

    Ptr<Shipping::PlaneFleet> pf =
        Shipping::PlaneFleet::PlaneFleetNew("PlaneFleet", em);
    pf->speedIs(200);
    pf->capacityIs(200);
    pf->costIs(200);
    em->planeFleetIs(pf);

    Ptr<Shipping::Customer> c = 
        Shipping::Customer::CustomerNew("Customer", em);
    em->customerIs(c);

    Ptr<Shipping::Port> port =
        Shipping::Port::PortNew("Port", em);
    em->portIs(port);

    Ptr<Shipping::BoatTerminal> bt =
        Shipping::BoatTerminal::BoatTerminalNew("BoatTerminal", em);
    em->boatTerminalIs(bt);

    Ptr<Shipping::TruckTerminal> tt =
        Shipping::TruckTerminal::TruckTerminalNew("TruckTerminal", em);
    em->truckTerminalIs(tt);

    Ptr<Shipping::PlaneTerminal> pt =
        Shipping::PlaneTerminal::PlaneTerminalNew("PlaneTerminal", em);
    em->planeTerminalIs(pt);

    Ptr<Shipping::BoatTerminal> bta =
    Shipping::BoatTerminal::BoatTerminalNew("BoatTerminalA", em);
    em->boatTerminalIs(bta);

    Ptr<Shipping::BoatTerminal> btb =
        Shipping::BoatTerminal::BoatTerminalNew("BoatTerminalB", em);
    em->boatTerminalIs(btb);

    Ptr<Shipping::BoatTerminal> btc =
        Shipping::BoatTerminal::BoatTerminalNew("BoatTerminalC", em);
    em->boatTerminalIs(btc);

    Ptr<Shipping::BoatSegment> bsa =
        Shipping::BoatSegment::BoatSegmentNew("BoatSegmentA", em);
    bsa->sourceIs(bta);
    em->boatSegmentIs(bsa);

    Ptr<Shipping::BoatSegment> bsb =
        Shipping::BoatSegment::BoatSegmentNew("BoatSegmentB", em);
    bsb->sourceIs(btb);
    em->boatSegmentIs(bsb);

    Ptr<Shipping::BoatSegment> bsbc =
        Shipping::BoatSegment::BoatSegmentNew("BoatSegmentBC", em);
    bsbc->sourceIs(btb);
    em->boatSegmentIs(bsbc);

    Ptr<Shipping::BoatSegment> bscb =
        Shipping::BoatSegment::BoatSegmentNew("BoatSegmentCB", em);
    bscb->sourceIs(btc);
    em->boatSegmentIs(bscb);

    bsa->lengthIs(Shipping::Mile(50));
    bsa->difficultyIs(Shipping::Difficulty(1));
    bsa->expeditedSupportIs(Shipping::Segment::no_);

    bsb->lengthIs(Shipping::Mile(50));
    bsb->difficultyIs(Shipping::Difficulty(1));
    bsb->expeditedSupportIs(Shipping::Segment::no_);

    bsbc->lengthIs(Shipping::Mile(50));
    bsbc->difficultyIs(Shipping::Difficulty(1));
    bsbc->expeditedSupportIs(Shipping::Segment::no_);

    bscb->lengthIs(Shipping::Mile(50));
    bscb->difficultyIs(Shipping::Difficulty(1));
    bscb->expeditedSupportIs(Shipping::Segment::no_);

    bsa->returnSegmentIs(bsb);
    bsbc->returnSegmentIs(bscb);
  }

  Fwk::Ptr<Shipping::EngineManager> em;
};

/*******************/
/* Test Engine */
/*******************/

TEST_F(EngineTest, BoatFleetTest) {
  Ptr<Shipping::BoatFleet> bf = em->boatFleet();

  ASSERT_EQ(100, bf->speed().value());
  ASSERT_EQ(100, bf->capacity().value());
  ASSERT_EQ(100, bf->cost().value());
}

TEST_F(EngineTest, TruckFleetTest) {
  Ptr<Shipping::TruckFleet> tf = em->truckFleet();

  ASSERT_EQ(150, tf->speed().value());
  ASSERT_EQ(150, tf->capacity().value());
  ASSERT_EQ(150, tf->cost().value());
}

TEST_F(EngineTest, PlaneFleetTest) {
  Ptr<Shipping::PlaneFleet> pf = em->planeFleet();

  ASSERT_EQ(200, pf->speed().value());
  ASSERT_EQ(200, pf->capacity().value());
  ASSERT_EQ(200, pf->cost().value());
}

TEST_F(EngineTest, BoatSegmentTest) {
  Ptr<Shipping::BoatTerminal> bta = em->boatTerminal("BoatTerminalA");
  Ptr<Shipping::BoatTerminal> btb = em->boatTerminal("BoatTerminalB");
  Ptr<Shipping::BoatTerminal> btc = em->boatTerminal("BoatTerminalC");

  Ptr<Shipping::BoatSegment> bsa = em->boatSegment("BoatSegmentA");
  Ptr<Shipping::BoatSegment> bsb = em->boatSegment("BoatSegmentB");
  Ptr<Shipping::BoatSegment> bsbc = em->boatSegment("BoatSegmentBC");
  Ptr<Shipping::BoatSegment> bscb = em->boatSegment("BoatSegmentCB");

  bsa->lengthIs(Shipping::Mile(50));
  bsa->difficultyIs(Shipping::Difficulty(1));
  bsa->expeditedSupportIs(Shipping::Segment::no_);

  bsb->lengthIs(Shipping::Mile(50));
  bsb->difficultyIs(Shipping::Difficulty(1));
  bsb->expeditedSupportIs(Shipping::Segment::no_);

  // bsa->returnSegmentIs(bsb);
  // bsbc->returnSegmentIs(bscb);

  ASSERT_EQ("BoatTerminalA", bsa->source()->name());
  ASSERT_EQ("BoatTerminalB", bsb->source()->name());
  
  ASSERT_EQ(bsa->returnSegment()->name(), "BoatSegmentB");
  ASSERT_EQ(bsb->returnSegment()->name(), "BoatSegmentA");
  ASSERT_EQ(bsbc->returnSegment()->name(), "BoatSegmentCB");
  ASSERT_EQ(bscb->returnSegment()->name(), "BoatSegmentBC");

  ASSERT_EQ(1, bta->segments().size());
  ASSERT_EQ(2, btb->segments().size());
}

TEST_F(EngineTest, CustomerTest) {
  Ptr<Shipping::Customer> c = em->customer("Customer");
  Ptr<Shipping::Customer> nullCustomer = NULL;
  ASSERT_NE(nullCustomer, c);
}

TEST_F(EngineTest, PortTest) {
  Ptr<Shipping::Port> port = em->port("Port");
  Ptr<Shipping::Port> nullPort = NULL;
  ASSERT_NE(nullPort, port);
}

TEST_F(EngineTest, BoatTerminalTest) {
  Ptr<Shipping::BoatTerminal> bt = em->boatTerminal("BoatTerminal");
  Ptr<Shipping::BoatTerminal> nullTerminal = NULL;
  ASSERT_NE(nullTerminal, bt);
}

TEST_F(EngineTest, TruckTerminalTest) {
  Ptr<Shipping::TruckTerminal> tt = em->truckTerminal("TruckTerminal");
  Ptr<Shipping::TruckTerminal> nullTerminal = NULL;
  ASSERT_NE(nullTerminal, tt);
}

TEST_F(EngineTest, PlaneTerminalTest) {
  Ptr<Shipping::PlaneTerminal> pt = em->planeTerminal("PlaneTerminal");
  Ptr<Shipping::PlaneTerminal> nullTerminal = NULL;
  ASSERT_NE(nullTerminal, pt);
}

TEST_F(EngineTest, SimpleConnectOneStepTest) {
  Ptr<Shipping::BoatTerminal> bta = em->boatTerminal("BoatTerminalA");
  Ptr<Shipping::BoatTerminal> btb = em->boatTerminal("BoatTerminalB");

  std::vector<Fwk::Ptr<Shipping::Path> > paths = em->connect(bta, btb);
  ASSERT_EQ(1, paths.size());
}

TEST_F(EngineTest, SimpleConnectTwoStepsTest) {
  Ptr<Shipping::BoatTerminal> bta = em->boatTerminal("BoatTerminalA");
  Ptr<Shipping::BoatTerminal> btc = em->boatTerminal("BoatTerminalC");

  std::vector<Fwk::Ptr<Shipping::Path> > paths = em->connect(bta, btc);
  ASSERT_EQ(1, paths.size());
}

TEST_F(EngineTest, SimpleExploreLineTest) {
  Ptr<Shipping::BoatTerminal> bta = em->boatTerminal("BoatTerminalA");

  Shipping::Mile distance(100);
  Shipping::Dollar cost(100000);
  Shipping::Time t(1);
  Shipping::Segment::ExpeditedSupport expedited =
      Shipping::Segment::no_;

  std::vector<Fwk::Ptr<Shipping::Path> > paths =
      em->explore(bta, distance, cost, t, expedited);

  ASSERT_EQ(2, paths.size());
}

TEST_F(EngineTest, SimpleExploreGraphTest) {
  Ptr<Shipping::BoatTerminal> bta = em->boatTerminal("BoatTerminalA");

  Ptr<Shipping::BoatTerminal> btd =
      Shipping::BoatTerminal::BoatTerminalNew("BoatTerminalD", em);
  em->boatTerminalIs(btd);

  Ptr<Shipping::BoatSegment> bsad =
      Shipping::BoatSegment::BoatSegmentNew("BoatSegmentAD", em);
  bsad->sourceIs(bta);
  em->boatSegmentIs(bsad);

  Ptr<Shipping::BoatSegment> bsda =
      Shipping::BoatSegment::BoatSegmentNew("BoatSegmentDA", em);
  bsda->sourceIs(btd);
  em->boatSegmentIs(bsda);

  bsad->returnSegmentIs(bsda);

  Shipping::Mile distance(100);
  Shipping::Dollar cost(10000);
  Shipping::Time t(1);
  Shipping::Segment::ExpeditedSupport expedited =
      Shipping::Segment::no_;

  std::vector<Fwk::Ptr<Shipping::Path> > paths =
      em->explore(bta, distance, cost, t, expedited);

  ASSERT_EQ(3, paths.size());
}

TEST_F(EngineTest, SimpleExploreGraphOutOfBoundaryTest) {
  Ptr<Shipping::BoatTerminal> bta = em->boatTerminal("BoatTerminalA");

  Ptr<Shipping::BoatTerminal> btd =
      Shipping::BoatTerminal::BoatTerminalNew("BoatTerminalD", em);
  em->boatTerminalIs(btd);

  Ptr<Shipping::BoatSegment> bsad =
      Shipping::BoatSegment::BoatSegmentNew("BoatSegmentAD", em);
  bsad->sourceIs(bta);
  bsad->lengthIs(200);
  em->boatSegmentIs(bsad);

  Ptr<Shipping::BoatSegment> bsda =
      Shipping::BoatSegment::BoatSegmentNew("BoatSegmentDA", em);
  bsda->sourceIs(btd);
  em->boatSegmentIs(bsda);

  bsad->returnSegmentIs(bsda);

  Shipping::Mile distance(100);
  Shipping::Dollar cost(10000);
  Shipping::Time t(1);
  Shipping::Segment::ExpeditedSupport expedited =
      Shipping::Segment::no_;

  std::vector<Fwk::Ptr<Shipping::Path> > paths =
      em->explore(bta, distance, cost, t, expedited);

  ASSERT_EQ(2, paths.size());
}

TEST_F(EngineTest, SimpleExploreGraphNegativeTest) {
  Ptr<Shipping::BoatTerminal> bta = em->boatTerminal("BoatTerminalA");

  Shipping::Mile distance(101);
  Shipping::Dollar cost(101);
  Shipping::Time t(2);
  Shipping::Segment::ExpeditedSupport expedited =
      Shipping::Segment::yes_;

  std::vector<Fwk::Ptr<Shipping::Path> > paths =
      em->explore(bta, distance, cost, t, expedited);

  ASSERT_EQ(0, paths.size());
}



TEST_F(EngineTest, StatsCountUpTest) {
  Fwk::Ptr<Shipping::Stats> stats = em->stats();
  ASSERT_EQ(1, stats->customerCount());
  ASSERT_EQ(1, stats->portCount());
  ASSERT_EQ(4, stats->boatTerminalCount());
  ASSERT_EQ(1, stats->truckTerminalCount());
  ASSERT_EQ(1, stats->planeTerminalCount());
  ASSERT_EQ(4, stats->boatSegmentCount());
  ASSERT_EQ(0, stats->expeditedSegmentCount());
}

TEST_F(EngineTest, StatsCountDownTest) {
  Fwk::Ptr<Shipping::Stats> stats = em->stats();

  em->entityDel("Customer");
  ASSERT_EQ(0, stats->customerCount());
  
  em->entityDel("Port");
  ASSERT_EQ(0, stats->portCount());

  em->entityDel("BoatTerminal");
  ASSERT_EQ(3, stats->boatTerminalCount());

  em->entityDel("TruckTerminal");
  ASSERT_EQ(0, stats->truckTerminalCount());

  em->entityDel("PlaneTerminal");
  ASSERT_EQ(0, stats->planeTerminalCount());

  em->entityDel("BoatSegmentA");
  ASSERT_EQ(3, stats->boatSegmentCount());

  Ptr<Shipping::BoatSegment> bsa = em->boatSegment("BoatSegmentA");
  bsa->expeditedSupportIs(Shipping::Segment::yes_);
  Ptr<Shipping::BoatSegment> bsb = em->boatSegment("BoatSegmentB");
  bsb->expeditedSupportIs(Shipping::Segment::yes_);
  Ptr<Shipping::BoatSegment> bsbc = em->boatSegment("BoatSegmentBC");
  bsbc->expeditedSupportIs(Shipping::Segment::yes_);
  Ptr<Shipping::BoatSegment> bscb = em->boatSegment("BoatSegmentCB");
  bscb->expeditedSupportIs(Shipping::Segment::yes_);
  ASSERT_EQ(4, stats->expeditedSegmentCount());

  bsa->expeditedSupportIs(Shipping::Segment::no_);
  ASSERT_EQ(3, stats->expeditedSegmentCount());

  em->entityDel("BoatSegmentB");
  ASSERT_EQ(2, stats->expeditedSegmentCount());
}

class TestHarness : public ::testing::Test {

};

// TEST_F(TestHarness, TestStatsAttributes) {
//     Ptr<Instance::Manager> m = shippingInstanceManager();
//     Ptr<Instance> fleet = m->instanceNew("fleet", "Fleet");
//     Ptr<Instance> stats = m->instanceNew("stats", "Stats");

//     /* Make some instanes of varying types, then test stats */
//     m->instanceNew("seg1", "Boat segment");
//     m->instanceNew("seg2", "Plane segment");
//     m->instanceNew("seg3", "Plane segment");
//     m->instanceNew("seg4", "Truck segment");
//     m->instanceNew("seg5", "Truck segment");
//     m->instanceNew("seg6", "Truck segment");
//     m->instanceNew("seg7", "Truck segment");
//     m->instanceNew("seg8", "Truck segment");

//     m->instanceNew("loc1", "Customer");
//     m->instanceNew("loc2", "Port");
//     m->instanceNew("loc3", "Truck terminal");
//     m->instanceNew("loc4", "Boat terminal");
//     m->instanceNew("loc5", "Plane terminal");
//     m->instanceNew("loc6", "Truck terminal");
//     m->instanceNew("loc7", "Truck terminal");
//     m->instanceNew("loc8", "Truck terminal");

//     EQUAL(stats->attribute("Boat segment"), "1");
//     EQUAL(stats->attribute("Plane segment"), "2");
//     EQUAL(stats->attribute("Truck segment"), "5");
//     EQUAL(stats->attribute("Customer"), "1");
//     EQUAL(stats->attribute("Port"), "1");
//     EQUAL(stats->attribute("Truck terminal"), "4");
//     EQUAL(stats->attribute("Plane terminal"), "1");
//     EQUAL(stats->attribute("Boat terminal"), "1");
// }