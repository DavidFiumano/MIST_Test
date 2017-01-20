#include <string>
#include <fstream>
#include <thread>
#include <chrono>

#include <time.h>
#include <MIST.pb.h>
#include <networking/SendData.hpp>
#include <networking/ReceiveData.hpp>
#include <MIST.hpp>
#include <Machine.hpp>
#include <stdlib.h>

//TODO .pop_back() protobuf string?
//std::string message;
std::string data;
std::string task;
std::string firstTwoChars;
//std::string dump_string;

ProtobufMIST::Task _task;

const char d = 182;
const char d_spc = 185;
int part;

std::string random_salt(std::string s) {
    std::string copy = "";
    std::string chars = "abcdefghijklmnopqrstufwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ12345678910!@#$%^&*()_+-=";
    for (std::string::iterator i = s.begin(); i != s.end(); i++) {
        copy.push_back(*i);
        srand(time(0));
        if ((rand() % 100) < 10) {
            srand(time(0));
            copy.push_back(chars.at(rand() % chars.length()));
        }
    }
    return copy;
}

std::string add_salt(std::string s) {
    std::string copy = "";
    for (std::string::iterator i = s.begin(); i != s.end(); i++) {
        copy.push_back(*i);
        if (*i == '6') { // memes
            copy.push_back('w');
            copy.push_back('h');
            copy.push_back('a');
            copy.push_back('t');
        }
        else if (*i == 'c' && *i == '9' && *i == 'D') { //c9D
                                                        //HG6v
            copy.pop_back();
            copy.push_back(*i);
            copy.push_back('G');
            copy.push_back('6');
            copy.push_back('v');
        }
        else if (tolower(*i) == 'm' && tolower(*(i + 1)) == 'i') { //memes
            copy.push_back('s');
            copy.push_back('t');
        }
    }
    return copy;
}

void hash()
{
    data = random_salt(data); //randomly salt
    data = add_salt(data); //add random chars
}

/*int main() {

    ReceiveData rObj(1025);
    part = -1;

    unsigned long long int it = 0;
    bool end = false;

    MIST::Task task("hash", *hash);

    std::cout << "Getting role" << std::endl;
    int i = 0;
    std::string x;
    bool atEndOfProgram = false;
    while(!atEndOfProgram){
        std::cout << "Loop Starting \n";
        x = rObj.receive<2>();
        std::cout << "recieved \n";
        if (x.at(0) == '1')
        {
            part = 1;
        }
        else if (x.at(0) == '2') {
            part = 2;
        }
        else {
            std::cout << "Something dun goofed " << x.at(0) << std::endl;
        }

        std::cout << "ComputerIsRole " << part << std::endl;
        if (x.at(1) == '0')
        {
            while (!end)
            {
                std::string y = rObj.receive<1>();
            //std::cout << "On it: " << it << " found " << x << std::endl;
            //if(it % 10000 == 0)
                std::cout << "Read " << it << " chars so far \n";
                std::cout << y << std::endl;
                if (y.find(d) != std::string::npos)
                    end = true;
                else
                    message += y;
                it += 1;
            }
            std::cout << "Data recieved: " << message.size() << std::endl;
            std::cout << message << std::endl;
            data = message;
            message.clear();
        }
        end = false;
        if (x.at(1) == '1')
        {
            std::cout << "Task recieved \n";
            while (!end)
            {
                //std::cout << "Task recieved \n";
                std::string z = rObj.receive<1>();
                if (z.find(d) != std::string::npos)
                    end = true;
                else
                    message += z;
                std::string * tempZ = &z;
                delete tempZ;
            }
            end = false;
            if (_task.ParseFromString(message))
            {
                std::cout << "Parsed From String \n";
            }
            else {
                std::cout << "Failure to Parse From String \n";
            }
        }
        std::cout << _task.task_name();
        if (_task.task_name() == "hash")
        {
            std::cout << "Running Hash" << std::endl;
            task.run();
            std::cout << "Hash complete" << std::endl;
        }
        i++;
        x.clear();
    }
    data = std::to_string(part) + data;
    std::cout << "Appending part" << std::endl;
    bool notSent;
    unsigned int tries = 0;
    SendData sObj("192.168.1.111", 1026);
    std::cout << "Returning Data" << std::endl;
    while(notSent) {
        if(notSent) {
            try {
                sObj.send(data, d);
                notSent = false;
            }
            catch (std::exception& e)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << e.what() << std::endl;
            }
        }
        tries++;
    }
    std::cout << "Data sent after " << tries << " tries" << std::endl;
    
    return 0;
}*/
/*int main()
{
    //First I would recieve and check what part I am and record that
    //Second, I would get data
    int part = -1;
    ReceiveData rObj(1025);
    SendData sObj("192.168.1.111", 1026);

    bool isPartSet = false;
    bool isComplete = false;
    while (isComplete != true)
    {
        firstTwoChars += rObj.receive<1>();
        std::cout << firstTwoChars.c_str() << std::endl;

        if (firstTwoChars.length() == 1)
        {
            if (firstTwoChars == "1")
            {
               part = 1;
               isPartSet = true;
               std::cout << "Part = 1 \n";
            }else if (firstTwoChars == "2"){
               part = 2;
               isPartSet = true;
               std::cout << "Part = 2 \n";
            }
        }
        else {
            if(isPartSet == true && firstTwoChars.length() ) 
        }

    return 0;
}*/

int main()
{
    MIST::Task taskThing("hash", *hash);
    ReceiveData * rObj = new ReceiveData(1025);
    SendData sObj("25.88.220.173", 1027);
    std::cout << "Receiving first char \n";
    firstTwoChars = rObj->receive<1>();
    //delete rObj;
    std::cout << firstTwoChars << std::endl;
    std::cout << "Received first char \n";
    int slavePart;
    //firstTwoChars.pop_back();
    if (firstTwoChars == "1") {
        slavePart = 1;
    }
    else if (firstTwoChars == "2") {
        slavePart = 2;
    }
    else
        std::cout << "You fucked up, what part is it? \n";

    std::cout << "Is part " << slavePart << std::endl;
    
    bool dataRecieved = false;
    while (!dataRecieved)
    {
        //!(data.substr(data.length() - 2) == "-1";
        //ReceiveData * rObj_data = new ReceiveData(1025);
        std::string chunk = rObj->receive<1>();
        if (chunk == "-1" || chunk.find((char)182) != std::string::npos) {
            std::cout << "Data recieved \n";
            dataRecieved = true;
            //std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        else
        {
            data += chunk;
            std::cout << "Added chunk: " << chunk << std::endl;
        }
        chunk.clear();
        //delete rObj_data;
    }
    std::cout << "All Data recieved! \n Parsing now \n";
    
    if (data.find(d_spc) != std::string::npos)
    {
        size_t data_before = data.find(d_spc); //find where data ends and task begins
        std::cout << "Data found at " << data_before << "bytes. \n";
        std::string task = data.substr(data_before); //copy task to new string
        std::cout << "Task copied: " << task << std::endl;
        data.erase(data_before);//erase everything that was the task from it's original string
    }
    else {
        std::cout << "Did not find d_spc \n";
        std::abort();
    }

    std::cout << "Data parsed \n Data: \n";
    std::cout << data << std::endl;
    std::cout << "Task: " << task << std::endl;
    //bool taskRecieved = false;
    //std::cout << "Starting loop \n";
    /*while (taskRecieved == false)
    {
        //!(data.substr(data.length() - 2) == "-1";
        std::string t_chunk = rObj.receive<1>();
        if (t_chunk == "-1" || t_chunk.find((char)182) != std::string::npos) {
            std::cout << "task recieved \n";
            taskRecieved = true;
            //std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        else
        {
            task += t_chunk;
            std::cout << "Added t_chunk: " << t_chunk << std::endl;
        }
        t_chunk.clear();
    }*/

    if (_task.ParseFromString(task))
    {
        std::cout << "Task parsed properly \n";
    }
    else {
        std::cout << "I fucked up parsing, trying again \n";
        if (_task.ParseFromString(task)) {
            task.pop_back();
            std::cout << "Worked the second time! \n";
        }
        else {
            std::cout << "Still fucked up \n";
            std::abort();
        }
    }
    if(_task.task_name() == "hash")
        taskThing.run();
    std::cout << taskThing.getID();
    std::cout << "Sending... \n";
    std::this_thread::sleep_for(std::chrono::seconds(20));
    std::string t_str = std::to_string(slavePart) + data;
    //while (true)
    //{
        try {
            sObj.send(t_str, d);
            //break;
        }
        catch (std::system_error& e)
        {
            std::cout << e.what() << std::endl;
        }
    //}
        int x;
        std::cin >> x;
    std::cout << "Sent! \n";
}