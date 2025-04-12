#ifndef NETWORK_H
#define NETWORK_H

#include <string>

void startServer(int port);
void connectToPeer(const std::string& ip, int port, const std::string& data);
void handleIncomingBlock(const std::string& rawData);

#endif
