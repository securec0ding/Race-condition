all: race

race: race.cpp
	g++ -g -o race race.cpp --std=c++11 -lcrypto

clean:
	rm -f race encrypted_data.bin