#include <iostream>
#include <string>
#include <cmath>

int main() {

	std::string msg {};

	std::cout << "Please enter a binary message\n";
	std::getline(std::cin, msg);

	//number of bits of information
	unsigned int m {static_cast<unsigned int> (msg.length())};
	//trying to find k, the number of redundant bits
	unsigned int k {1};
	while (std::pow(2, k) < m+k+1) k++;
	
	//n is the total number of bits
	unsigned int n {m+k};

	//msg_bits is the encoded message we want to find out
	unsigned int *msg_bits = new unsigned int[n] {};

	size_t index {};
	//Populate the vector with 1's and 0's in the positions not power of 2
	for (size_t i {}; i < n; i++) {
		//This is the first position, and 1 is a power of 2 so skip
		if (i+1 == 1)
			continue;
		//The position is a power of 2 that is different from 1
		if (((i+1) & i) == 0)
			continue;

		msg_bits[i] = std::stoul(msg.substr(index++, 1));
		if (msg_bits[i] != 0 && msg_bits[i] != 1) {
			std::cout << "Invalid message, the message has to contain ONLY the characters 0 and 1\n";
			return 1;
		}
	}

	//building the hamming matrix
	unsigned int **HammingMatrix = new unsigned int*[k];

	for (size_t i {}; i < k; i++)
		HammingMatrix[i] = new unsigned int[n];

	for (size_t j {}; j < n; j++) {
		size_t tmp {j+1};
		//i+1 because size_t is unsigned and so after i=0 the next one overflows
		for (size_t i {k-1}; i+1 > 0; i--) {
			HammingMatrix[i][j] = tmp % 2;
			tmp /= 2;	
		}
	}

	//calculating the redundant bits
	for (size_t i {}; i < k; i++) {
		for (size_t j {}; j < n; j++) {
			msg_bits[static_cast<size_t>(std::pow(2, k-i-1)-1)] += msg_bits[j] & HammingMatrix[i][j];
		}
		msg_bits[static_cast<size_t>(std::pow(2, k-i-1)-1)] %= 2;
	}

	for (size_t i {}; i < n; i++)
		std::cout << msg_bits[i];

	return 0;
}
