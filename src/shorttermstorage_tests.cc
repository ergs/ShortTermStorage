#include <gtest/gtest.h>

#include "shorttermstorage_facility.h"

#include "agent_tests.h"
#include "context.h"
#include "facility_tests.h"

using shorttermstorage::ShortTermStorage;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ShortTermStorageTest : public ::testing::Test {
 protected:
  cyclus::TestContext tc;
  ShortTermStorage* facility;

  virtual void SetUp() {
    facility = new ShortTermStorage(tc.get());
  }

  virtual void TearDown() {
    delete facility;
  }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(ShortTermStorageTest, InitialState) {
  // Test things about the initial state of the facility here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(ShortTermStorageTest, Print) {
  EXPECT_NO_THROW(std::string s = facility->str());
  // Test ShortTermStorage specific aspects of the print method here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(ShortTermStorageTest, Tick) {
  ASSERT_NO_THROW(facility->Tick());
  // Test ShortTermStorage specific behaviors of the Tick function here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(ShortTermStorageTest, Tock) {
  EXPECT_NO_THROW(facility->Tock());
  // Test ShortTermStorage specific behaviors of the Tock function here
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Do Not Touch! Below section required for connection with Cyclus
cyclus::Agent* ShortTermStorageConstructor(cyclus::Context* ctx) {
  return new ShortTermStorage(ctx);
}
// Required to get functionality in cyclus agent unit tests library
#ifndef CYCLUS_AGENT_TESTS_CONNECTED
int ConnectAgentTests();
static int cyclus_agent_tests_connected = ConnectAgentTests();
#define CYCLUS_AGENT_TESTS_CONNECTED cyclus_agent_tests_connected
#endif  // CYCLUS_AGENT_TESTS_CONNECTED
INSTANTIATE_TEST_CASE_P(ShorttermstorageFac, FacilityTests,
                        ::testing::Values(&ShortTermStorageConstructor));
INSTANTIATE_TEST_CASE_P(ShorttermstorageFac, AgentTests,
                        ::testing::Values(&ShortTermStorageConstructor));
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
