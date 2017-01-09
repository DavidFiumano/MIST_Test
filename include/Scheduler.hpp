#pragma once

#include <MIST_Internal.hpp>
#include <networking/ReceiveData.hpp>
#include <MIST.pb.h>
#include <Task.hpp>
#include <vector>
#include <memory>
#include <thread>

class Scheduler {
private:
    std::vector<MIST::Task*> task_queue;
    bool running = false;
    std::thread* checker;

public:
    Scheduler(std::vector<MIST::Task*> task_queue = {}) {
        this->task_queue = task_queue;
        Start();
    }

    ~Scheduler() {
        //Make sure threads are close
        Stop();
        //empty task_queue to prevent evil raw pointers from ruining program
        task_queue.empty();
    }

    void updateTaskVector(std::string id, MIST_taskfunc fn) {
        task_queue.push_back(new MIST::Task(id, fn));
    }

    void removeTask(std::string id) {
        std::vector<MIST::Task*> copy = {};
        for(auto t : task_queue) {
            if(t->getID() != id) {
                copy.push_back(t);
            }
        }
        task_queue = copy;
    }

    void check_for_tasks() {
        while(this->running) {
            auto rdo = std::make_shared<ReceiveData>();
            bool end = false;
            std::string message = "";
            auto parsed = std::make_shared<ProtobufMIST::Task>();

            while(!end) {
                std::string x = rdo->receive<1>();
                if( (x.find(MIST::Internal::delimiter) != std::string::npos) || x == "-1") {
                    end = true;
                } else {
                    message += x;
                }
            }

            if(parsed->ParseFromString(message)) {
                for(auto task : this->task_queue) {
                    if(parsed->task_name() == task->getID()) {
                        auto t = std::make_shared<std::thread>(&MIST::Task::run, task);
                        t->join();
                        t = nullptr;
                    }
                }
            } else {
                std::cout << "Warning: Could not parse message as task. "
                             "This does not necessarily mean something has gone wrong.\n";
            }
            rdo = nullptr;
            parsed = nullptr;
        }
    }

    void Start() {
        this->running = true;
        this->checker = new std::thread(&Scheduler::check_for_tasks, this);
    }

    void Stop() {
        this->running = false;
        delete checker;
    }
};
