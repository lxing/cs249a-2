#include <vector>

#include "gtest/gtest.h"
#include "../Engine.h"

/******************/
/* Initialization */
/******************/

class EngineTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    Ptr<Shipping::BoatFleet> bf =
        Shipping::BoatFleet::BoatFleetNew("BoatFleet");
    bf->speedIs(100);
    bf->capacityIs(100);
    bf->costIs(100);
    em.boatFleetIs(bf);

    Ptr<Shipping::TruckFleet> tf =
        Shipping::TruckFleet::TruckFleetNew("TruckFleet");
    tf->speedIs(150);
    tf->capacityIs(150);
    tf->costIs(150);
    em.truckFleetIs(tf);

    Ptr<Shipping::PlaneFleet> pf =
        Shipping::PlaneFleet::PlaneFleetNew("PlaneFleet");
    pf->speedIs(200);
    pf->capacityIs(200);
    pf->costIs(200);
    em.planeFleetIs(pf);

    Ptr<Shipping::Customer> c = 
        Shipping::Customer::CustomerNew("Customer");
    em.customerIs(c);

    Ptr<Shipping::Port> port =
        Shipping::Port::PortNew("Port");
    em.portIs(port);

    Ptr<Shipping::BoatTerminal> bt =
        Shipping::BoatTerminal::BoatTerminalNew("BoatTerminal");
    em.boatTerminalIs(bt);

    Ptr<Shipping::TruckTerminal> tt =
        Shipping::TruckTerminal::TruckTerminalNew("TruckTerminal");
    em.truckTerminalIs(tt);

    Ptr<Shipping::PlaneTerminal> pt =
        Shipping::PlaneTerminal::PlaneTerminalNew("PlaneTerminal");
    em.planeTerminalIs(pt);

    Ptr<Shipping::BoatTerminal> bta =
    Shipping::BoatTerminal::BoatTerminalNew("BoatTerminalA");
    em.boatTerminalIs(bta);

    Ptr<Shipping::BoatTerminal> btb =
        Shipping::BoatTerminal::BoatTerminalNew("BoatTerminalB");
    em.boatTerminalIs(btb);

    Ptr<Shipping::BoatTerminal> btc =
        Shipping::BoatTerminal::BoatTerminalNew("BoatTerminalC");
    em.boatTerminalIs(btc);

    Ptr<Shipping::BoatSegment> bsa =
        Shipping::BoatSegment::BoatSegmentNew("BoatSegmentA");
    bsa->sourceIs(bta);
    em.boatSegmentIs(bsa);

    Ptr<Shipping::BoatSegment> bsb =
        Shipping::BoatSegment::BoatSegmentNew("BoatSegmentB");
    bsb->sourceIs(btb);
    em.boatSegmentIs(bsb);

    Ptr<Shipping::BoatSegment> bsbc =
        Shipping::BoatSegment::BoatSegmentNew("BoatSegmentBC");
    bsbc->sourceIs(btb);
    em.boatSegmentIs(bsbc);

    Ptr<Shipping::BoatSegment> bscb =
        Shipping::BoatSegment::BoatSegmentNew("BoatSegmentCB");
    bscb->sourceIs(btc);
    em.boatSegmentIs(bscb);

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

  Shipping::EngineManager em;
};

/*******************/
/* Test Engine */
/*******************/

TEST_F(EngineTest, BoatFleetTest) {
  Ptr<Shipping::BoatFleet> bf = em.boatFleet();

  ASSERT_EQ(100, bf->speed().value());
  ASSERT_EQ(100, bf->capacity().value());
  ASSERT_EQ(100, bf->cost().value());
}

TEST_F(EngineTest, TruckFleetTest) {
  Ptr<Shipping::TruckFleet> tf = em.truckFleet();

  ASSERT_EQ(150, tf->speed().value());
  ASSERT_EQ(150, tf->capacity().value());
  ASSERT_EQ(150, tf->cost().value());
}

TEST_F(EngineTest, PlaneFleetTest) {
  Ptr<Shipping::PlaneFleet> pf = em.planeFleet();

  ASSERT_EQ(200, pf->speed().value());
  ASSERT_EQ(200, pf->capacity().value());
  ASSERT_EQ(200, pf->cost().value());
}

TEST_F(EngineTest, BoatSegmentTest) {
  Ptr<Shipping::BoatTerminal> bta = em.boatTerminal("BoatTerminalA");
  Ptr<Shipping::BoatTerminal> btb = em.boatTerminal("BoatTerminalB");
  Ptr<Shipping::BoatTerminal> btc = em.boatTerminal("BoatTerminalC");

  Ptr<Shipping::BoatSegment> bsa = em.boatSegment("BoatSegmentA");
  Ptr<Shipping::BoatSegment> bsb = em.boatSegment("BoatSegmentB");
  Ptr<Shipping::BoatSegment> bsbc = em.boatSegment("BoatSegmentBC");
  Ptr<Shipping::BoatSegment> bscb = em.boatSegment("BoatSegmentCB");

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
  Ptr<Shipping::Customer> c = em.customer("Customer");
  Ptr<Shipping::Customer> nullCustomer = NULL;
  ASSERT_NE(nullCustomer, c);
}

TEST_F(EngineTest, PortTest) {
  Ptr<Shipping::Port> port = em.port("Port");
  Ptr<Shipping::Port> nullPort = NULL;
  ASSERT_NE(nullPort, port);
}

TEST_F(EngineTest, BoatTerminalTest) {
  Ptr<Shipping::BoatTerminal> bt = em.boatTerminal("BoatTerminal");
  Ptr<Shipping::BoatTerminal> nullTerminal = NULL;
  ASSERT_NE(nullTerminal, bt);
}

TEST_F(EngineTest, TruckTerminalTest) {
  Ptr<Shipping::TruckTerminal> tt = em.truckTerminal("TruckTerminal");
  Ptr<Shipping::TruckTerminal> nullTerminal = NULL;
  ASSERT_NE(nullTerminal, tt);
}

TEST_F(EngineTest, PlaneTerminalTest) {
  Ptr<Shipping::PlaneTerminal> pt = em.planeTerminal("PlaneTerminal");
  Ptr<Shipping::PlaneTerminal> nullTerminal = NULL;
  ASSERT_NE(nullTerminal, pt);
}

TEST_F(EngineTest, SimpleConnectOneStepTest) {
  Ptr<Shipping::BoatTerminal> bta = em.boatTerminal("BoatTerminalA");
  Ptr<Shipping::BoatTerminal> btb = em.boatTerminal("BoatTerminalB");

  std::vector<Fwk::Ptr<Shipping::Path> > paths = em.connect(bta, btb);
  ASSERT_EQ(1, paths.size());
}

TEST_F(EngineTest, SimpleConnectTwoStepsTest) {
  Ptr<Shipping::BoatTerminal> bta = em.boatTerminal("BoatTerminalA");
  Ptr<Shipping::BoatTerminal> btc = em.boatTerminal("BoatTerminalC");

  std::vector<Fwk::Ptr<Shipping::Path> > paths = em.connect(bta, btc);
  ASSERT_EQ(1, paths.size());
}

TEST_F(EngineTest, SimpleExploreLineTest) {
  Ptr<Shipping::BoatTerminal> bta = em.boatTerminal("BoatTerminalA");

  Shipping::Mile distance(100);
  Shipping::Dollar cost(100);
  Shipping::Time t(1);
  Shipping::Segment::ExpeditedSupport expedited =
      Shipping::Segment::no_;

  std::vector<Fwk::Ptr<Shipping::Path> > paths =
      em.explore(bta, distance, cost, t, expedited);

  ASSERT_EQ(2, paths.size());
}

TEST_F(EngineTest, SimpleExploreGraphTest) {
  Ptr<Shipping::BoatTerminal> bta = em.boatTerminal("BoatTerminalA");

  Ptr<Shipping::BoatTerminal> btd =
      Shipping::BoatTerminal::BoatTerminalNew("BoatTerminalD");
  em.boatTerminalIs(btd);

  Ptr<Shipping::BoatSegment> bsad =
      Shipping::BoatSegment::BoatSegmentNew("BoatSegmentAD");
  bsad->sourceIs(bta);
  em.boatSegmentIs(bsad);

  Ptr<Shipping::BoatSegment> bsda =
      Shipping::BoatSegment::BoatSegmentNew("BoatSegmentDA");
  bsda->sourceIs(btd);
  em.boatSegmentIs(bsda);

  bsad->returnSegmentIs(bsda);

  Shipping::Mile distance(100);
  Shipping::Dollar cost(100);
  Shipping::Time t(1);
  Shipping::Segment::ExpeditedSupport expedited =
      Shipping::Segment::no_;

  std::vector<Fwk::Ptr<Shipping::Path> > paths =
      em.explore(bta, distance, cost, t, expedited);

  ASSERT_EQ(3, paths.size());
}

TEST_F(EngineTest, SimpleExploreGraphOutOfBoundaryTest) {
  Ptr<Shipping::BoatTerminal> bta = em.boatTerminal("BoatTerminalA");

  Ptr<Shipping::BoatTerminal> btd =
      Shipping::BoatTerminal::BoatTerminalNew("BoatTerminalD");
  em.boatTerminalIs(btd);

  Ptr<Shipping::BoatSegment> bsad =
      Shipping::BoatSegment::BoatSegmentNew("BoatSegmentAD");
  bsad->sourceIs(bta);
  bsad->lengthIs(200);
  em.boatSegmentIs(bsad);

  Ptr<Shipping::BoatSegment> bsda =
      Shipping::BoatSegment::BoatSegmentNew("BoatSegmentDA");
  bsda->sourceIs(btd);
  em.boatSegmentIs(bsda);

  bsad->returnSegmentIs(bsda);

  Shipping::Mile distance(100);
  Shipping::Dollar cost(100);
  Shipping::Time t(1);
  Shipping::Segment::ExpeditedSupport expedited =
      Shipping::Segment::no_;

  std::vector<Fwk::Ptr<Shipping::Path> > paths =
      em.explore(bta, distance, cost, t, expedited);

  ASSERT_EQ(2, paths.size());
}

TEST_F(EngineTest, SimpleExploreGraphNegativeTest) {
  Ptr<Shipping::BoatTerminal> bta = em.boatTerminal("BoatTerminalA");

  Shipping::Mile distance(101);
  Shipping::Dollar cost(101);
  Shipping::Time t(2);
  Shipping::Segment::ExpeditedSupport expedited =
      Shipping::Segment::yes_;

  std::vector<Fwk::Ptr<Shipping::Path> > paths =
      em.explore(bta, distance, cost, t, expedited);

  ASSERT_EQ(0, paths.size());
}

TEST_F(EngineTest, StatsTest) {
  
}