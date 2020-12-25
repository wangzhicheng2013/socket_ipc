
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <iostream>
#include <string>
#include <chrono>
#include "base_thread.hpp"
class ipc_socket_client final : public base_thread {
    virtual void process() override {
        std::cout << "I am a ipc socket client thread." << std::endl;
        std::string str;
        while (true) {
            data_process(str);
            if (write(sock_fd_, str.c_str(), str.size()) < 0) {
                std::cerr << "send to server failed." << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
public:
    virtual ~ipc_socket_client() {
        if (sock_fd_ >= 0) {
            close(sock_fd_);
        }
    }
public:
    bool init() {
        sock_fd_ = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sock_fd_ < 0) {
            return false;
        }
        un_.sun_family = AF_UNIX;
        strncpy(un_.sun_path, socket_path_, sizeof(un_.sun_path) - 1);
        if (connect(sock_fd_, (struct sockaddr *)&un_, sizeof(un_)) < 0) {
            std::cerr << "client connect failed for ipc path:" << socket_path_ << std::endl;
            return false;
        }
        return true;
    }
    inline void set_socket_path(const char *path) {
        socket_path_ = path;
    }
    void data_process(std::string &str) {
        str.assign(BUFF_SIZE, 'A');
    }
private:
    int sock_fd_ = -1;
    struct sockaddr_un un_ = { 0 };
private:
    const char *socket_path_ = "./ipc.socket";
    static const int BUFF_SIZE = 1024;
};