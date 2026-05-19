//
// Created by Mateusz M on 10/05/2026.
//

#include <iostream>
#include <cmath>

#include "../include/ReportMaker.hpp"


int ReportMaker::report_counter = 0;

// Line drawer
void ReportMaker::line(int length, char letter)  {
    for (int i = 0; i < length; i++) {
        std::cout << letter;
        this->report_file << letter;
    }
}
void ReportMaker::line()  {
    line( this->line_length);
    std::cout << std::endl;
    this->report_file << '\n';
}

// Lines with titles
void ReportMaker::title_line(const std::string& title)  {
    const float half = (float)(this->line_length - title.length()) / 2;
    line( ceil( half ) );

    std::cout << title;
    this->report_file << title;

    line( floor( half ) );

    std::cout << std::endl;
    this->report_file << '\n';
}
void ReportMaker::run_test(const std::string& test_description)  {
    title_line( "TEST " + std::to_string( ++this->test_counter ) );
    std::cout << "(i) " << test_description << std::endl;
    this->report_file << "(i) " << test_description << "\n";
}
void ReportMaker::header(const std::string& report_description)  {
    title_line( "REPORT "+std::to_string( this->report_id ) );
    std::cout << "(i) " << report_description << std::endl;
    this->report_file << "(i) " << report_description << "\n";
}


// Logging tools
void ReportMaker::log(const std::string& message, const bool silent) {
    if (!silent) std::cout << "\t* " << message << std::endl;
    this->report_file << "\t* " << message << "\n";
}
void ReportMaker::sublog(const std::string& message, const bool silent) {
    if (!silent) std::cout << "\t\t> " << message << std::endl;
    this->report_file << "\t\t> " << message << "\n";
}
void ReportMaker::suberror(const std::string& message, const bool silent) {
    if (!silent) std::cout << "\t\t! " << message << std::endl;
    this->report_file << "\t\t! " << message << "\n";
}



