// ✅ Archivo: blockchain.cpp
#define OPENSSL_SUPPRESS_DEPRECATED
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <sqlite3.h>
#include "Blockchain.h"
#include "Ledger.h"



RSA* rsaKeyPair = nullptr;
sqlite3* db = nullptr;

std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)input.c_str(), input.size(), hash);
    std::ostringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str();
}

void generateRSAKeyPair() {
    BIGNUM* bn = BN_new();
    BN_set_word(bn, RSA_F4);
    rsaKeyPair = RSA_new();
    RSA_generate_key_ex(rsaKeyPair, 2048, bn, nullptr);
    BN_free(bn);
}

void initDB() {
    sqlite3_open("blockchain.db", &db);
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS blocks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT,
            sender TEXT,
            receiver TEXT,
            amount REAL,
            hash TEXT
        );
    )";
    char* err = nullptr;
    sqlite3_exec(db, sql, 0, 0, &err);
}

void shutdownDB() {
    if (db) sqlite3_close(db);
    if (rsaKeyPair) RSA_free(rsaKeyPair);
}

std::string getCurrentTimestamp() {
    time_t now = time(nullptr);
    tm localTime{};
    localtime_s(&localTime, &now);  // ✅ versión segura

    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
    return std::string(buffer);
}

void addBlock(const Transaction& tx) {
    std::string timestamp = getCurrentTimestamp();
    std::string data = timestamp + tx.getSender() + tx.getReceiver() + std::to_string(tx.getAmount());
    std::string hash = sha256(data);

    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO blocks (timestamp, sender, receiver, amount, hash) VALUES (?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, timestamp.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, tx.getSender().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, tx.getReceiver().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 4, tx.getAmount());
        sqlite3_bind_text(stmt, 5, hash.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
}

void printBlockchain() {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT timestamp, sender, receiver, amount, hash FROM blocks ORDER BY id;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << "Block: " << sqlite3_column_text(stmt, 0) << " | ";
            std::cout << (const char*)sqlite3_column_text(stmt, 1) << " -> ";
            std::cout << (const char*)sqlite3_column_text(stmt, 2) << " : ";
            std::cout << sqlite3_column_double(stmt, 3) << " | Hash: ";
            std::cout << (const char*)sqlite3_column_text(stmt, 4) << "\n";
        }
    }
    sqlite3_finalize(stmt);
}
