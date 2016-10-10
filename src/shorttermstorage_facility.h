#ifndef CYCLUS_SHORTTERMSTORAGES_SHORTTERMSTORAGE_FACILITY_H_
#define CYCLUS_SHORTTERMSTORAGES_SHORTTERMSTORAGE_FACILITY_H_

#include <string>

#include "cyclus.h"

namespace shorttermstorage {

/// @class ShorttermstorageFacility
///
/// This Facility is intended
/// as a skeleton to guide the implementation of new Facility
/// agents.
/// The ShorttermstorageFacility class inherits from the Facility class and is
/// dynamically loaded by the Agent class when requested.
///
/// @section intro Introduction
/// Place an introduction to the agent here.
///
/// @section agentparams Agent Parameters
/// Place a description of the required input parameters which define the
/// agent implementation.
///
/// @section optionalparams Optional Parameters
/// Place a description of the optional input parameters to define the
/// agent implementation.
///
/// @section detailed Detailed Behavior
/// Place a description of the detailed behavior of the agent. Consider
/// describing the behavior at the tick and tock as well as the behavior
/// upon sending and receiving materials and messages.
class ShorttermstorageFacility : public cyclus::Facility  {
 public:
  /// Constructor for ShorttermstorageFacility Class
  /// @param ctx the cyclus context for access to simulation-wide parameters
  explicit ShorttermstorageFacility(cyclus::Context* ctx);

  /// The Prime Directive
  /// Generates code that handles all input file reading and restart operations
  /// (e.g., reading from the database, instantiating a new object, etc.).
  /// @warning The Prime Directive must have a space before it! (A fix will be
  /// in 2.0 ^TM)

  #pragma cyclus

  /// A verbose printer for the ShorttermstorageFacility
  virtual std::string str();

  /// The handleTick function specific to the ShorttermstorageFacility.
  /// @param time the time of the tick
  virtual void Tick();

  /// The handleTick function specific to the ShorttermstorageFacility.
  /// @param time the time of the tock
  virtual void Tock();

  std::set<cyclus::RequestPortfolio<cyclus::Material>::Ptr> GetMatlRequests();

  void AcceptMatlTrades(const std::vector< std::pair<cyclus::Trade<cyclus::Material>,
                                      cyclus::Material::Ptr> >& responses);

  virtual std::set<cyclus::BidPortfolio<cyclus::Material>::Ptr> GetMatlBids(
                          cyclus::CommodMap<cyclus::Material>::type& commod_requests);

  void GetMatlTrades( const std::vector< cyclus::Trade<cyclus::Material> >& trades,
                      std::vector<std::pair<cyclus::Trade<cyclus::Material>,
                      cyclus::Material::Ptr> >& responses);

  #pragma cyclus var {"tooltip": "Input commodities that the facility consumes.", \
                      "doc": "Commodities that the storage facility consumes", \
                      "uitype": "incommodity"}
  std::string in_commod;

  #pragma cyclus var {"tooltip": "output commodity", \
                      "doc": "commodity that storage facility supplies", \
                      "uitype": "outcommodity", \
                      "uilabel": "Discharge Commodity"}
  std::string out_commod;

  #pragma cyclus var {"tooltip": "The maximum decay heat limit that the "\
                      "storage facility can accomidate.", \
                      "doc" : "The facility is designed to support a used " \
                              "fuel with a decay heat per volume of up to this " \
                              "value", \
                      "units": "W/m^3", \
                      "uilabel": "Decay Heat Upper Limit"}
  double dec_heat_ulimit;

  #pragma cyclus var {"tooltip": "The maximum decay heat limit that the "\
                      "storage facility can accomidate.", \
                      "doc" : "The facility is designed to support a used " \
                              "fuel with a decay heat per volume of up to this " \
                              "value", \
                      "units": "W/m^3", \
                      "uilabel": "Decay Heat Lower Limit", \
                      "default": 0}
  double dec_heat_llimit;

  #pragma cyclus var {"tooltip": "Capacity per timestep", \
                      "doc" : "The amount of material that the facility " \
                              "can recieve per timestep.", \
                      "units": "kg", \
                      "uilabel": "Recieving Capacity"}
  double input_capacity;

  #pragma cyclus var {"tooltip": "Capacity per timestep", \
                      "doc" : "The amount of material that the facility " \
                              "can eject per timestep.", \
                      "units": "kg", \
                      "uilabel": "Removal Capacity"}
  double output_capacity;

  #pragma cyclus var {"tooltip": "Maximum size", \
                      "doc" : "The maximum amount of material " \
                      "that the facility can hold.", \
                      "units": "kg", \
                      "uilabel": "Maximum Storage Size"}
  double maximum_storage;

 private:
  cyclus::toolkit::ResourceBuff storage_;


  // And away we go!
};

}  // namespace shorttermstorage

#endif  // CYCLUS_SHORTTERMSTORAGES_SHORTTERMSTORAGE_FACILITY_H_
