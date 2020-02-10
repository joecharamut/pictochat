#include "Network.h"

#include <SDL_net.h>
#include <string>
#include <vector>

void Network::test() {
    if (SDLNet_Init() < 0) {
        printf("error init net: %s\n", SDL_GetError());
        return;
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, "www.spaghetti.rocks", 80) < 0) {
        printf("error resolve: %s\n", SDL_GetError());
        return;
    }

    TCPsocket sock;
    sock = SDLNet_TCP_Open(&ip);
    if (!sock) {
        printf("error opening socket: %s\n", SDL_GetError());
        return;
    }

    std::string request = "GET /form.php?view=1 HTTP/1.0\r\n"
                          "Host: www.spaghetti.rocks\r\n"
                          "\r\n";

    if (SDLNet_TCP_Send(sock, request.c_str(), request.length() + 1) < request.length() + 1) {
        printf("error send: %s\n", SDL_GetError());
        return;
    }

    std::vector<unsigned char> data;
    unsigned char buf[1];
    int read;

    do {
        read = SDLNet_TCP_Recv(sock, buf, 1);
        data.push_back(buf[0]);
    } while (read > 0);

    std::string msg;

    for (unsigned char c : data) {
        msg += c;
    }

    printf("message: %s\n", msg.c_str());

    SDLNet_TCP_Close(sock);
    SDLNet_Quit();
}
