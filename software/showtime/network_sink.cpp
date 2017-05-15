#include "showtime/network_sink.h"

#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

namespace showtime {

namespace {

constexpr int kPort = 12345;

} // namespace

NetworkSink::NetworkSink() {
  socket_ = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socket_ == -1) {
    std::cout << "Failed to open network socket!" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  sockaddr_in addr;
  std::memset(&addr, 0, sizeof(addr));
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(kPort);
  addr.sin_family = PF_INET;
  if (bind(socket_, (sockaddr*)&addr, sizeof(addr)) != 0) {
    std::cout << "Failed to bind network socket!" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  int enabled = 1;
  if (setsockopt(socket_, SOL_SOCKET, SO_BROADCAST, &enabled, sizeof(int)) != 0) {
    std::cout << "Failed to enable broadcast packets!" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::memset(&addr_, 0, sizeof(addr_));
  addr_.sin_addr.s_addr = htonl(-1); // 255.255.255.255
  addr_.sin_port = htons(kPort);
  addr_.sin_family = PF_INET;

  last_packet_time_ = glfwGetTime();
}

NetworkSink::~NetworkSink() {
  shutdown(socket_, SHUT_RDWR);
  close(socket_);
}

void NetworkSink::sink(ColorChannels channels) {
  const double delay = 0.1;
  const double now = glfwGetTime();
  if (now >= last_packet_time_ + delay) {
    packet_ = quantizeChannels(channels);
    sendto(socket_, packet_.data(), packet_.size() * sizeof(ByteColor), 0,
           (sockaddr*)&addr_, sizeof(addr_));
    last_packet_time_ = now;
  }
}

} // namespace showtime
