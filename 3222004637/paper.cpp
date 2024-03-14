#include <iostream>  
#include <fstream>  
#include <string>  
#include <algorithm>  
#include <vector>  
#include <locale>  
#include <codecvt>  
#include <regex>    

// ʹ��UTF-8��������������  
std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

// ʹ�����Ķ��ź;�ŷִʺ���  
std::vector<std::wstring> tokenize_zh(const std::wstring& text) {
	std::vector<std::wstring> tokens;
	std::wstring token;

	for (wchar_t ch : text) {
		if (ch == L'��' || ch == L'��') {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
		}
		else {
			token += ch;
		}
	}

	if (!token.empty()) {
		tokens.push_back(token);
	}

	return tokens;
}


// �Ƴ�������  
std::wstring removePunctuation_zh(const std::wstring& input) {
	std::wstring result;
	for (wchar_t c : input) {
		if (!std::iswpunct(c)) {
			result.push_back(c);
		}
	}
	return result;
}

// �Ƴ�ͣ�ô�  
std::vector<std::wstring> removeStopWords_zh(const std::vector<std::wstring>& tokens, const std::vector<std::wstring>& stopWords) {
	std::vector<std::wstring> result;
	for (const std::wstring& token : tokens) {
		if (std::find(stopWords.begin(), stopWords.end(), token) == stopWords.end()) {
			result.push_back(token);
		}
	}
	return result;
}

// ��Ԥ�������Ӧ���������ı�  
std::wstring preprocessText_zh(const std::wstring& input, const std::vector<std::wstring>& stopWords) {
	std::wstring processed = removePunctuation_zh(input);
	std::vector<std::wstring> tokens = tokenize_zh(processed);
	tokens = removeStopWords_zh(tokens, stopWords);

	std::wostringstream stream;
	for (const std::wstring& token : tokens) {
		stream << token << L" ";
	}
	return stream.str();
}

// �������ƶȺ���
double calculateSimilarity(const std::wstring& str1, const std::wstring& str2) {
	// ʹ�ü򵥵��ַ�ƥ�����������ƶ�  
	int commonChars = 0;
	int totalChars = std::max(str1.length(), str2.length());

	for (size_t i = 0; i < str1.length(); i++) {
		for (size_t j = 0; j < str2.length(); j++) {
			if (str1[i] == str2[j]) {
				commonChars++;
				break;  // �ҵ�һ��ƥ��������ڲ�ѭ��  
			}
		}
	}

	return static_cast<double>(commonChars) / totalChars;
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

	std::wstring wcontent1 = converter.from_bytes(content1);
	std::wstring wcontent2 = converter.from_bytes(content2);

	// ����ͣ�ô��б�  
	std::vector<std::wstring> stopWords = { L"��", L"��", L"��", L"��", L"��", L"��" };
	std::wstring preprocessedContent1 = preprocessText_zh(content1, stopWords);
	std::wstring preprocessedContent2 = preprocessText_zh(content2, stopWords);

	double similarity = calculateSimilarity(content1, content2);
	std::cout << "���ƶ�: " << similarity << std::endl;

	return 0;
}