
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
#include "base_thread.hpp"
class ipc_socket_server final : public base_thread {
    virtual void process() override {
        std::cout << "I am a ipc socket server thread." << std::endl;
        char buf[BUFF_SIZE] = "";
        while (true) {
            int client_fd = accept(sock_fd_, nullptr, nullptr);
            if (client_fd < 0) {
                std::cerr << "accept error." << std::endl;
                continue;
            }
            int len = 0;
            while ((len = read(client_fd, buf, sizeof(buf))) > 0) {
                data_process(buf, len);
            }
            if (len < 0) {
                std::cerr << "server read exception." << std::endl;
                break;
            }
            std::cout << "link closed." << std::endl;
            close(client_fd);
        }
    }
public:
    virtual ~ipc_socket_server() {
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
        unlink(socket_path_);
        if (bind(sock_fd_, (struct sockaddr *)&un_, sizeof(un_)) < 0) {
            std::cerr << "server bind failed for ipc path:" << socket_path_ << std::endl;
            return false;
        }
        if (listen(sock_fd_, queue_len_) < 0) {
            std::cerr << "listen failed for sock fd:" << sock_fd_ << " queue len:" << queue_len_ << std::endl;
            return false;
        }
        return true;
    }
    inline void set_socket_path(const char *path) {
        socket_path_ = path;
    }
    inline void set_queue_len(int len) {
        queue_len_ = len;
    }
    void data_process(char *buf, int len) {
        for (int i = 0;i < len;i++) {
            cout << buf[i];
        }
        std::cout << std::endl;
    }
private:
    int sock_fd_ = -1;
    int queue_len_ = 128;
    struct sockaddr_un un_ = { 0 };
private:
    const char *socket_path_ = "./ipc.socket";
    static const int BUFF_SIZE = 1024;
};