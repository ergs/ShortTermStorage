#include "shorttermstorage.h"

#include <limits>

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
    std::cout << prototype() << " " << id() << " " << storage_.quantity() << std::endl;
    manifest = storage_.PopN(storage_.count());
    double heat = 0;
    for (int i = 0; i < manifest.size(); ++i){
        manifest[i]->Decay(ctx->time());
        std::cout << i << ": " << manifest[i]->DecayHeat() << std::endl;
        heat += manifest[i]->DecayHeat();
    }
        
    cyclus::toolkit::RecordTimeSeries("DecayHeat", this, heat);
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
    CompMap cm;
    Material::Ptr target = Material::CreateUntracked(input_capacity, Composition::CreateFromAtom(cm));

    // Define Constraint Capacity
    double cc_left = input_capacity;
    if(storage_.quantity() + input_capacity > maximum_storage){
        cc_left = maximum_storage - storage_.quantity();
    }
    CapacityConstraint<Material> cc (cc_left);

    //Building ports
    port->AddRequest(target, this, in_commod, def_pref, false,
                     // INSERT SCOPATZ RAGE
                     std::bind(&ShortTermStorage::decay_heat, this, std::placeholders::_1));
    port->AddConstraint(cc);
    ports.insert(port);
    return ports;
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
    using cyclus::RequestPortfolio;

    cyclus::Context* ctx = context();
    std::set<BidPortfolio<Material>::Ptr> ports;

    // If theres nothing to give dont offer anything
    if(storage_.count() == 0) {return ports;}

    CapacityConstraint<Material> cc (storage_.quantity());
    BidPortfolio<Material>::Ptr port(new BidPortfolio<Material>());

    // Put everything in inventory to manifest
    std::vector<cyclus::Material::Ptr> manifest;
    manifest = storage_.PopN(storage_.count());
    
    // Offering Bids
    std::vector<Request<Material>*>& requests = commod_requests[out_commod];
    std::vector<Request<Material>*>::iterator it;
    for (it = requests.begin(); it != requests.end(); ++it) {
        Request<Material>* req = *it;
        //if(req->requester() == this){continue;}        
        Request<Material>::cost_function_t cf = req->cost_function();
        for (int i = 0; i < manifest.size(); ++i) {
            if (cf == NULL) {
                port->AddBid(req, manifest[i], this);
            } else {
                double cost = cf(manifest[i]);
                if (cost <= 1.0) {
                    port->AddBid(req, manifest[i], this, false, cost_to_pref(cost));
                }   
            }
        }
    }
    port->AddConstraint(cc);
    ports.insert(port);
    storage_.Push(manifest);
    return ports;
}

void ShortTermStorage::AdjustMatlPrefs(cyclus::PrefMap<cyclus::Material>::type& prefs) {
    using cyclus::BidPortfolio;
    using cyclus::CapacityConstraint;
    using cyclus::Converter;
    using cyclus::Material;
    using cyclus::Request;
    using cyclus::RequestPortfolio;
    using cyclus::Bid;

    cyclus::PrefMap<cyclus::Material>::type::iterator pmit;
    for (pmit = prefs.begin(); pmit != prefs.end(); ++pmit) {
        std::map<Bid<Material>*, double>::iterator mit;
        Request<Material>* req = pmit->first;
        if(req->requester() != this)
            continue;
        for (mit = pmit->second.begin(); mit != pmit->second.end(); ++mit) {
            Bid<Material>* bid = mit->first;
            //Bid<Material>* bid1 = Bid<Material>::Create(req, bid->offer(), this, false);
            //std::cout <<"YESTS: " << bid1->preference() << std::endl;
            std::cout << decay_heat_ulimit << " : " << bid->preference() << std::endl;
            if(bid->preference() < 0.0){
                std::cout << "FIRST "<< (mit->second) << " : ";
                mit->second = cost_to_pref(decay_heat(bid->offer()));
                std::cout << (mit->second) << std::endl;
            }
        }
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Accept material offered
void ShortTermStorage::AcceptMatlTrades(const std::vector< std::pair<cyclus::Trade<cyclus::Material>,
                                        cyclus::Material::Ptr> >& responses) {

    std::vector<std::pair<cyclus::Trade<cyclus::Material>, cyclus::Material::Ptr> >::const_iterator it;

    for (it = responses.begin(); it != responses.end(); ++it) {
        //std::cout << it->first.request->requester() << ": " << it->first.bid->bidder() << std::endl;
        storage_.Push(it->second);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Discharging material from the storage
void ShortTermStorage::GetMatlTrades(const std::vector< cyclus::Trade<cyclus::Material> >& trades,
        std::vector<std::pair<cyclus::Trade<cyclus::Material>,cyclus::Material::Ptr> >& responses) {
    using cyclus::Material;
    using cyclus::Trade;
    cyclus::Context* ctx = context();

    std::vector< cyclus::Trade<cyclus::Material> >::const_iterator it;
    for (it = trades.begin(); it != trades.end(); ++it) {  
        double qty = it->amt;
        Material::Ptr discharge = cyclus::ResCast<Material>(storage_.Pop(qty));
        responses.push_back(std::make_pair(*it, discharge));
    }
}

double ShortTermStorage::decay_heat(cyclus::Material::Ptr mat) {
    double cost = 1e100;
    double heat = mat->DecayHeat() / mat->quantity();
    if(heat < decay_heat_ulimit && heat > decay_heat_llimit ){
        cost = 1.0 - heat/decay_heat_ulimit;
    }
    //std::cout << decay_heat_ulimit << ": " << heat << ": "  <<cost <<  ": " << cost_to_pref(cost) << std::endl;
    return cost;
}

double ShortTermStorage::cost_to_pref(double c) {
    return (c > 1.0) ? 0.0 : 10. - (c * 9.);
}

void ShortTermStorage::sort_inventory() {
    using cyclus::Material;
    std::vector<Material::Ptr> manifest;
    manifest = storage_.PopN(storage_.count());
    
}

// WARNING! Do not change the following this function!!! This enables your
// archetype to be dynamically loaded and any alterations will cause your
// archetype to fail.
extern "C" cyclus::Agent* ConstructShortTermStorage(cyclus::Context* ctx) {
    return new ShortTermStorage(ctx);
}


}  // namespace shorttermstorage
