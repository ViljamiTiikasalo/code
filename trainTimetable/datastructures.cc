// Datastructures.cc


#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    Stations_.clear();
    timeplaanari.clear();
    order_of_regions.clear();
    reg_vector.clear();
}

unsigned int Datastructures::station_count()
{

    unsigned int stat_count = Stations_.size();
    return stat_count;
}

void Datastructures::clear_all()
{
    Stations_.clear();
    timeplaanari.clear();
    order_of_regions.clear();
    reg_vector.clear();
}

std::vector<StationID> Datastructures::all_stations()
{
    std::vector<StationID> stationID;
    for(std::map<StationID, std::pair<Name, Coord>>::iterator it = Stations_.begin(); it != Stations_.end(); ++it){
        stationID.push_back(it->first);
    }
    return stationID;
}

bool Datastructures::add_station(StationID id, const Name& name, Coord xy)
{
    int before_ins_size = Stations_.size();
    Stations_.insert({id,std::make_pair(name,xy)});
    int after_ins_size = Stations_.size();
    if (before_ins_size != after_ins_size){
        return true;
    }
    return false;
}

Name Datastructures::get_station_name(StationID id)
{ 
    Name station_name;
    if(Stations_.count(id) == 0){
        return NO_NAME;
    }
    else{
        station_name = Stations_[id].first;
        return station_name;
    }
}

Coord Datastructures::get_station_coordinates(StationID id)
{
    Coord station_coord;
    if(Stations_.count(id)==0){
        return NO_COORD;
    }
    else{
        station_coord = Stations_[id].second;
        return station_coord;
    }
}

std::vector<StationID> Datastructures::stations_alphabetically()
{
    std::vector<StationID> ordered_id;
    if(Stations_.size() > 0){
        std::vector<std::pair<Name, StationID>> alphabet_order;
        for(std::pair<StationID,std::pair<Name,Coord>> element : Stations_){
            alphabet_order.push_back(std::make_pair(element.second.first, element.first));
        }
        std::sort(alphabet_order.begin(),alphabet_order.end());


        for(auto& it : alphabet_order){
            ordered_id.push_back(it.second);
        }
    }
    return ordered_id;

}

std::vector<StationID> Datastructures::stations_distance_increasing()
{
    std::vector<StationID> station_distance;
    if(Stations_.size() > 0){
        std::vector<std::pair<StationID, float>> pairs;

        for (const auto& element : Stations_){
            float distance = eucledian_distance(element.second.second);
            pairs.push_back(std::make_pair(element.first, distance));
        }
        std::sort(pairs.begin(), pairs.end(), compare);

        int size = pairs.size();
        station_distance.push_back(pairs[0].first);
        for (int i = 1; i < size; i++){

            station_distance.push_back(pairs[i].first);

            if (pairs[i].second == pairs[i-1].second){
                Coord coord1 = get_station_coordinates(pairs[i-1].first);
                Coord coord2 = get_station_coordinates(pairs[i].first);

                if(coord1.y > coord2.y){
                    std::iter_swap(station_distance.begin()+i-1,station_distance.begin()+i);
                }
            }
        }
    }
    return station_distance;
}




StationID Datastructures::find_station_with_coord(Coord xy)
{
    for(auto& it: Stations_){
        if(it.second.second == xy)
            return it.first;
    }
    return NO_STATION;
}

bool Datastructures::change_station_coord(StationID id, Coord newcoord)
{
    if (Stations_.find(id) != Stations_.end()){
        Stations_[id].second = newcoord;
        return true;
    }
    else{
        return false;
    }
}

bool Datastructures::add_departure(StationID stationid, TrainID trainid, Time time)
{

    std::unordered_map<StationID, std::vector<std::pair<Time, TrainID>>>::const_iterator got = timeplaanari.find(stationid);

    if(Stations_.find(stationid) != Stations_.end()){
        if(got != timeplaanari.end()){
            for (auto& it : got->second){
                if(it.first == time && it.second == trainid){
                    return false;
                }
            }
            auto pair = std::make_pair(time, trainid);
            timeplaanari[stationid].push_back(pair);
            return true;

        }else{
            std::vector<std::pair<Time,TrainID>> time_and_train;
            time_and_train.push_back(std::make_pair(time,trainid));
            timeplaanari.insert({stationid, time_and_train});
            return true;
        }
    }
    return false;
}

bool Datastructures::remove_departure(StationID stationid, TrainID trainid, Time time)
{
    std::unordered_map<StationID, std::vector<std::pair<Time, TrainID>>>::const_iterator got = timeplaanari.find(stationid);
    int index = 0;

    if(Stations_.find(stationid) != Stations_.end()){
        if(got != timeplaanari.end()){
            for (auto& it : got->second){
                std::cout<< index << std::endl;
                if(it.first == time && it.second == trainid){
                    if(got->second.size() == 1){
                        timeplaanari.erase(got);
                        return true;
                    }else{
                        timeplaanari[stationid].erase(timeplaanari[stationid].begin()+index);
                        return true;
                    }
                }
                index++;
            }
        }
    }
    return false;
}

std::vector<std::pair<Time, TrainID>> Datastructures::station_departures_after(StationID stationid, Time time)
{

    std::unordered_map<StationID, std::vector<std::pair<Time, TrainID>>>::const_iterator got = timeplaanari.find(stationid);
    std::vector<std::pair<Time, TrainID>> train_times;

    if(Stations_.find(stationid) != Stations_.end()){
        if(got != timeplaanari.end()){
            for (auto& it : got->second){
                if(it.first >= time){
                    train_times.push_back(std::make_pair(it.first, it.second));
                }
            }
        }
        std::sort(train_times.begin(), train_times.end());
        return train_times;
    }else{
        train_times.push_back(std::make_pair(NO_TIME, NO_TRAIN));
        return train_times;
    }
}

bool Datastructures::add_region(RegionID id, const Name &name, std::vector<Coord> coords)
{
    //std::unordered_map<RegionID, int>::const_iterator got = order_of_regions.find(id);

    if(order_of_regions.find(id) != order_of_regions.end()){
        return false;
    }
    else{
        int order_nr = order_of_regions.size();


        reg_vector.push_back({id, name, coords, 0,""});
        order_of_regions.insert({id, order_nr});
        return true;
    }
}

std::vector<RegionID> Datastructures::all_regions()
{
    std::vector<RegionID> all_reg;
    for(auto& structi : reg_vector){
        RegionID regid = structi.regionid;
        all_reg.push_back(regid);
    }
    return all_reg;
}

Name Datastructures::get_region_name(RegionID id)
{
    if(order_of_regions.find(id) == order_of_regions.end()){
        return NO_NAME;
    }
    else{
        int num = order_of_regions.at(id);
        Name name = reg_vector[num].name;
        return name;
    }
}

std::vector<Coord> Datastructures::get_region_coords(RegionID id)
{
    std::vector<Coord> coords_region;
    if(order_of_regions.count(id) != 0){
        int num = order_of_regions.at(id);
        for(auto coord : reg_vector[num].coords){
            coords_region.push_back(coord);
        }
        return coords_region;
    }
    else{
        coords_region.push_back(NO_COORD);
        return coords_region;
    }
}

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    if(order_of_regions.count(id) != 0){
        if(order_of_regions.count(parentid) != 0){
            int num = order_of_regions.at(id);
            if(reg_vector[num].parent == 0){
                reg_vector[num].parent = parentid;
                return true;
            }
        }
    }
    return false;
}

bool Datastructures::add_station_to_region(StationID id, RegionID parentid)
{
    if(Stations_.count(id) != 0){
        if(order_of_regions.count(parentid) != 0){
            int num = order_of_regions.at(parentid);
            if(reg_vector[num].station == ""){
                reg_vector[num].station = id;
                return true;
            }
        }
    }
    return false;
}

std::vector<RegionID> Datastructures::station_in_regions(StationID id)
{
    std::vector<RegionID> stat_in_reg;
    if(Stations_.count(id) != 0){
        for(auto& region : reg_vector){
            if(region.station == id){
                RegionID reg = region.regionid;
                stat_in_reg = recursive_find(reg,stat_in_reg);
            }
        }

    }
    else{
        stat_in_reg.push_back(NO_REGION);
    }
    return stat_in_reg;
}

std::vector<RegionID> Datastructures::recursive_find(RegionID id, std::vector<RegionID> stat_in_reg)
{
    stat_in_reg.push_back(id);
    for(auto& region : reg_vector){
        if(region.regionid == id){
            if(region.parent != 0){
                RegionID reg = region.parent;
                stat_in_reg = recursive_find(reg,stat_in_reg);
            }

        }

    }
    return stat_in_reg;
}


std::vector<RegionID> Datastructures::all_subregions_of_region(RegionID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("all_subregions_of_region()");
}

std::vector<StationID> Datastructures::stations_closest_to(Coord /*xy*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("stations_closest_to()");
}

bool Datastructures::remove_station(StationID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_station()");
}

RegionID Datastructures::common_parent_of_regions(RegionID /*id1*/, RegionID /*id2*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("common_parent_of_regions()");
}
