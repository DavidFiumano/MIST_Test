#include <string>
#include <fstream>

#include <networking/SendData.hpp>
#include <networking/ReceiveData.hpp>
#include <MIST.hpp>
#include <Machine.hpp>

#define FILE_SIZE 600000000

int main() {
    std::vector<MIST::Machine> machines_used = { MIST::Machine("local"), MIST::Machine("Helper 1", "192.168.1.3", false), MIST::Machine("Helper 2", "192.168.1.4", false) };
    auto mist = MIST::MIST(true, machines_used);

    std::ifstream hash;
    std::string data1 = "";
    std::string data2 = "";
    std::string mydata = "";
    hash.open("Hash_Me", std::fstream::binary);
    if(hash.is_open()) {
        try {
            std::cout << "Dangerously large file being imported into code" << std::endl;
            char chunk;
            int counter = 0;
            while(hash.get(chunk)) {
                if(counter < FILE_SIZE / 3) {
                    data1 += chunk;
                    counter++;
                } else if(counter < FILE_SIZE * (2.0f / 3.0f)) {
                    data2 += chunk;
                    counter++;
                } else {
                    mydata += chunk;
                    counter++;
                }
            }
        } catch(std::exception& e) {
            std::cerr << "Error encountered: " << e.what() << std::endl;
        }
    }
    hash.close();

    std::cout << "data1: " << data1.substr(0, 32) << " data2 " << data2.substr(0, 32) << " mydata " << mydata.substr(0, 32) << std::endl;
    //std::cout << "Hola" << std::endl;

    SendData update_first_helper(machines_used[1].address, 612);
    SendData update_second_helper(machines_used[2].address, 612);

    std::cout << "Sent " << update_first_helper.send("00" + data1) << " to " << machines_used[1].name << " and ";
    std::cout << "sent " << update_second_helper.send("00" + data2) << " to " << machines_used[2].name << std::endl;

    ProtobufMIST::Task task;
    task.task_name = "hash";
    std::string s;
    task.SerializeToString(&s);

    std::cout << "Sent " << update_first_helper.send("01" + s) << " to " << machines_used[1].name << " and ";
    std::cout << "sent " << update_second_helper.send("01" + s) << " to " << machines_used[2].name << std::endl;

    return 0;
}
