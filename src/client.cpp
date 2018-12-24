#include "cpplibsocket/Socket.h"

#include <array>
#include <iostream>
#include <string>

using namespace cpplibsocket;

static std::string toString(const Byte* data, const UnsignedSize size) {
    assert(data != nullptr);
    const char* const strBuffer = reinterpret_cast<const char*>(data);
    return std::string(strBuffer, strBuffer + size);
}

int main() {
    try {
        Socket<IPProto::TCP> clientSocket(IPVer::IPV4);
        clientSocket.connect("127.0.0.1", 19850);

        static const std::string firstPayload = "Knock knock!";
        Expected<UnsignedSize> sent = clientSocket.send(reinterpret_cast<const Byte*>(firstPayload.c_str()), firstPayload.size());
        if (!sent) {
            std::clog << sent.error() << '\n';
            return 1;
        }

        std::array<Byte, 4096> response;
        const Expected<UnsignedSize> recieved = clientSocket.receive(reinterpret_cast<Byte*>(&response[0]), response.max_size());
        if (recieved) {
            std::clog << toString(response.data(), *recieved) << '\n';
        } else {
            std::clog << "Something went pee pee poo poo!\n";
            return 1;
        }

        static const std::string secondPayload = "Pee pee poo poo";
        sent = clientSocket.send(reinterpret_cast<const Byte*>(secondPayload.c_str()), secondPayload.size());
        if (!sent) {
            std::clog << sent.error() << '\n';
            return 1;
        }
    } catch (const Exception& e) {
        std::clog << "Something went pee pee poo poo - " << e.what() << '\n';
        return 1;
    }
    return 0;
}
