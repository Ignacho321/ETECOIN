#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
    std::string sender;
    std::string receiver;
    double amount;
    std::string timestamp;
    std::string hash;

public:
    Transaction(const std::string& from, const std::string& to, double amt);

    std::string getSender() const;
    std::string getReceiver() const;
    double getAmount() const;
    std::string getTimestamp() const;
    std::string getHash() const;

private:
    std::string calculateHash() const;
};

#endif
