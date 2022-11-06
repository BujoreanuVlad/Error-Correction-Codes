#include <iostream>
#include <string>

//The final message, I chose the unsigned long long data type to have 64 bits * 100 since it's a vector
//Thus, instead of using something like a bool vector to represent the bits which would waste 7 bits for every represented bit
//this method makes use of all the necessary bits.
unsigned long long encoded_msg[100] {};
//Algorithm is the representation of the polynomial used, without the trailing 1 because that is already assumed, so we save 1 bit in this representation
unsigned long long algorithms[] {0x1, 0x5, 0x9, 0x12, 0x37, 0x44, 0xD3, 0x319, 0x5C2, 0xC07, 0x1E7A, 0x62CC, 0x8810, 0xAEB6E5, 0x30185CE3, 0x82608EDB, 0x800000000000000D};
//Redundancy levels are the grade of each polynomial from "algorithms"
unsigned int redundancy_levels[] {1, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 15, 16, 24, 30, 32, 64};

void CRC_encode(size_t algorithm_index, std::string &msg) {

    unsigned long algorithm = algorithms[algorithm_index-1];
    unsigned int redundancy_level = redundancy_levels[algorithm_index];

    size_t index_in_vector {};

    //Generate the initial message to which we append "redundancy_level" number of bits
    for (size_t i {}; i < msg.length(); i++) {

        //We extract each character from our string     
        char ch = msg[i];
        //And convert it to its ASCII representation
        unsigned int tmp = static_cast<unsigned int> (ch);

        //Our vector "encoded_msg" looks something like this:
        //0x00 0x00 ... 0x00
        //If we want to add bits to it, then we need to offset them to the left by i*8 since each char has 8 bits + redundancy level
        index_in_vector = 100-1-(8*i+redundancy_level)/64;
        size_t index_in_number {(8*i+redundancy_level)%64};

        encoded_msg[index_in_vector] |= (tmp << (index_in_number * 8));
    }

}

int main() {

    std::string msg;
    size_t n {};

    std::cout << "Please enter the message you wish to send\n";
    std::cin >> msg;

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