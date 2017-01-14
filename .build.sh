cmd="g++ HashFile.cpp -std=c++1y -lpthread -lprotobuf -Iinclude/ -Iprotobuf_files/ -Iasio-$1/include/ -c -o HashFile.o"
eval $cmd
link="g++ -std=c++1y HashFile.o protobuf_files/MISTpb.o -lpthread -lprotobuf -o HashFile.out"
eval $link
printf "Exited with code $?\n"
