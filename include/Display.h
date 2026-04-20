#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Display {
public:
    // This function prints the CineSearch banner.
    void printBanner();

    // This function prints the main menu.
    void printMenu();

    // This function prints scenario title header.
    void printScenarioHeader(string scenarioTitle);

    // This function prints results section header.
    void printResultsHeader();

    // This function prints one result line.
    void printResult(string title);

    // This function prints performance section header.
    void printPerfHeader();

    // This function prints one performance row.
    void printPerfRow(string approach, long long timeUs, string complexity);

    // This function prints winner name.
    void printWinner(string dsName);

    // This function prints wrapped explanation paragraph.
    void printWhyExplanation(string explanation);

    // This function waits for enter key.
    void pressEnterToContinue();

    // This function prints a section line with title.
    void printSectionLine(string title);

    // This function prints complexity table for all DS.
    void printComplexityTable();

    // This function prints success message.
    void printSuccess(string msg);

    // This function prints info message.
    void printInfo(string msg);

    // This function prints warning message.
    void printWarn(string msg);

    // This function prints loading message.
    void printLoading(string msg);

    // This function formats integer microseconds with commas.
    string formatTime(long long t);
};
