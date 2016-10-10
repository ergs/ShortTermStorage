#include <gtest/gtest.h>

#include "shorttermstorage_facility.h"

#include "agent_tests.h"
#include "context.h"
#include "facility_tests.h"

using shorttermstorage::ShorttermstorageFacility;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ShorttermstorageFacilityTest : public ::testing::Test {
 protected:
  cyclus::TestContext tc;
  ShorttermstorageFacility* facility;

  virtual void SetUp() {
    facility = new ShorttermstorageFacility(tc.get());
  }

  virtual void TearDown() {
    delete facility;
  }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(ShorttermstorageFacilityTest, InitialState) {
  // Test things about the initial state of the facility here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(ShorttermstorageFacilityTest, Print) {
  EXPECT_NO_THROW(std::string s = facility->str());
  // Test ShorttermstorageFacility specific aspects of the print method here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(ShorttermstorageFacilityTest, Tick) {
  ASSERT_NO_THROW(facility->Tick());
  // Test ShorttermstorageFacility specific behaviors of the Tick function here
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TEST_F(ShorttermstorageFacilityTest, Tock) {
  EXPECT_NO_THROW(facility->Tock());
  // Test ShorttermstorageFacility specific behaviors of the Tock function here
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Do Not Touch! Below section required for connection with Cyclus
cyclus::Agent* ShorttermstorageFacilityConstructor(cyclus::Context* ctx) {
  return new ShorttermstorageFacility(ctx);
}
// Required to get functionality in cyclus agent unit tests library
#ifndef CYCLUS_AGENT_TESTS_CONNECTED
int ConnectAgentTests();
static int cyclus_agent_tests_connected = ConnectAgentTests();
#define CYCLUS_AGENT_TESTS_CONNECTED cyclus_agent_tests_connected
#endif  // CYCLUS_AGENT_TESTS_CONNECTED
INSTANTIATE_TEST_CASE_P(ShorttermstorageFac, FacilityTests,
                        ::testing::Values(&ShorttermstorageFacilityConstructor));
INSTANTIATE_TEST_CASE_P(ShorttermstorageFac, AgentTests,
                        ::testing::Values(&ShorttermstorageFacilityConstructor));
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
