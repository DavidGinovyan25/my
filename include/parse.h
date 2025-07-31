#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <limits>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cerrno>

std::string GetLog(std::ifstream&);
void parse(int, char *[]);
std::string GetFailedRequest(std::string&, std::ostream&);
void IsFileStatus(std::ifstream&);
void IsFileStatus(std::ofstream&);
uint32_t IsNumber(const char *, uint32_t);
uint32_t CheckEqual(const char *str, uint32_t upper_limit);
u_int32_t GetTimeInTimestamp(std::string&);
void CheckIndex(int, int, char *[]);
void GetTopFailedRequest(int);
u_int32_t FindMaxRequestWindow(int interval_time);

struct DataStorage {
    std::ifstream ifstream;
    std::ofstream ofstream; 
    std::string output_filename = "";
    std::string temp_name = "";
    std::string log = "";
    u_int32_t start_time = 0;
    u_int32_t end_time = __UINT32_MAX__;
    std::unordered_map<std::string, int> many_requests;
} inline keeper;

struct TimeIntervalsInSeconds {
    u_int32_t minute = 60;
    u_int32_t hour = 3600;
    u_int32_t day = 86400;
    u_int32_t year = 365 * day;
} inline time_constants;

struct WindowProccessing {
    std::ifstream ifstreamL;
    std::ifstream ifstreamR;
    std::string log = "";
    u_int32_t time = 0;
} inline window;
