g++ main.c "././src/*.c" -Wall -shared -o bin/x86_64/libfmb.dll -I "./include/" -O2
g++ main.c "././src/*.c" -Wall -fPIC -shared -o bin/Linux/libfmb.so -I "./include/" -O2