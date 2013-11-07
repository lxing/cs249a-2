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