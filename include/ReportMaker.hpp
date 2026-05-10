//
// Created by Mateusz Miliutin on 10/05/2026.
//

#ifndef ALGOSY2GRAFY_REPORTMAKER_HPP
#define ALGOSY2GRAFY_REPORTMAKER_HPP
#include <fstream>
#include <iostream>


class ReportMaker {
    std::ofstream report_file;
    int line_length = 60;
    int test_counter = 0;
    int report_id;
    static int report_counter;

    public:
    explicit ReportMaker(const std::string& directory) {
        this->report_id = ++report_counter;
        this->report_file.open(directory + "/report_" + std::to_string(this->report_id) + ".log");
    }
    ~ReportMaker() {
        this->report_file.close();
    }

    void line( int length, char letter = '=' );
    void line();

    void title_line(const std::string& title);
    void run_test(const std::string& test_description);
    void header(const std::string& report_description);


    void log(const std::string& message, bool silent = false);
    void sublog(const std::string& message, bool silent = false);
    void suberror(const std::string& message, bool silent = false);

    void silent_log(const std::string& message) { this->log( message, true ); }
    void silent_sublog(const std::string& message) { this->sublog( message, true ); }
    void silent_suberror(const std::string& message) { this->suberror( message, true ); }

    // TODO add log option that selects normal log or error log depending on a bool expression

};


#endif //ALGOSY2GRAFY_REPORTMAKER_HPP