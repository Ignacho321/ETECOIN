//ledger.h
#ifndef LEDGER_H
#define LEDGER_H

#include <map>
#include <string>
#include <mutex>
#include <iostream>
#include "Transaction.h"

class Ledger {
private:
    std::map<std::string, double> balances;
    const double MAX_SUPPLY = 21000000.0;
    double totalIssued = 0.0;
    mutable std::mutex ledgerMutex;

public:
    bool issueCoins(const std::string& to, double amount);
    bool applyTransaction(const Transaction& tx);
    double getBalance(const std::string& address) const;
    void printBalances() const;
};

extern Ledger ledger;

#endif
