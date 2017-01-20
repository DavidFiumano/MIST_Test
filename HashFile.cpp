#include <string>
#include <fstream>
#include <time.h>
#include <thread>
#include <chrono>

#include <networking/SendData.hpp>
#include <networking/ReceiveData.hpp>
#include <MIST.hpp>
#include <Machine.hpp>

#define FILE_SIZE 60

std::string random_salt(std::string s) {
    std::string copy = "";
    std::string chars = "abcdefghijklmnopqrstufwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ12345678910!@#$%^&*()_+-=";
    for(std::string::iterator i = s.begin(); i != s.end(); i++) {
        copy.push_back(*i);
        srand(time(0));
        if((rand() % 100) < 10) {
            srand(time(0));
            copy.push_back(chars.at(rand() % chars.length()));
        }
    }
    return copy;
}

std::string add_salt(std::string s) {
    std::string copy = "";
    for(std::string::iterator i = s.begin(); i != s.end(); i++) {
        copy.push_back(*i);
        if(*i == '6') { // memes
            copy.push_back('w');
            copy.push_back('h');
            copy.push_back('a');
            copy.push_back('t');
        } else if(*i == 'c' && *i == '9' && *i == 'D') { //c9D
            //HG6v
            copy.pop_back();
            copy.push_back(*i);
            copy.push_back('G');
            copy.push_back('6');
            copy.push_back('v');
        } else if(tolower(*i) == 'm' && tolower(*(i + 1)) == 'i') { //memes
            copy.push_back('s');
            copy.push_back('t');
        }
    }
    return copy;
}

int main() {
    std::vector<MIST::Machine> machines_used = { MIST::Machine("local"), MIST::Machine("Helper 1", "25.88.30.47", false), MIST::Machine("Helper 2", "25.88.123.114", false) };
    auto mist = MIST::MIST(true, machines_used);

    std::ifstream hash;
    std::string data1 = "";
    std::string data2 = "";
    std::string mydata = "";
    printf("Dangerously large file being imported into code...\n");
    hash.open("Hash_Me", std::fstream::binary);
    if(hash.is_open()) {
        try {
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

    std::string one(""), two("");
    unsigned short port1 = 1026;
    unsigned short port2 = 1027;

    auto receive_slaves = [=](unsigned short& port, std::string& out) {
        bool got = false;
        printf("Looking for string on port %u\n", port);
        while(!got) {
            auto slave = new ReceiveData(port);
            std::string x = slave->receive<1>();
            //printf("Got chunk: %s\n", x.c_str());
            if(!(x.find((char)182) != std::string::npos || x == "-1")) {
                out += x;
            } else {
                got = true;
            }
            delete slave;
        }
        printf("Received a full string!\n");
    };

    printf("data1: %s data2: %s mydata: %s", data1.substr(0, 10).c_str(), data2.substr(0, 10).c_str(), mydata.substr(0, 10).c_str());

    //start waiting in the background without ruining main process
    printf("Spawning threads...\n");
    auto t = new std::thread(receive_slaves, std::ref(port1), std::ref(one));
    auto t2 = new std::thread(receive_slaves, std::ref(port2), std::ref(two));
    t->detach();
    t2->detach();

    ProtobufMIST::Task task;
    task.set_task_name("hash");
    std::string serialized;
    task.SerializeToString(&serialized);

    const char c = 185;

    printf("Send all!\n");
    std::string s1 = "1" + data1 + c + serialized;
    std::string s2 = "2" + data2 + c + serialized;


    mist.send_task(s1, "Helper 1", 1025);
    printf("Updated first task!\n");

    mist.send_task(s2, "Helper 2", 1025);
    printf("Updated first task!\n");

    std::string mydata_salted = add_salt(random_salt(mydata)); //TODO: Add pepper

    printf("Old mydata size: %zu\nNew mydata size: %zu\n", mydata.length(), mydata_salted.length());


    //printf("Openning both receive channels...\n");
    //printf("Waiting for receive threads to exit...\n");
    printf("(Hopefully) received all parts!\n");

    printf("Removing delimiters...\n");
    one.erase(std::remove(one.begin(), one.end(), (char)182), one.end());
    two.erase(std::remove(two.begin(), two.end(), (char)182), two.end());

    printf("one: %s\ntwo: %s\n", one.substr(0, 10).c_str(), two.substr(0, 10).c_str());

    std::ofstream output;
    output.open("Hashed");
    output << one << two << mydata_salted;
    output.close();

    printf("Cleaning threads...\n");
    delete t, t2;

    printf("Aloha!\n");
    return 0;
}
