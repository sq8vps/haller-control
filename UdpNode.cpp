#include "UdpNode.hpp"
#include <string>
#include <QDebug>
#include <arpa/inet.h> // htons, inet_addr
#include <netinet/in.h> // sockaddr_in
#include <cstdint> // uint16_t
#include <sys/socket.h> // socket, sendto
#include <unistd.h> // close

UdpNode::UdpNode()
{

}

void UdpNode::sendMessage()
{
    std::string hostname{"1.2.3.4"};
    uint16_t port = 9000;

    int sock = ::socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in destination;
    destination.sin_family = AF_INET;
    destination.sin_port = htons(port);
    destination.sin_addr.s_addr = inet_addr(hostname.c_str());

    std::string msg = "Mess sent";
    int n_bytes = ::sendto(sock, msg.c_str(), msg.length(), 0, reinterpret_cast<sockaddr*>(&destination), sizeof(destination));
    ::close(sock);
    qDebug() << "in udpnode";
}
