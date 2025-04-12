

// Ledger.cpp
#include "Ledger.h"

#define _CRT_SECURE_NO_WARNINGS

Ledger ledger;  // Instancia global

bool Ledger::issueCoins(const std::string& to, double amount) {
    std::lock_guard<std::mutex> lock(ledgerMutex);
    if (totalIssued + amount > MAX_SUPPLY) {
        std::cerr << "❌ No se pueden emitir más monedas. Suministro máximo alcanzado.\n";
        return false;
    }
    balances[to] += amount;
    totalIssued += amount;
    return true;
}

bool Ledger::applyTransaction(const Transaction& tx) {
    std::lock_guard<std::mutex> lock(ledgerMutex);
    if (balances[tx.getSender()] >= tx.getAmount()) {
        balances[tx.getSender()] -= tx.getAmount();
        balances[tx.getReceiver()] += tx.getAmount();
        return true;
    }
    std::cerr << "❌ Transacción inválida. Fondos insuficientes en: " << tx.getSender() << "\n";
    return false;
}


double Ledger::getBalance(const std::string& address) const {
    std::lock_guard<std::mutex> lock(ledgerMutex);
    auto it = balances.find(address);
    return (it != balances.end()) ? it->second : 0.0;
}

void Ledger::printBalances() const {
    std::lock_guard<std::mutex> lock(ledgerMutex);
    std::cout << "--- Balances actuales ---\n";
    for (const auto& pair : balances) {
        std::cout << pair.first << ": " << pair.second << " coins\n";
    }
    std::cout << "--------------------------\n";
}