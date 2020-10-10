#include "client.hpp"

INITIALIZE_EASYLOGGINGPP

int main(){
    int port = 5000;
    char str[100];
    Client obj;
    obj.Init("127.0.0.1", port, "keys/client.crt", "keys/client.key", "keys/ca.crt");
    obj.Connect();
    while(1){
        LOG(INFO) << "@client#";
        fgets(str, sizeof(str), stdin);
        obj.Send(str);
    }
}
