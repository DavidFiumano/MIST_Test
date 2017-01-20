#pragma once

#include <asio.hpp>

using asio::ip::tcp;

namespace SendReceive {
    asio::io_service global_io_service;
}