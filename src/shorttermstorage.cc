#include "shorttermstorage.h"

namespace shorttermstorage {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ShortTermStorage::ShortTermStorage(cyclus::Context* ctx) : cyclus::Facility(ctx) {}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ShortTermStorage::str() {
    return Facility::str();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ShortTermStorage::Tick() {
    using cyclus::Material;
    cyclus::Context* ctx = context();
    std::vector<Material::Ptr> manifest;
    manifest = storage_.PopN(storage_.count());
    for (int i = 0; i < manifest.size(); ++i){
        manifest[i]->Decay(ctx->time());
    }
    storage_.Push(manifest);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ShortTermStorage::Tock() {}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// The storage facility requests the amount of material it can take.
std::set<cyclus::RequestPortfolio<cyclus::Material>::Ptr> ShortTermStorage::GetMatlRequests() {
    using cyclus::RequestPortfolio;
    using cyclus::Material;
    using cyclus::CapacityConstraint;
    using cyclus::Composition;
    using cyclus::CompMap;

    std::set<RequestPortfolio<Material>::Ptr> ports;
    if(storage_.quantity() >= maximum_storage){return ports;}
    RequestPortfolio<Material>::Ptr port(new RequestPortfolio<Material>());
    CapacityConstraint<Material> cc;
    CompMap cm;
    Material::Ptr target = Material::CreateUntracked(1, Composition::CreateFromAtom(cm));

    //Define Constraint Capacity
    if(storage_.quantity() + input_capacity > maximum_storage){
        cc = maximum_storage - storage_.quantity();
    } else {
        cc = input_capacity;
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
std::set<cyclus::BidPortfolio<cyclus::Material>::Ptr> ShortTermStorage::GetMatlBids(
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
    manifest = storage_.PopN(storage_.count());

    // Offering Bids
    std::vector<Request<Material>*>& requests = commod_requests[out_commod];
    std::vector<Request<Material>*>::iterator it;
    for (it = requests.begin(); it != requests.end(); ++it) {
        Request<Material>* req = *it;

    }
    RequestPortfolio<Material>::Ptr port(new RequestPortfolio<Material>());
    storage_.Push(manifest);
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
extern "C" cyclus::Agent* ConstructShortTermStorage(cyclus::Context* ctx) {
    return new ShortTermStorage(ctx);
}


}  // namespace shorttermstorage
