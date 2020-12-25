#include "ipc_socket_server.hpp"
#include "ipc_socket_client.hpp"
int main() {
    ipc_socket_server server;
    ipc_socket_client client;
    if (!server.init() || !client.init()) {
        return -1;
    }
    server.run();
    sleep(5);
    client.run();
    server.join();
    client.join();
    
    return 0;
}