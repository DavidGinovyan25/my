#include "parse.h"

void run(std::ostream& ostream) {
    std::string request = "";
    u_int32_t time = GetTimeInTimestamp(keeper.log);
    while (!keeper.ifstream.eof()) {    
        if (keeper.start_time <= time && time <= keeper.end_time) 
            request = GetFailedRequest(keeper.log, ostream);
        if (request != "")
            ostream << request << std::endl;
        keeper.log = GetLog(keeper.ifstream);
        time = GetTimeInTimestamp(keeper.log);
    }
}

void parse_ifstream(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i - 1], "-o") == 0) 
            continue;
        keeper.ifstream.open(argv[i]);
        window.ifstreamL.open(argv[i]);
        window.ifstreamR.open(argv[i]);
        if (keeper.ifstream.is_open())
            break;   
    }
}

void parse_ofstream(int argc, char *argv[]) { //проблема не в нахождении в одном цикле у -о и -р
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-o") == 0) {
            CheckIndex(argc, i, argv);
            keeper.output_filename = argv[i + 1];
            keeper.ofstream.open(argv[i + 1], std::ios::app);
            IsFileStatus(keeper.ofstream);
            continue;
        }
        if (strncmp(argv[i], "--output=", 9) == 0) {
            keeper.output_filename = CheckEqual(argv[i], UINT32_MAX); //std::cout << output_filename << std::endl;
            keeper.ofstream.open(keeper.output_filename, std::ios::app);
            IsFileStatus(keeper.ofstream);
            continue;
        }
    }
}

void parse_time(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-f") == 0) {
            CheckIndex(argc, i, argv);
            keeper.start_time = IsNumber(argv[i + 1], UINT32_MAX);
            continue;
        }
        if (strncmp(argv[i], "--front=", 8) == 0) {
            keeper.start_time = CheckEqual(argv[i], UINT32_MAX);
            continue;
        } 
        if (strcmp(argv[i], "-e") == 0) {
            CheckIndex(argc, i, argv);
            keeper.end_time = IsNumber(argv[i + 1], UINT32_MAX);
            continue;
        }
        if (strncmp(argv[i], "--to=", 5) == 0) {
            keeper.end_time = CheckEqual(argv[i], UINT32_MAX);
            continue;
        }
        if (keeper.start_time > keeper.end_time) {
            std::cerr << "ERROR: start time more than end time" << std::endl;
            exit(1);
        }
    }    
}

void parse_top(int argc, char *argv[]) {
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-s") == 0) {
            CheckIndex(argc, i, argv);
            GetTopFailedRequest(IsNumber(argv[i + 1], INT16_MAX));
            continue;
        }
        if (strncmp(argv[i], "--status=", 9) == 0) {
            GetTopFailedRequest(CheckEqual(argv[i], UINT32_MAX));
            continue;
        }
    }
}
void parse_stdout(int argc, char *argv[]) {
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--print") == 0 || strcmp(argv[i], "-p") == 0) {
            run(std::cout);
            continue;
        }    
    }
}

void parse_window(int argc, char *argv[]) {
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-w") == 0) {
            int x = FindMaxRequestWindow(IsNumber(argv[i + 1], INT16_MAX));
            std::cout << x << std::endl;
            continue;
        }

        if (strncmp(argv[i], "--window=", 9) == 0) {
            int x = FindMaxRequestWindow(CheckEqual(argv[i], INT16_MAX));
            std::cout << x << std::endl;
            continue;
        }
    }
}

void parse(int argc, char *argv[]) {
    parse_ifstream(argc, argv);
    IsFileStatus(keeper.ifstream);
    parse_ofstream(argc, argv);
    keeper.log = GetLog(keeper.ifstream);
    parse_time(argc, argv);
    parse_stdout(argc, argv);
    parse_window(argc, argv);
    run(keeper.ofstream);
    parse_top(argc, argv);
    keeper.ifstream.close();
    keeper.ofstream.close();
}
