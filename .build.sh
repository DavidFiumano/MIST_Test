cmd="g++ HashFile.cpp -std=c++1y -lpthread -lprotobuf -Iinclude/ -Iprotobuf_files/ -Iasio-$1/include/ -o HashFile.o"
eval $cmd
printf "Exited with code $?\n"
