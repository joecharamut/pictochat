#include "Socket.h"

#include <utility>

int Socket::socketIndex = -1;
std::vector<std::function<void(Socket::EventData)>> Socket::callbacks;

Socket::Socket(std::string inUrl, std::function<void(Socket::EventData)> callback) : url(std::move(inUrl)) {
    socketIndex++;
    callbacks.push_back(callback);

#ifdef __EMSCRIPTEN__
    EmscriptenWebSocketCreateAttributes attr;
    emscripten_websocket_init_create_attributes(&attr);
    attr.url = url.c_str();
    socket = emscripten_websocket_new(&attr);
    if (socket <= 0) {
        printf("websocket creation failed\n");
        exit(1);
    }

    emscripten_websocket_set_onopen_callback(socket, (void *) socketIndex, websocketOpen);
    emscripten_websocket_set_onclose_callback(socket, (void *) socketIndex, websocketClose);
    emscripten_websocket_set_onerror_callback(socket, (void *) socketIndex, websocketError);
    emscripten_websocket_set_onmessage_callback(socket, (void *) socketIndex, websocketMessage);
#endif
}

Socket::~Socket() {

}

void Socket::send(const std::string &message) {
#ifdef __EMSCRIPTEN__
    emscripten_websocket_send_utf8_text(socket, message.c_str());
#endif
}

void Socket::close() {
#ifdef __EMSCRIPTEN__
    emscripten_websocket_close(socket, 1000, "closed");
    emscripten_websocket_delete(socket);
#endif
}

#ifdef __EMSCRIPTEN__

int Socket::websocketOpen(int eventType, const EmscriptenWebSocketOpenEvent *event, void *userData) {
    PointerInt index { .ptr = userData };
    callbacks[index.i](EventData { .type = Open });
    return 0;
}

int Socket::websocketClose(int eventType, const EmscriptenWebSocketCloseEvent *event, void *userData) {
    PointerInt index { .ptr = userData };
    callbacks[index.i](EventData { .type = Close, .closeCode = event->code, .closeReason = std::string(event->reason) });
    return 0;
}

int Socket::websocketError(int eventType, const EmscriptenWebSocketErrorEvent *event, void *userData) {
    PointerInt index { .ptr = userData };
    callbacks[index.i](EventData { .type = Error });
    return 0;
}

int Socket::websocketMessage(int eventType, const EmscriptenWebSocketMessageEvent *event, void *userData) {
    PointerInt index { .ptr = userData };
    callbacks[index.i](EventData { .type = Message, .messageData = event->data, .messageBytes = (int) event->numBytes });
    return 0;
}

#endif
