#include "server.hpp"

INITIALIZE_EASYLOGGINGPP

int main(){
    int port = 5000;
    Server obj;
    obj.Init(port, "keys/server.crt", "keys/server.key", "keys/ca.crt");
    obj.Connect();
}
