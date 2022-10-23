#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "version.h"

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string& str, char d)
{
	std::vector<std::string> r;

	std::string::size_type start = 0;
	std::string::size_type stop = str.find_first_of(d);
	while (stop != std::string::npos)
	{
		r.push_back(str.substr(start, stop - start));

		start = stop + 1;
		stop = str.find_first_of(d, start);
	}

	r.push_back(str.substr(start));

	return r;
}

inline constexpr std::string_view separator = ".";

class IPAddress final {
public:
	IPAddress() = delete;
	IPAddress(const std::string& ip_string) {
		get_bytes_from_string(ip_string);
	}
	~IPAddress() = default;

	bool operator <(const IPAddress& other) const {
		if (this->firstByte < other.firstByte) {
			return false;
		}
		if (this->firstByte > other.firstByte) {
			return true;
		}
		// 2. 
		if (this->secondByte < other.secondByte) {
			return false;
		}
		if (this->secondByte > other.secondByte) {
			return true;
		}
		// 3.
		if (this->thirdByte < other.thirdByte) {
			return false;
		}
		if (this->thirdByte > other.thirdByte) {
			return true;
		}
		// 4.
		if (this->fourthByte < other.fourthByte) {
			return false;
		}
		if (this->fourthByte > other.fourthByte) {
			return true;
		}

		return false;
	}

	unsigned int get_firstByte() const {
		return firstByte;
	}

	unsigned int get_secondByte() const {
		return secondByte;
	}

	unsigned int get_thirdByte() const {
		return thirdByte;
	}

	unsigned int get_fourthByte() const {
		return fourthByte;
	}

	std::string get_IPAddress() const {
		return  std::to_string(firstByte) + "." + 
				std::to_string(secondByte) + "." + 
				std::to_string(thirdByte) + "." +
				std::to_string(fourthByte);
	}

private:
	unsigned int firstByte{ 0 };
	unsigned int secondByte{ 0 };
	unsigned int thirdByte{ 0 };
	unsigned int fourthByte{ 0 };

	void get_bytes_from_string(const std::string& ip_string) {
		std::vector<int> byteVector;
		get_bytes(ip_string, byteVector);

		for (size_t i{ 0 }; i < byteVector.size(); ++i) {
			if (i == 0) {
				firstByte = byteVector[i];
			}
			else if (i == 1) {
				secondByte = byteVector[i];
			}
			else if (i == 2) {
				thirdByte = byteVector[i];
			}
			else if (i == 3) {
				fourthByte = byteVector[i];
			}
		}
	}

	void get_bytes(const std::string& ip_string, std::vector<int>& byteVector) {
		size_t position{ ip_string.find_first_of(separator) };
		byteVector.push_back(std::stoi(ip_string.substr(0, position)));
		if (position == std::string::npos) {
			return;
		}

		std::string editIPString = ip_string.substr(position + 1, ip_string.size());
		get_bytes(editIPString, byteVector);
	}
};


int main(int, char*[]) {
	try {
		std::vector<IPAddress> ip_pool;
		//ip_pool.emplace_back("2.1.2.1");
		//ip_pool.emplace_back("1.1.1.1");
		//ip_pool.emplace_back("1.2.1.1");
		for (std::string line; std::getline(std::cin, line);) {
			// �������� ���� �� ������. ���� ��������� �������� ���������.
			std::vector<std::string> v = split(line, '\t');
			// ��������� ������ ���� (���� � IP ��������) � ������.
			ip_pool.emplace_back(v.at(0));
		}

		// reverse lexicographically sort
		// ��������� IP ������ � ���������. � ��������� ������������ ���������������� ������� ���������. 
		std::multiset<IPAddress> ip_pool_reverseSorted;
		for (const auto& ip : ip_pool) {
			ip_pool_reverseSorted.insert(ip);
		}

		for (auto& ip : ip_pool_reverseSorted) {
			std::cout << ip.get_IPAddress() << std::endl;
		}

		// 222.173.235.246
		// 222.130.177.64
		// 222.82.198.61
		// ...
		// 1.70.44.170
		// 1.29.168.152
		// 1.1.234.8

		// TODO filter by first byte and output
		// std::cout << "===========================" << std::endl;
		std::vector<IPAddress> ip_pool_firstByteSorted;
		unsigned int firstByte = 1;
		std::copy_if(ip_pool_reverseSorted.begin(), ip_pool_reverseSorted.end(), std::back_inserter(ip_pool_firstByteSorted),
			[firstByte](const IPAddress& ip) { return ip.get_firstByte() == firstByte; });

		for (auto& ip : ip_pool_firstByteSorted) {
			std::cout << ip.get_IPAddress() << std::endl;
		}
		// 1.231.69.33
		// 1.87.203.225
		// 1.70.44.170
		// 1.29.168.152
		// 1.1.234.8

		// TODO filter by first and second bytes and output
		// std::cout << "***********************************" << std::endl;
		ip_pool_firstByteSorted.clear();
		firstByte = 46;
		unsigned int secondByte = 70;
		std::copy_if(ip_pool_reverseSorted.begin(), ip_pool_reverseSorted.end(), std::back_inserter(ip_pool_firstByteSorted),
			[firstByte, secondByte](const IPAddress& ip) {
				return (ip.get_firstByte() == firstByte) && (ip.get_secondByte() == secondByte); 
				});

		for (auto& ip : ip_pool_firstByteSorted) {
			std::cout << ip.get_IPAddress() << std::endl;
		}
		// 46.70.225.39
		// 46.70.147.26
		// 46.70.113.73
		// 46.70.29.76

		// TODO filter by any byte and output
		// std::cout << "---------------------------------------------" << std::endl;
		ip_pool_firstByteSorted.clear();
		unsigned int anyByte = 46;
		std::copy_if(ip_pool_reverseSorted.begin(), ip_pool_reverseSorted.end(), std::back_inserter(ip_pool_firstByteSorted),
			[anyByte](const IPAddress& ip) { 
				return (ip.get_firstByte() == anyByte) || (ip.get_secondByte() == anyByte) ||
				 (ip.get_thirdByte() == anyByte) || (ip.get_fourthByte() == anyByte);
			});
		for (auto& ip : ip_pool_firstByteSorted) {
			std::cout << ip.get_IPAddress() << std::endl;
		}
		// 186.204.34.46
		// 186.46.222.194
		// 185.46.87.231
		// 185.46.86.132
		// 185.46.86.131
		// 185.46.86.131
		// 185.46.86.22
		// 185.46.85.204
		// 185.46.85.78
		// 68.46.218.208
		// 46.251.197.23
		// 46.223.254.56
		// 46.223.254.56
		// 46.182.19.219
		// 46.161.63.66
		// 46.161.61.51
		// 46.161.60.92
		// 46.161.60.35
		// 46.161.58.202
		// 46.161.56.241
		// 46.161.56.203
		// 46.161.56.174
		// 46.161.56.106
		// 46.161.56.106
		// 46.101.163.119
		// 46.101.127.145
		// 46.70.225.39
		// 46.70.147.26
		// 46.70.113.73
		// 46.70.29.76
		// 46.55.46.98
		// 46.49.43.85
		// 39.46.86.85
		// 5.189.203.46
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
