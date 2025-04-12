
#include "Transaction.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

#define _CRT_SECURE_NO_WARNINGS

Transaction::Transaction(const std::string& from, const std::string& to, double amt)
    : sender(from), receiver(to), amount(amt) {

    time_t now = time(nullptr);
    tm localTime{};
    localtime_s(&localTime, &now);  // uso seguro en Windows

    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
    timestamp = buffer;

    hash = calculateHash();  // este es correcto si calculateHash está bien definido abajo
}

std::string Transaction::getSender() const { return sender; }
std::string Transaction::getReceiver() const { return receiver; }
double Transaction::getAmount() const { return amount; }
std::string Transaction::getTimestamp() const { return timestamp; }
std::string Transaction::getHash() const { return hash; }

std::string Transaction::calculateHash() const {
    std::ostringstream ss;
    ss << timestamp << sender << receiver << amount;

    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(ss.str().c_str()), ss.str().length(), digest);

    std::ostringstream hashStream;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        hashStream << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];

    return hashStream.str();
}
