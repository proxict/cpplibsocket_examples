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
        Socket<IPProto::TCP> serverSocket(IPVer::IPV4);
        serverSocket.bind("127.0.0.1", 19850);
        serverSocket.listen(256);

        std::cout << "Awaiting connection...\n";

        while (serverSocket.isOpen()) {
            Expected<Socket<IPProto::TCP>> client = serverSocket.accept();
            if (!client) {
                std::clog << client.error() << '\n';
                serverSocket.close();
                break;
            }

            std::cout << "Client connected.\n";

            std::array<Byte, 4096> buffer;
            Expected<UnsignedSize> recieved = client->receive(buffer.data(), buffer.max_size());
            if (!recieved) {
                std::clog << recieved.error() << '\n';
                continue;
            }
            std::cout << toString(buffer.data(), *recieved) << '\n';

            static const std::string question = "Who's there?";
            const Expected<UnsignedSize> sent =
                client->send(reinterpret_cast<const Byte*>(&question[0]), question.size());
            if (!sent) {
                std::clog << sent.error() << '\n';
                continue;
            }

            recieved = client->receive(buffer.data(), buffer.max_size());
            if (!recieved) {
                std::clog << recieved.error() << '\n';
                continue;
            }
            std::cout << toString(buffer.data(), *recieved) << '\n';
        }
    } catch (const Exception& e) {
        std::clog << "Something went pee pee poo poo - " << e.what() << '\n';
        return 1;
    }
    return 0;
}
