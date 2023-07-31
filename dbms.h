#ifndef DBMS_H
#define DBMS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <map>

class DBMS {
public:
    DBMS();
    ~DBMS() = default;
    void run();
    void createTable(const std::string& payload);
    bool tableExists(const std::string& tableName) const;
    void addInstruction(const std::string& command, void (DBMS::*functionPtr)(const std::string&));
    void insertData(const std::string& payload);
    void deleteFrom(const std::string& payload);
    void showTables() const;

private:
    void removeUnwantedSymbols(std::string& command);
    int countOfColumns(const std::string& line);
    void checkStringForKeys(const std::string& command);
    void createHelper(const std::vector<std::string>& columnNames, const std::string& tableName, const std::string& fileName) const;

private:
    std::unordered_map<std::string, void (DBMS::*)(const std::string&)> instructionSet;
};

#endif  //DBMS_H
