#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include "Transaction.h"

void initDB();
void generateRSAKeyPair();
std::string getPublicKeyPEM();
std::string signData(const std::string& data);
std::string sha256(const std::string& input);
std::string getCurrentTimestamp();
std::string getLastBlockHash();
void addBlock(const Transaction& tx);
void printBlockchain();

#endif // BLOCKCHAIN_H