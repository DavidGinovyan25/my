#include "parse.h"

void IsFileStatus(std::ifstream& file) {
    if (!file.is_open()) {
        std::cerr << "ERROR: input file wasnt opened" << std::endl;
        exit(1);
    }
    std::cout << "Input file successfully opened" << std::endl;
}

void IsFileStatus(std::ofstream& file) {
    if (!file.is_open()) {
        std::cerr << "ERROR: output file wasnt opened" << std::endl;
        exit(1);
    }
    std::cout << "Output file successfully opened" << std::endl;
}

void CheckIndex(int argc, int index, char *argv[]) {
    if (index == argc - 1) {
        std::cout << "After argument " << argv[index] << " absent need parametr" << std::endl; 
        exit(1);
    }
}

uint32_t CheckEqual(const char *str, uint32_t upper_limit) {
    const char *equal = strchr(str, '=');
    if (!equal) {
        std::cout << "ERROR: doesnt have symbol equal" << std::endl;
        exit(1);
    }
    return IsNumber(equal + 1, upper_limit);
}

uint32_t IsNumber(const char *str, uint32_t upper_limit) {
    if (*str == '\0') {
        std::cout << "ERROR: string without symbol" << std::endl;
        exit(1);
    }
    char* end_ptr = nullptr;
    errno = 0;
    long num = strtol(str, &end_ptr, 10);
    if (num < 0 || num > upper_limit) {
        std::cout << "ERROR: " << str << " number isnt in interval" << std::endl;
        exit(1);
    }
    if (end_ptr == str || *end_ptr != '\0' || errno == ERANGE) {
        std::cout << "ERROR: " << str << " isnt number" << std::endl;
        int size = keeper.many_requests.size();
        int n = (10 > size) ? size : 10;
        return n;
    }
    return static_cast<uint32_t>(num);
}