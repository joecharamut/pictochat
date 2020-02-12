#include <utility>

#include <utility>
#include <cstring>
#include <iostream>

#include <CxxUrl/url.hpp>

#include "Request.h"

int Network::Request::requestIndex = -1;
std::vector<Network::Request *> Network::Request::requestBuffer;

Network::Request::Request(std::string url, Network::Method method, std::function<void(Response *)> callback) {
    this->callback = std::move(callback);
    this->url = std::move(url);
    requestIndex++;

#ifdef __EMSCRIPTEN__
    emscripten_fetch_attr_init(&this->fetch_attr);

    if (method == GET) {
        strcpy(this->fetch_attr.requestMethod, "GET");
    } else if (method == POST) {
        strcpy(this->fetch_attr.requestMethod, "POST");
    }

    fetch_attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    requestBuffer.push_back(this);

    std::string num = std::to_string(requestIndex);
    char *str = (char *) malloc((num.length() + 1) * sizeof(char));
    strcpy(str, num.c_str());
    fetch_attr.userData = (void *) str;
    fetch_attr.onsuccess = request_success_callback;
    fetch_attr.onerror = request_fail_callback;
#endif
}

void Network::Request::setHeaders(const std::map<std::string, std::string> &headers) {
#ifdef __EMSCRIPTEN__
    int length = (headers.size() * 2) + 1;
    const char *arr[length];
    std::vector<std::string> linear;
    for (const auto &pair : headers) {
        linear.push_back(pair.first);
        linear.push_back(pair.second);
    }

    for (int i = 0; i < linear.size(); i++) {
        arr[i] = linear[i].c_str();
    }

    arr[length - 1] = nullptr;

    fetch_attr.requestHeaders = arr;
#else
    printf("Network::Request::setHeaders() stub\n");
#endif
}

void Network::Request::execute() {
#ifdef __EMSCRIPTEN__
    emscripten_fetch(&fetch_attr, url.c_str());
#else
    Url parsed(url);

    std::unique_ptr<httplib::Client> client = nullptr;
    if (parsed.port().empty()) {
        client = std::make_unique<httplib::Client>(parsed.host());
    } else {
        client = std::make_unique<httplib::Client>(parsed.host(), std::stoi(parsed.port()));
    }

    auto res = client->Get(url.c_str());
    if (res) {
        byte *data = nullptr;
        if (res->body.length() > 0) {
            data = new byte[res->body.length()];
            for (int i = 0; i < res->body.length(); i++) {
                data[i] = res->body[i];
            }
        }

        Response *resp = new Response(
                res->body.length(),
                data,
                SUCCESS,
                res->status,
                "");
        callback(resp);
    } else {
        Response *resp = new Response(0, nullptr, FAILURE, 0, "");
        callback(resp);
    }
#endif
}

#ifdef __EMSCRIPTEN__
void Network::Request::request_callback(emscripten_fetch_t *fetch, Network::Status status) {
    byte *data = new byte[fetch->numBytes];
    if (status == SUCCESS) {
        for (unsigned long long i = 0; i < fetch->numBytes; i++) {
            data[i] = fetch->data[i];
        }
    }

    Response *resp = new Response(
            fetch->numBytes,
            data,
            status,
            fetch->status,
            fetch->statusText
    );
    emscripten_fetch_close(fetch);
    callback(resp);
}

void Network::Request::request_success_callback(emscripten_fetch_t *fetch) {
    int index = std::stoi((char *) fetch->userData);
    requestBuffer[index]->request_callback(fetch, SUCCESS);
}

void Network::Request::request_fail_callback(emscripten_fetch_t *fetch) {
    int index = std::stoi((char *) fetch->userData);
    requestBuffer[index]->request_callback(fetch, FAILURE);
}
#endif
