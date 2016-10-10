#include "shorttermstorage_facility.h"

namespace shorttermstorage {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ShorttermstorageFacility::ShorttermstorageFacility(cyclus::Context* ctx) : cyclus::Facility(ctx) {}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ShorttermstorageFacility::str() {
    return Facility::str();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ShorttermstorageFacility::Tick() {
    cyclus::Context* ctx = context();
    std::vector<cyclus::Material::Ptr> manifest;
    manifest = cyclus::ResCast<Material>(storage_.PopN(storage_.count())); 
    for (int i = 0; i < manifest.size(); ++i){
        manifest[i].Decay(ctx->time());
    }
    storage.PushAll(manifest);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ShorttermstorageFacility::Tock() {}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// The storage facility requests the amount of material it can take.
std::set<cyclus::RequestPortfolio<cyclus::Material>::Ptr> ShorttermstorageFacility::GetMatlRequests() {
    using cyclus::RequestPortfolio;
    using cyclus::Material;
    using cyclus::CapacityConstraint;
    using cyclus::Composition;
    using cyclus::CompMap;

    std::set<RequestPortfolio<Material>::Ptr> ports;
    if(storage_.quantity() => maximum_storage){return ports;}
    

    //Define Constraint Capacity
    if(storage_.qty + input_capacity > maximum_storage){
        CapacityConstraint<Material> cc(maximum_storage - storage_.quantity()); 
    } else {
        CapacityConstraint<Material> cc(input_capacity); 
    }
    
    //Building ports
    port->AddRequest(target, this, in_commod);
    port->AddConstraint(cc);
    ports.insert(port);
    return ports;
}
a
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Accept material offered
void ShorttermstorageFaciilty::AcceptMatlTrades(const std::vector< std::pair<cyclus::Trade<cyclus::Material>,
                                        cyclus::Material::Ptr> >& responses) {

    std::vector<std::pair<cyclus::Trade<cyclus::Material>, cyclus::Material::Ptr> >::const_iterator it;

    for (it = responses.begin(); it != responses.end(); ++it) {
        if(it->first.request->commodity() == in_commod){
            storage_.Push(it->second);
        }
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Offer materials
std::set<cyclus::BidPortfolio<cyclus::Material>::Ptr> ShorttermstorageFacility::GetMatlBids(
                            cyclus::CommodMap<cyclus::Material>::type& commod_requests) {

    using cyclus::BidPortfolio;
    using cyclus::CapacityConstraint;
    using cyclus::Converter;
    using cyclus::Material;
    using cyclus::Request;

    cyclus::Context* ctx = context();
    std::set<BidPortfolio<Material>::Ptr> ports;

    // If theres nothing to give dont offer anything
    if(storage_.count() == 0) {return ports;}

    // Put everything in inventory to manifest
    std::vector<cyclus::Material::Ptr> manifest;
    manifest = cyclus::ResCast<Material>(storage_.PopN(storage_.count()));

    // Offering Bids
    std::vector<Request<Material>*>& requests = commod_requests[out_commod];
    std::vector<Request<Material>*>::iterator it;
    for (it = requests.begin(); it != requests.end(); ++it) {
        Request<Material>* req = *it;
        
    }

    storage_.PushAll(manifest);
    ports.insert(port);

    return ports;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Discharging material from the storage
void ShorttermstorageFacaility::GetMatlTrades(const std::vector< cyclus::Trade<cyclus::Material> >& trades,
        std::vector<std::pair<cyclus::Trade<cyclus::Material>,cyclus::Material::Ptr> >& responses) {
    using cyclus::Material;
    using cyclus::Trade;
    cyclus::Context* ctx = context();

    std::vector< cyclus::Trade<cyclus::Material> >::const_iterator it;
    for (it = trades.begin(); it != trades.end(); ++it) {

    }
}

double decay_heat(cyclus::Material::Ptr mat) {
    double pref = 1;
    double heat = mat.DecayHeat();
    if(heat > dec_heat_ulimit || heat < dec_heat_llimit ){
        pref = 0;
    return pref;    
}

// WARNING! Do not change the following this function!!! This enables your
// archetype to be dynamically loaded and any alterations will cause your
// archetype to fail.
extern "C" cyclus::Agent* ConstructShorttermstorageFacility(cyclus::Context* ctx) {
    return new ShorttermstorageFacility(ctx);
}


}  // namespace shorttermstorage
