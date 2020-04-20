#ifndef PROJECT_SOCKET_H
#define PROJECT_SOCKET_H

//#define __EMSCRIPTEN__ 1

#ifdef __EMSCRIPTEN__
#include <emscripten/websocket.h>
#endif

#include <string>
#include <vector>
#include <functional>

class Socket {
public:
    enum EventType {
        None, Open, Close, Error, Message
    };

    struct EventData {
        EventType type = None;

        int closeCode = 0;
        std::string closeReason = "";

        unsigned char *messageData = nullptr;
        int messageBytes = 0;
    };

    Socket(std::string inUrl, std::function<void(Socket::EventData)>);
    ~Socket();

    void send(const std::string &message);
    void close();

private:
    union PointerInt {
        void *ptr;
        int i;
    };

    std::string url;
    static int socketIndex;
    static std::vector<std::function<void(Socket::EventData)>> callbacks;


#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_WEBSOCKET_T socket;

    static int websocketOpen(int eventType, const EmscriptenWebSocketOpenEvent *event, void *userData);
    static int websocketClose(int eventType, const EmscriptenWebSocketCloseEvent *event, void *userData);
    static int websocketError(int eventType, const EmscriptenWebSocketErrorEvent *event, void *userData);
    static int websocketMessage(int eventType, const EmscriptenWebSocketMessageEvent *event, void *userData);
#endif
};


#endif //PROJECT_SOCKET_H
