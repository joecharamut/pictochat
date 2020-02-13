#include <utility>

#ifndef PROJECT_REQUEST_H
#define PROJECT_REQUEST_H

#include <string>
#include <vector>
#include <functional>
#include <map>
#include <memory>

//#define __EMSCRIPTEN__ 1

#ifdef __EMSCRIPTEN__
#include <emscripten/fetch.h>
#else
#include <cpp-httplib/httplib.h>
#endif

namespace Network {
    typedef unsigned char byte;

    enum Method {
        GET, POST
    };

    enum Status {
        SUCCESS, FAILURE
    };

    struct Response {
        unsigned long long int size;
        std::shared_ptr<std::vector<byte>> data;
        Status status;
        int statusCode;
        std::string statusText;

        Response(unsigned long long int size, std::shared_ptr<std::vector<byte>> data, Status status, int statusCode,
                std::string statusText) : size(size), data(std::move(data)), status(status), statusCode(statusCode),
                statusText(std::move(statusText)) { };
    };

    class Request {
    public:
        Request(std::string url, Method method, std::function<void(Response)> callback);
        void setHeaders(const std::map<std::string, std::string> &headers);
        void execute();

    private:
        static int requestIndex;
        static std::vector<std::function<void(Response)>> callbacks;

        std::function<void(Response)> callback;
        std::string url;

#ifdef __EMSCRIPTEN__
        emscripten_fetch_attr_t fetch_attr {};

        static void request_callback(emscripten_fetch_t *fetch, Status status, int index);
        static void request_success_callback(emscripten_fetch_t *fetch);
        static void request_fail_callback(emscripten_fetch_t *fetch);
#endif
    };
}

#endif //PROJECT_REQUEST_H
