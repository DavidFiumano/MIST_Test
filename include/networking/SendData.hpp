#pragma once

#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif

#include <asio.hpp>
#include <chrono>
#include <vector>
#include <string>
#include <tuple>
#include <iostream>

//#include "SendReceiveConsts.hpp"

using asio::ip::tcp;
typedef unsigned short ushort;

class SendData {
private:
    asio::io_service service;
    tcp::resolver resolver;
    tcp::resolver::query query;
    tcp::socket socket;
    tcp::resolver::iterator endpoint_iterator;

    std::string IP;
    ushort port;

    inline void send_string(std::string dataToSend, const char &separator = '\0') {
        if(!this->socket.is_open()) {
            asio::connect(this->socket, this->endpoint_iterator);
        }

        service.run();

        std::string MISTCompliant = dataToSend;
        MISTCompliant.push_back(separator);
        printf("Sent %lu to %s\n", asio::write(socket, asio::buffer(MISTCompliant.c_str(), MISTCompliant.length())), IP.c_str());
    };

public:
    SendData(std::string IP, ushort port) //TODO
        : resolver(service),
          query(IP, std::to_string(port)),
          socket(service) {
       this->IP = IP;
       this->port = port;
       this->endpoint_iterator = resolver.resolve(this->query);
   }

   ~SendData() {
       //stop();
   }

    // EXTREME ASIO WRAPPING!!!
    DEPRECATED(void simple_send(std::string data));

    inline void send(std::string data, const char &separator = '\0') {
        send_string(data, separator);
    }

    inline void stop() {
        asio::error_code error;
        this->socket.shutdown(tcp::socket::shutdown_type::shutdown_receive, error);
        if(error) {
            printf("An error occurred when shutting down SendData socket: %s (File: %s, Line %i)\n", error.message().c_str(), __FILE__, __LINE__);
        }
        this->socket.close();
        printf("Socket closed.\n");
    }

    inline std::tuple<std::string, ushort> get_raw_info() {
        return std::tuple<std::string, ushort>(this->IP, this->port);
    }
};
