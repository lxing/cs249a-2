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
    nullInstance = NULL;

    fleet = manager->instanceNew("fleet", "Fleet");
    fleet->attributeIs("Boat, speed", "20.1");
  }

  Ptr<Instance::Manager> manager;
  Ptr<Instance> fleet;
  Ptr<Instance> nullInstance;
};

/*****************/
/* Test Instance */
/*****************/

TEST_F(InstanceTest, FleetTest) {
  ASSERT_NE(fleet, nullInstance);
  //ASSERT_EQ(fleet->attribute("Boat, speed"), "20.10");
  unsigned int xx = 20;
  stringstream ss;
  ss << fixed << setprecision(2);
  ss << xx;
  cout << ss.str();
}
