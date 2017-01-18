#include <string>
#include <fstream>
#include <time.h>
#include <thread>

#include <networking/SendData.hpp>
#include <networking/ReceiveData.hpp>
#include <MIST.hpp>
#include <Machine.hpp>

#define FILE_SIZE 600000000

std::string random_salt(std::string s) {
    std::string copy = s;
    std::string chars = "abcdefghijklmnopqrstufwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ12345678910!@#$%^&*()_+-=";
    for(std::string::iterator i = copy.begin(); i != copy.end(); i++) {
        srand(time(0));
        if((rand() % 100) < 10) {
            srand(time(0));
            copy.insert(i, chars.at(rand() % chars.length()));
        }
    }
    return copy;
}

std::string add_salt(std::string s) {
    std::string copy = s;
    for(std::string::iterator i = copy.begin(); i != copy.end(); i++) {
        if(*i == '6') { // memes
            copy.insert(i + 1, 'w');
            copy.insert(i + 2, 'h');
            copy.insert(i + 3, 'a');
            copy.insert(i + 4, 't');
        } else if(*i == 'c' && *i == '9' && *i == 'D') { //c9D
            //HG6v
            *i = 'H';
            copy.insert(i + 1, 'G');
            copy.insert(i + 2, '6');
            copy.insert(i + 3, 'v');
        } else if(tolower(*i) == 'm' && tolower(*(i + 1)) == 'i') { //memes
            copy.insert(i + 2, 's');
            copy.insert(i + 3, 't');
        }
    }
    return copy;
}

int main() {
    std::vector<MIST::Machine> machines_used = { MIST::Machine("local"), MIST::Machine("Helper 1", "192.168.1.137", false), MIST::Machine("Helper 2", "192.168.1.144", false) };
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

    SendData update_second_helper(machines_used[2].address, 1025);
    SendData update_first_helper(machines_used[1].address, 1025);


    std::cout << "Send data!\n";
    update_first_helper.send("10HFAYM837E6DVJAVFW7QWFMX", (char)182); //TODO: Change back to data1
    std::cout << "Updated first helper\n";

    update_second_helper.send("209383MDHSKEML09DKCYZ6HZ", (char)182); //TODO: Cange back to data2
    std::cout << "Updated second helper!\n";

    ProtobufMIST::Task task;
    task.set_task_name("hash");
    std::string s;
    task.SerializeToString(&s);
    std::cout << s << std::endl;

    std::cout << "Send tasks!\n";

    mist.send_task("11" + s, "Helper 1", 1025);
    std::cout << "Updated first task!\n";

    mist.send_task("21" + s, "Helper 2", 1025);
    std::cout << "Updated second task!\n";

    std::string mydata_salted = add_salt(random_salt("HSHXN7591LOOOAPSLDJNTR4")); //TODO: Add pepper

    std::cout << "Old mydata size: " << mydata.length() << std::endl << "New mydata size: " << mydata_salted.length() << std::endl;

    std::string one(""), two("");
    ReceiveData slave_strings(1026);

    bool both_received;
    std::array<bool, 2> received = { false, false };
    char receiving = '\0';
    std::cout << "Waiting for strings\n";
    while(!both_received) {
        if(std::all_of(std::begin(received), std::end(received), [](bool b) { return b; }))
            both_received = true;
        else {
            std::string x = slave_strings.receive<1>();
            if(receiving == '\0') {
                if(x == "1" && !received[0]) {
                    std::cout << "Compiling string from slave 1!" << std::endl;
                    receiving = '1';
                } else if(x == "2" && !received[1]) {
                    std::cout << "Compiling string from slave 2!" << std::endl;
                    receiving = '2';
                } else {
                    std::cout << "Something's gone wrong! Quitting read loop (" << __LINE__ << " " << __FILE__ << ")" << std::endl;
                    both_received = true;
                }
            } else if(receiving == '1') {
                if(x != "-1" && x.find((char)182) == std::string::npos)
                    one += x;
                else {
                    received[0] = true; // have reached eof
                    std::cout << "Received 1st part!\n";
                    receiving = '\0';
                }
            } else if(receiving == '2') {
                if(x != "-1" && x.find((char)182) == std::string::npos)
                    two += x;
                else {
                    received[1] = true;
                    std::cout << "Received 1nd part!\n";
                    receiving = '\0';
                }
            } else {
                receiving = '\0';
            }
        }
    }

    std::cout << "Received all parts!\n";

    std::cout << "Removing delimiters...\n";
    one.erase(std::remove(one.begin(), one.end(), (char)182), one.end());
    two.erase(std::remove(two.begin(), two.end(), (char)182), two.end());

    std::ofstream output;
    output.open("Hashed");
    output << one << two << mydata_salted;
    output.close();

    std::cout << "Aloha!\n";

    return 0;
}
