#define _CRT_SECURE_NO_WARNINGS

// ✅ Archivo: main.cpp
#include <iostream>
#include "Ledger.h"
#include "Transaction.h"

extern void initDB();
extern void generateRSAKeyPair();
extern void addBlock(const Transaction& tx);
extern void printBlockchain();
extern void shutdownDB();

int main() {
    initDB();
    generateRSAKeyPair();

    ledger.issueCoins("Alice", 100);
    Transaction tx1("SYSTEM", "Alice", 100);
    addBlock(tx1);

    ledger.issueCoins("Bob", 50);
    Transaction tx2("SYSTEM", "Bob", 50);
    addBlock(tx2);

    if (ledger.applyTransaction(Transaction("Alice", "Bob", 30))) {
        addBlock(Transaction("Alice", "Bob", 30));
    }

    if (ledger.applyTransaction(Transaction("Bob", "Charlie", 20))) {
        addBlock(Transaction("Bob", "Charlie", 20));
    }

    ledger.printBalances();
    printBlockchain();
    shutdownDB();
    return 0;
}

// ✅ Archivo: Blockchain.h
#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "Transaction.h"

void initDB();
void generateRSAKeyPair();
void shutdownDB();
void addBlock(const Transaction& tx);
void printBlockchain();

#endif
