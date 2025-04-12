#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <iostream>
#include "Transaction.h"
#include "Ledger.h"
#include "Blockchain.h"

void handleIncomingBlock(const std::string& rawData) {
    std::stringstream ss(rawData);
    std::string timestamp, sender, receiver, amountStr, prevHash, hash, pubKeyAndSig;

    std::getline(ss, timestamp);
    std::getline(ss, sender);
    std::getline(ss, receiver);
    std::getline(ss, amountStr);
    std::getline(ss, prevHash);
    std::getline(ss, hash);
    std::getline(ss, pubKeyAndSig);

    double amount = std::stod(amountStr);
    size_t sep = pubKeyAndSig.find("|");
    if (sep == std::string::npos) {
        std::cerr << "❌ Formato de firma inválido.\n";
        return;
    }

    std::string pubKey = pubKeyAndSig.substr(0, sep);
    std::string signature = pubKeyAndSig.substr(sep + 1);

    // Podés agregar verificación aquí

    Transaction tx(sender, receiver, amount);
    if (ledger.applyTransaction(tx)) {
        addBlock(tx);
        std::cout << "✅ Bloque válido recibido y agregado\n";
    } else {
        std::cout << "❌ Transacción inválida. Bloque rechazado\n";
    }
}
