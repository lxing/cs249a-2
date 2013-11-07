#include "gtest/gtest.h"
#include "../Engine.h"

/******************/
/* Initialization */
/******************/

class EngineTest : public ::testing::Test {
protected:
  virtual void SetUp() {

  }

  Shipping::EngineManager em;
};

/*******************/
/* Test Engine */
/*******************/

TEST_F(EngineTest, EmptyStats) {
  Ptr<Shipping::Stats> stats = em.stats();
}