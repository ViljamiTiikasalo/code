// Datastructures.hh


#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <math.h>
#include <iostream>


// Types for IDs
using StationID = std::string;
using TrainID = std::string;
using RegionID = unsigned long long int;
using Name = std::string;
using Time = unsigned short int;

// Return values for cases where required thing was not found
StationID const NO_STATION = "---";
TrainID const NO_TRAIN = "---";
RegionID const NO_REGION = -1;
Name const NO_NAME = "!NO_NAME!";
Time const NO_TIME = 9999;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();


// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Map.size time complexity is constant.
    unsigned int station_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: .clear() is linear, no matter if its done 1 or 4 times.
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: The most heavy operation is for loop that goes through every element.
    std::vector<StationID> all_stations();

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: the heaviest operation is insert which is logarithmic when inserting one element.
    bool add_station(StationID id, Name const& name, Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: there is two logarithmic operations.
    Name get_station_name(StationID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: only two logarithmic operations.
    Coord get_station_coordinates(StationID id);

    // We recommend you implement the operations below only after implementing the ones above.

    // Estimate of performance: O(n)
    // Short rationale for estimate: there is couple for loops that are O(n) but they're not nested.
    std::vector<StationID> stations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: there is couple for loops that are O(n) but they're not nested.
    std::vector<StationID> stations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: go through every element in map.
    StationID find_station_with_coord(Coord xy);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: there is only logarithmic operations.
    bool change_station_coord(StationID id, Coord newcoord);

    // Estimate of performance: O(n)
    // Short rationale for estimate: in worst case every element is checked.
    bool add_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate:in worst case every element is checked in for loop.
    bool remove_departure(StationID stationid, TrainID trainid, Time time);

    // Estimate of performance: O(n)
    // Short rationale for estimate: again worst case is if every element in map have been in comparison.
    std::vector<std::pair<Time, TrainID>> station_departures_after(StationID stationid, Time time);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: heaviest operations are logarithmic at worst.
    bool add_region(RegionID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n)
    // Short rationale for estimate: every region is checked and listed.
    std::vector<RegionID> all_regions();

    // Estimate of performance: O(n)
    // Short rationale for estimate: average case would be O(1) but in worst case O(n).
    Name get_region_name(RegionID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop is heaviest and worst case is last element fits the criteria.
    std::vector<Coord> get_region_coords(RegionID id);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: every operation is log(n) or less.
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: O(log(n))
    // Short rationale for estimate: this is almost the same function as above.
    bool add_station_to_region(StationID id, RegionID parentid);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: for loop is heaviest operation which has the recursive function.
    std::vector<RegionID> station_in_regions(StationID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<RegionID> all_subregions_of_region(RegionID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StationID> stations_closest_to(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_station(StationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    RegionID common_parent_of_regions(RegionID id1, RegionID id2);

private:
    // Add stuff needed for your class implementation here

    static bool compare(std::pair<StationID,float> first, std::pair<StationID, float> second){
    return first.second < second.second;
    }

    float eucledian_distance(Coord c1){
            float distance = sqrt((c1.x*c1.x) + (c1.y*c1.y));
            return distance;
    }
    std::vector<RegionID> recursive_find(RegionID id, std::vector<RegionID> stat_in_reg);

    struct Region{
        RegionID regionid;
        Name name;
        std::vector<Coord> coords;
        RegionID parent;
        StationID station;

    };

    std::vector<Region> reg_vector;
    std::unordered_map<RegionID, int> order_of_regions;

    std::unordered_map<StationID, std::vector<std::pair<Time, TrainID>>> timeplaanari;

    std::map<StationID, std::pair<Name, Coord>> Stations_;
};

#endif // DATASTRUCTURES_HH
