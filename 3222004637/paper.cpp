#include <iostream>  
#include <fstream>  
#include <string>  
#include <algorithm>  

// ���������ַ��������ƶȣ�ʹ�ü򵥵Ĺ����Ӵ����ȣ�  
double calculateSimilarity(const std::string& str1, const std::string& str2) {
	size_t maxLen = 0;
	for (size_t i = 0; i < str1.length(); ++i) {
		for (size_t j = 0; j < str2.length(); ++j) { 
			size_t len = 0; 
			while (i + len < str1.length() && j + len < str2.length() && str1[i + len] == str2[j + len]) {
				++len;
			}
			maxLen = std::max(maxLen, len);
		}
	}
	return static_cast<double>(maxLen) / std::min(str1.length(), str2.length());
}

int main() {
	std::string fileName1, fileName2;
	std::cout << "�������һ�������ļ�������: ";
	std::cin >> fileName1;
	std::cout << "������ڶ��������ļ�������: ";
	std::cin >> fileName2;

	std::ifstream file1(fileName1);
	std::ifstream file2(fileName2);

	if (!file1.is_open() || !file2.is_open()) {
		std::cerr << "�޷����ļ���" << std::endl;
		return 1;
	}

	std::string content1((std::istreambuf_iterator<char>(file1)), std::istreambuf_iterator<char>());
	std::string content2((std::istreambuf_iterator<char>(file2)), std::istreambuf_iterator<char>());

	file1.close();
	file2.close();

	double similarity = calculateSimilarity(content1, content2);
	std::cout << "���ƶ�: " << similarity << std::endl;

	return 0;
}