#include <iostream>
#include "gtest/gtest.h"
#include "../Instance.h"

using namespace std;

/******************/
/* Initialization */
/******************/

class InstanceTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    manager = shippingInstanceManager();
    null = NULL;

    fleet = manager->instanceNew("fleet", "Fleet");
    fleet->attributeIs("Truck, speed", "50.5");
    fleet->attributeIs("Truck, capacity", "1");
    fleet->attributeIs("Truck, cost", "2.5");
    fleet->attributeIs("Boat, speed", "20.1");
    fleet->attributeIs("Boat, capacity", "40");
    fleet->attributeIs("Boat, cost", "3");
    fleet->attributeIs("Plane, speed", "300");
    fleet->attributeIs("Plane, capacity", "2");
    fleet->attributeIs("Plane, cost", "5.666");

    customer = manager->instanceNew("cust", "Customer");
    port = manager->instanceNew("port", "Port");
    truckT = manager->instanceNew("truckT", "Truck terminal");
    boatT = manager->instanceNew("boatT", "Boat terminal");
    planeT = manager->instanceNew("planeT", "Plane terminal");

    //truckS = manager->instanceNew("truckS", "Truck segment");
    //boatS = manager->instanceNew("boatS", "Boat segment");
    //planeS = manager->instanceNew("planeS", "Plane segment");
  }

  Ptr<Instance::Manager> manager;
  Ptr<Instance> null;

  Ptr<Instance> fleet;

  Ptr<Instance> customer;
  Ptr<Instance> port;
  Ptr<Instance> truckT;
  Ptr<Instance> boatT;
  Ptr<Instance> planeT;

  Ptr<Instance> truckS;
  Ptr<Instance> boatS;
  Ptr<Instance> planeS;
};

/*****************/
/* Test Instance */
/*****************/

TEST_F(InstanceTest, FleetTest) {
  ASSERT_NE(fleet, null);
  ASSERT_NE(manager->instance("fleet"), null);

  ASSERT_EQ(fleet->attribute("Truck, speed"), "50.50");
  ASSERT_EQ(fleet->attribute("Truck, capacity"), "1");
  ASSERT_EQ(fleet->attribute("Truck, cost"), "2.50");
  ASSERT_EQ(fleet->attribute("Boat, speed"), "20.10");
  ASSERT_EQ(fleet->attribute("Boat, capacity"), "40");
  ASSERT_EQ(fleet->attribute("Boat, cost"), "3.00");
  ASSERT_EQ(fleet->attribute("Plane, speed"), "300.00");
  ASSERT_EQ(fleet->attribute("Plane, capacity"), "2");
  ASSERT_EQ(fleet->attribute("Plane, cost"), "5.67"); // Test rounding
}

TEST_F(InstanceTest, LocationTest) {
  ASSERT_NE(customer, null);
  ASSERT_NE(port, null);
  ASSERT_NE(truckT, null);
  ASSERT_NE(boatT, null);
  ASSERT_NE(planeT, null);
  ASSERT_NE(manager->instance("cust"), null);
  ASSERT_NE(manager->instance("port"), null);
  ASSERT_NE(manager->instance("truckT"), null);
  ASSERT_NE(manager->instance("boatT"), null);
  ASSERT_NE(manager->instance("planeT"), null);
}

TEST_F(InstanceTest, SegmentTest) {
  //ASSERT_NE(truckS, null);
  //ASSERT_NE(boatS, null);
  //ASSERT_NE(planeS, null);
  //ASSERT_NE(manager->instance("truckS"), null);
  //ASSERT_NE(manager->instance("boatS"), null);
  //ASSERT_NE(manager->instance("planeS"), null);
}
