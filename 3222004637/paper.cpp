#include <iostream>  
#include <fstream>  
#include <string>  
#include <algorithm>  
#include <vector>  
#include <locale>  
#include <codecvt>  
#include <regex>    

// 使用UTF-8编码来处理中文  
std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

// 使用中文逗号和句号分词函数  
std::vector<std::wstring> tokenize_zh(const std::wstring& text) {
	std::vector<std::wstring> tokens;
	std::wstring token;

	for (wchar_t ch : text) {
		if (ch == L'，' || ch == L'。') {
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


// 移除标点符号  
std::wstring removePunctuation_zh(const std::wstring& input) {
	std::wstring result;
	for (wchar_t c : input) {
		if (!std::iswpunct(c)) {
			result.push_back(c);
		}
	}
	return result;
}

// 移除停用词  
std::vector<std::wstring> removeStopWords_zh(const std::vector<std::wstring>& tokens, const std::vector<std::wstring>& stopWords) {
	std::vector<std::wstring> result;
	for (const std::wstring& token : tokens) {
		if (std::find(stopWords.begin(), stopWords.end(), token) == stopWords.end()) {
			result.push_back(token);
		}
	}
	return result;
}

// 将预处理步骤应用于中文文本  
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

// 计算相似度函数
double calculateSimilarity(const std::wstring& str1, const std::wstring& str2) {
	// 使用简单的字符匹配来计算相似度  
	int commonChars = 0;
	int totalChars = std::max(str1.length(), str2.length());

	for (size_t i = 0; i < str1.length(); i++) {
		for (size_t j = 0; j < str2.length(); j++) {
			if (str1[i] == str2[j]) {
				commonChars++;
				break;  // 找到一个匹配后跳出内层循环  
			}
		}
	}

	return static_cast<double>(commonChars) / totalChars;
}


int main() {
	std::string fileName1, fileName2;
	std::cout << "请输入第一个论文文件的名称: ";
	std::cin >> fileName1;
	std::cout << "请输入第二个论文文件的名称: ";
	std::cin >> fileName2;

	std::ifstream file1(fileName1);
	std::ifstream file2(fileName2);

	if (!file1.is_open() || !file2.is_open()) {
		std::cerr << "无法打开文件！" << std::endl;
		return 1;
	}

	std::string content1((std::istreambuf_iterator<char>(file1)), std::istreambuf_iterator<char>());
	std::string content2((std::istreambuf_iterator<char>(file2)), std::istreambuf_iterator<char>());

	file1.close();
	file2.close();

	std::wstring wcontent1 = converter.from_bytes(content1);
	std::wstring wcontent2 = converter.from_bytes(content2);

	// 中文停用词列表  
	std::vector<std::wstring> stopWords = { L"的", L"和", L"是", L"了", L"在", L"不" };
	std::wstring preprocessedContent1 = preprocessText_zh(content1, stopWords);
	std::wstring preprocessedContent2 = preprocessText_zh(content2, stopWords);

	double similarity = calculateSimilarity(content1, content2);
	std::cout << "相似度: " << similarity << std::endl;

	return 0;
}