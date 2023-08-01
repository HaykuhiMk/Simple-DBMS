#include "dbms.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

#include <string.h>

DBMS::DBMS(){
    this->addInstruction("create table", &DBMS::createTable);
    this->addInstruction("insert into", &DBMS::insertData);
    this->addInstruction("delete from", &DBMS::deleteFrom);
}

void DBMS::run(){
    std::string command;
    while (true) {
        std::cout << "Enter a command: ";
        std::getline(std::cin, command);
        if (command == "exit") {
            break;
        }
        checkStringForKeys(command);
    }
}

void DBMS::checkStringForKeys(const std::string& command) {
    bool flag = false;
    for (const auto& entry : instructionSet) {
        std::size_t instr = command.find(entry.first);
        if (instr != std::string::npos) {
            std::string payload = command.substr(instr + entry.first.length());
            (this->*(entry.second))(payload);
            flag = true;
            break; 
        }
    }
    if (!flag)
    {
        std::cout << "Undefined instruction!" << std::endl;
    }
}

void DBMS::deleteFrom(const std::string& payload)
{
    std::istringstream iss(payload);
    std::string tableName;
    iss >> tableName;
    if (!tableExists(tableName)) {
        std::cout << "Table '" << tableName << "' does not exist.\n";
        return;
    }
    std::string fileName = tableName + ".txt";
    std::ifstream entityFile(fileName);
    std::ofstream tmpFile("tmp.txt");
    if (!entityFile.is_open()) {
        std::cerr << "Error opening the entity file." << std::endl;
        return;
    }
    if (!tmpFile.is_open()) {
        std::cerr << "Error opening the temporary file." << std::endl;
        entityFile.close();
        return;
    }
    std::string payloadCopy = payload;
    std::size_t startPos = payload.find('{');
    std::size_t endPos = payload.find('}');
    if (startPos != std::string::npos && endPos != std::string::npos) {
        payloadCopy = payload.substr(startPos + 1, endPos - startPos - 1);
    } else {
        std::cerr << "Opening or closing curly braces not found in the string." << std::endl;
    }
    std::string line;
    while (std::getline(entityFile, line)) {
        if (line.find(payloadCopy) == std::string::npos) {
            tmpFile << line << std::endl;
        }
    }
    entityFile.close();
    tmpFile.close();
    std::remove(fileName.c_str());
    std::rename("tmp.txt", fileName.c_str());

}

void DBMS::insertData(const std::string& payload)
{
    std::istringstream iss(payload);
    std::string tableName;
    iss >> tableName;
    if (!tableExists(tableName)) {
        std::cout << "Table '" << tableName << "' does not exist.\n";
        return;
    }
    char sym;
    std::string data;
    std::vector<std::string> dataVec;
    while (iss >> sym) {
        if (sym == '{') {
            while (iss >> data && data != "}") {
                removeUnwantedSymbols(data);
                dataVec.push_back(data);
            }
            break;
        }
    } 
    std::fstream entityFile(tableName + ".txt", std::ios::in | std::ios::out | std::ios::app);
    if (!entityFile.is_open()) {
        std::cerr << "Error opening the entity file." << std::endl;
        return;
    }
    std::string line;
    std::getline(entityFile, line);
    int columnCount = countOfColumns(line);
    if (dataVec.size() != columnCount) 
    {
        std::cout << "The number of columns is incorrect!" << std::endl;
        return;
    }
    for (const std::string& dt : dataVec) {
        entityFile << dt << " ";
    }
    entityFile << '\n';
    entityFile.close();
}

void DBMS::createTable(const std::string& payload) {
    std::istringstream iss(payload);
    std::string tableName;
    iss >> tableName;
    if (tableExists(tableName)) {
        std::cout << "Table '" << tableName << "' already exists.\n";
        return;
    }
    char sym;
    std::string columnName;
    std::vector<std::string> columnNames;
    while (iss >> sym) {
        if (sym == '{') {
            while (iss >> columnName && columnName != "}") {
                removeUnwantedSymbols(columnName);
                columnNames.push_back(columnName);
            }
            break;
        }
    }
    createHelper(columnNames, tableName, tableName + ".txt");
    createHelper(columnNames, tableName, "meta.txt");
    std::cout << "Table '" << tableName << "' created successfully.\n";
}

bool DBMS::tableExists(const std::string& tableName) const {
    std::ifstream metaFile("meta.txt");
    if (!metaFile.is_open()) {
        std::cerr << "Error opening meta file: " << std::endl;
        return false;
    }
    std::string firstWord;
    while (metaFile >> firstWord) {
        if (firstWord == tableName) {
            metaFile.close();
            return true;
        }
        metaFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    metaFile.close();
    return false;
}

void DBMS::createHelper(const std::vector<std::string>& columnNames, const std::string& tableName, const std::string& fileName) const {
    std::ofstream outFile(fileName, std::ios::app);
    if (!outFile) {
        std::cerr << "Error opening the file.\n";
        return;
    }
    outFile << tableName << " : ";
    for (size_t i = 0; i < columnNames.size(); ++i) {
        outFile << columnNames[i];
        if (i != columnNames.size() - 1) {
            outFile << ", ";
        }
    }
    outFile << '\n';
    outFile.close();
}

void DBMS::showTables() const {
    std::ifstream file("meta.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening meta file: " << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    file.close();
}

void DBMS::addInstruction(const std::string& command, void (DBMS::*functionPtr)(const std::string&)) {
    instructionSet[command] = functionPtr;
}

void DBMS::removeUnwantedSymbols(std::string& command){
    auto isUnwantedChar = [](char c) {
        return c == ',' || c == ';' || c == '}';
    };
    command.erase(std::remove_if(command.begin(), command.end(), isUnwantedChar), command.end());
}

int DBMS::countOfColumns(const std::string& line)
{
   int columnsCount = 0;
    for (char ch : line) {
        if (ch == ',') {
            columnsCount++;
        }
    }
    return ++columnsCount;
}