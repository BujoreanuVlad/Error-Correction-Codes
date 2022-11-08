#include <iostream>
#include <bitset>
#include <string>
//#define DEBUG

using ull = unsigned long long;

//The final message, I chose the unsigned long long data type to have 64 bits * 100 since it's a vector
//Thus, instead of using something like a bool vector to represent the bits which would waste 7 bits for every represented bit
//this method makes use of all the necessary bits.
ull encoded_msg[100] {};
ull encoded_msg_copy[100] {};
//Algorithm is the representation of the polynomial used, without the trailing 1 because that is already assumed, so we save 1 bit in this representation
ull algorithms[] {0x1, 0x5, 0x9, 0x12, 0x37, 0x44, 0xD3, 0x319, 0x5C2, 0xC07, 0x1E7A, 0x62CC, 0x8810, 0xAEB6E5, 0x30185CE3, 0x82608EDB, 0x800000000000000D};
//Redundancy levels are the grade of each polynomial from "algorithms"
const unsigned int redundancy_levels[] {1, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 15, 16, 24, 30, 32, 64};

ull generateMask(unsigned int redundancy_level) {

	ull mask {1};
	for (size_t i {1}; i < redundancy_level; i++) {
		mask <<= 1;
		mask++;
	}

	return mask;
}

void CRC_encode(size_t algorithm_index, std::string &msg) {

    ull algorithm = algorithms[algorithm_index-1];
    const unsigned int redundancy_level {redundancy_levels[algorithm_index-1]};
	ull mask {generateMask(redundancy_level)};
	ull seeker_mask {1ULL << 63};

    size_t index_in_vector {};
	size_t index_in_number {};

    //Generate the initial message to which we append "redundancy_level" number of bits
    for (size_t i {}; i < msg.length(); i++) {

        //We extract each character from our string     
        char ch = msg[i];
        //And convert it to its ASCII representation
        ull tmp = static_cast<ull> (ch);

        //Our vector "encoded_msg" looks something like this:
        //0x00 0x00 ... 0x00
        //If we want to add bits to it, then we need to offset them to the left by i*8 since each char has 8 bits 
        index_in_vector = 100-1-i/8;
        index_in_number = 7 - i%8;

        encoded_msg_copy[index_in_vector] = encoded_msg[index_in_vector] |= (tmp << (index_in_number * 8));
    }

	size_t remainder {};

	for (size_t i {99}; i >= index_in_vector; i--) {

		size_t index {};
		seeker_mask = 1ULL << 63;

		while (index < 64) {


			while ((seeker_mask & encoded_msg[i]) == 0 && index <= 64) {

				seeker_mask = (seeker_mask >> 1);
				index++;
				if (i == index_in_vector && index == (8-index_in_number)*8) {
					index = 1000;
					break;
				}
			}


			if (index > 64)
				break;

			#ifdef DEBUG
				std::cout << std::bitset<64>(encoded_msg[i]) << "\n";
			#endif
			
			if (index + redundancy_level < 64) {
				encoded_msg[i] ^= (algorithm << (64-redundancy_level-index));
				//This is the +1 coefficient we don't account for in the algorithms[] vector
				encoded_msg[i] ^= (1ULL << (64-redundancy_level-index-1));
				#ifdef DEBUG
					std::cout << std::bitset<64>(encoded_msg[i]) << "\n";
				#endif
			}

			else {
				
				remainder = redundancy_level+index - 64;
				encoded_msg[i] ^= (algorithm >> (remainder));
				ull leftover_algorithm {algorithm & (mask >> (redundancy_level - remainder))};
				#ifdef DEBUG
					std::cout << "Message i:   " << std::bitset<64>(encoded_msg[i]) << "\n";
				#endif
			
				encoded_msg[i-1] ^= (leftover_algorithm << (64 - remainder));
				//This is the +1 coefficient we don't account for in the algorithms[] vector
				encoded_msg[i-1] ^= (1ULL << (64-remainder-1));
				#ifdef DEBUG
					std::cout << "Message i+1: " << std::bitset<64>(encoded_msg[i-1]) << "\n";
				#endif
			}
		}
	}

	ull crc_code {};

	if (index_in_number) {
	
		if (redundancy_level < index_in_number * 8) {
			crc_code = (encoded_msg[index_in_vector] >> (index_in_number*8 - redundancy_level)) & mask;
		}
		else {
			ull crc_part1 {encoded_msg[index_in_vector] & (mask >> (redundancy_level - index_in_number * 8))};
			ull crc_part2 {(encoded_msg[index_in_vector-1] >> (63 - redundancy_level + index_in_number * 8)) & mask};
			crc_code = crc_part2 | (crc_part1 << (index_in_number * 8));
		}
	}

	else {
		crc_code = (encoded_msg[index_in_vector-1] >> (64 - redundancy_level)) & mask;
	}


	for (size_t i {99}; i > index_in_vector; i--)
		std::cout << std::bitset<64>(encoded_msg_copy[i]);
	switch(index_in_number) {
		case 0: std::cout << std::bitset<64>(encoded_msg_copy[index_in_vector] >> (index_in_number * 8));break;
		case 1: std::cout << std::bitset<64-8>(encoded_msg_copy[index_in_vector]>> (index_in_number * 8));break;
		case 2: std::cout << std::bitset<64-2*8>(encoded_msg_copy[index_in_vector]>> (index_in_number * 8));break;
		case 3: std::cout << std::bitset<64-3*8>(encoded_msg_copy[index_in_vector]>> (index_in_number * 8));break;
		case 4: std::cout << std::bitset<64-4*8>(encoded_msg_copy[index_in_vector]>> (index_in_number * 8));break;
		case 5: std::cout << std::bitset<64-5*8>(encoded_msg_copy[index_in_vector]>> (index_in_number * 8));break;
		case 6: std::cout << std::bitset<64-6*8>(encoded_msg_copy[index_in_vector]>> (index_in_number * 8));break;
		case 7: std::cout << std::bitset<64-7*8>(encoded_msg_copy[index_in_vector]>> (index_in_number * 8));break;
	};

	switch(algorithm_index) {
		case 1: std::cout << std::bitset<1>(crc_code); break;
		case 2: std::cout << std::bitset<3>(crc_code); break;
		case 3: std::cout << std::bitset<4>(crc_code); break;
		case 4: std::cout << std::bitset<5>(crc_code); break;
		case 5: std::cout << std::bitset<6>(crc_code); break;
		case 6: std::cout << std::bitset<7>(crc_code); break;
		case 7: std::cout << std::bitset<8>(crc_code); break;
		case 8: std::cout << std::bitset<10>(crc_code); break;
		case 9: std::cout << std::bitset<11>(crc_code); break;
		case 10: std::cout << std::bitset<12>(crc_code); break;
		case 11: std::cout << std::bitset<13>(crc_code); break;
		case 12: std::cout << std::bitset<15>(crc_code); break;
		case 13: std::cout << std::bitset<16>(crc_code); break;
		case 14: std::cout << std::bitset<24>(crc_code); break;
		case 15: std::cout << std::bitset<30>(crc_code); break;
		case 16: std::cout << std::bitset<32>(crc_code); break;
		case 17: std::cout << std::bitset<64>(crc_code); break;
	}
}

int main() {

    std::string msg {};
    size_t n {};

    std::cout << "Please enter the message you wish to send\n";
	std::getline(std::cin, msg);

    std::cout << "Please pick one of the following algorithms:\n";
    std::cout << "1.  CRC-1\n";
    std::cout << "2.  CRC-3-GSM\n";
    std::cout << "3.  CRC-4-ITU\n";
    std::cout << "4.  CRC-5-USB\n";
    std::cout << "5.  CRC-6-GSM\n";
    std::cout << "6.  CRC-7\n";
    std::cout << "7.  CRC-8-Bluetooth\n";
    std::cout << "8.  CRC-10\n";
    std::cout << "9.  CRC-11\n";
    std::cout << "10. CRC-12\n";
    std::cout << "11. CRC-13-BBC\n";
    std::cout << "12. CRC-15-CAN\n";
    std::cout << "13. CRC-16-CCITT\n";
    std::cout << "14. CRC-24\n";
    std::cout << "15. CRC-30\n";
    std::cout << "16. CRC-32\n";
    std::cout << "17. CRC-64-ISO\n";
    std::cout << "(Enter the desired number)\n";

    std::cin >> n;

    CRC_encode(n, msg);

    return 0;
}
