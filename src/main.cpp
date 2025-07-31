#include "parse.h"

std::string GetLog(std::ifstream& file) {    //получаю лог
    std::string log;
    std::getline(file, log);
    return log;
}

void AddRequestInHashMap(std::string log) {  //заполняю хеш-мапу запросом
    ++keeper.many_requests[log];
}

std::string PartOfLog(int& index, std::string log, char c) {  //парсинг для времени
    std::string temp = "";
    do{
        temp += log[++index];
    } while (log[index] != c);
    return temp;
}

int CountLeapYear(int year) {   //определение количества високосных лет
    int count = 0;
    for (int i = 1970; i < year + 1; ++i) {
        if (i % 4 == 0 || (i % 100 == 0 && i % 400 == 0))
            count++;
    }
    return count;
}

u_int32_t GetTimeInTimestamp(std::string& log) {   //вернет время в timestamp
    int index = 0;
    std::string temp = "";
    u_int32_t x = time_constants.day * 181; // from JAN to JUL (border not included). without note leap year 
    for (int i = 0; i < log.size(); ++i) {
        if (log[i] != '[')
            continue;
        index = i;
        break;
    }
    x += (strtol(PartOfLog(index, log, '/').c_str(), NULL, 10) - 1) * time_constants.day; // ПОТОМУ ЧТО ДЕНЬ НЕ ЗАКОНЧИЛСЯ
    temp = PartOfLog(index, log, '/'); //
    int year = strtol(PartOfLog(index, log, ':').c_str(), NULL, 10); //
    x += (year - 1970) * time_constants.year;
    x += CountLeapYear(year) * time_constants.day;
    x += strtol(PartOfLog(index, log, ':').c_str(), NULL, 10) * time_constants.hour;  //
    x += strtol(PartOfLog(index, log, ':').c_str(), NULL, 10) * time_constants.minute;    //
    x += strtol(PartOfLog(index, log, ' ').c_str(), NULL, 10); // 
    return x;
}

std::string GetFailedRequest(std::string& log, std::ostream& ostream) { //вернет запрос с ошибкой 5хх
    int count = 0;  
    int index1 = 0;
    int index2 = 0;
    std::string request = "";
    for (size_t i = 0; i < log.size(); ++i) {
        if (log[i] != '\"')
            continue;
        count += 1;
        (count == 1) ? index1 = i : index2 = i;
        if (count == 2)
            break; 
    }
    if (log[index2 + 2] == '5') {   
        for (int x = index1 + 1; x < index2; ++x) {
            request += log[x];
        }
        AddRequestInHashMap(request);
    }
    return request;
}

void GetTopFailedRequest(int n) {   //топ N повторяющихся запросов с ошибкой
    std::vector<std::pair<std::string, int>> v(
        keeper.many_requests.begin(), 
        keeper.many_requests.end()
    );
    std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second)
            return a.second > b.second;
        return a.first < b.first;
    });
    n = (n < v.size()) ? n : v.size(); 
    std::cout << std::endl <<"\033[1mTOP " << n << " FAILED REQUEST\033[0m" << std::endl;
    for (int i = 0; i < n && i < v.size(); ++i) {
        std::cout << "\033[1;33mRequest:\033[0m "<<  v[i].first << " ";
        std::cout << "\033[1;35mCount:\033[0m " << v[i].second << std::endl;
    }
    std::cout << std::endl;
}

/*открыть файл в двух потоках
в пермо потоке будет указатель L, во втором потоке - R
пройтись методом двух указателей - если разница при увеличении R меньше, 
то увеличиваем R, если больше - сравниваем интервал с максимальным интервалом
и увеличиваем L
оптимизация - как-нибудь учесть повторяющееся время*/
u_int32_t FindMaxRequestWindow(int interval_time) { //НЕВЕРНО СЧИТАЕТ ДИАПАЗОН
    std::string left_log = GetLog(window.ifstreamL);
    std::string right_log = GetLog(window.ifstreamR);
    uint32_t less_time = GetTimeInTimestamp(left_log);
    uint32_t more_time = GetTimeInTimestamp(right_log);
    uint32_t ltime = less_time;
    uint32_t rtime = more_time;
    uint32_t max_count_request = 0;
    uint32_t current_count_request = 0;

    while(!window.ifstreamR.eof()) { //пока не достигнут конец файла
        if(more_time - less_time <= interval_time) {    //если разница во времени меньше интервала
            right_log = GetLog(window.ifstreamR);   //  увеличиваю правый указатель
            more_time = GetTimeInTimestamp(right_log);  //увеличиваю правый указатель
            ++current_count_request;    //увеличиваю счетчик
            //std::cout << less_time << " " << more_time << std::endl;
        } else {    //если нет - обновляю максимум
            max_count_request = std::max(max_count_request, current_count_request);
            int count = 0;
            while (more_time - less_time > interval_time) { //пока изначальное условие не выполнено
                count++;
                left_log = GetLog(window.ifstreamL);    //увеличиваю левый указатель
                less_time = GetTimeInTimestamp(left_log);   //увеличиваю левый указатель
            }   //
            current_count_request -=count;  //
        }
    }
    max_count_request = std::max(max_count_request, current_count_request);
    return max_count_request;
}

int main(int argc, char *argv[]) {
    parse(argc, argv);
    return 0;
}




























//сделать проверки на входные аргументы командной строки

// u_int32_t FindMaxRequestWindow(int interval_time) {
//     u_int32_t count_window_request = 0;
//     u_int32_t max_request = 0;
//     auto skip_size = 0;
//     auto right_size = 0;
//     window.log = AccessLog(window.ifstream);
//     window.time = ConvertTimeFromTimestamp(window.log);

//     std::string temp_log = AccessLog(window.ifstream);
//     u_int32_t temp_time = ConvertTimeFromTimestamp(temp_log);

//     while (!window.ifstream.eof()) {
//         if (temp_time - window.time + 1 <= interval_time) {
//             ++count_window_request;
//             temp_log = AccessLog(window.ifstream);
//             temp_time = ConvertTimeFromTimestamp(temp_log);
//             right_size += temp_log.size();
//         } else {
//             skip_size += window.log.size();
//             window.ifstream.seekg(skip_size);
//             window.log = AccessLog(window.ifstream);
//             window.time = ConvertTimeFromTimestamp(window.log);
//             window.ifstream.seekg(right_size);
//         }
//     }
//     return count_window_request;
// }
