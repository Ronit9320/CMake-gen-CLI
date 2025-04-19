#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

std::string readFromFile(const std::string &filePath);
std::string user_input(std::string question);

void cmake_gen() {
  std::string cmake_version =
      user_input("Enter Cmake version[Enter for the default version]: ");
  std::string project_name = user_input("Enter project name: ");
  std::string standard = user_input("C++ standard? [11/14/17/20/23]: ");
  std::string src = user_input("Enter source files (separated by space): ");

  std::string cmake_template = readFromFile("../template/cmake_template.txt");

  std::map<std::string, std::string> replacements;

  replacements["{{VERSION}}"] = cmake_version;
  replacements["{{PROJECT_NAME}}"] = project_name;
  replacements["{{CXX_STANDARD}}"] = standard;
  replacements["{{SOURCE_FILES}}"] = src;

  std::ofstream file("CMakeLists.txt");
  if (file.is_open()) {
    file << cmake_template;
    file.close();
  } else {
    std::cout << "Failed to open file for writing" << std::endl;
  }
}

std::string readFromFile(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    std::cout << "Failed to open file" << std::endl;
    return "";
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

std::string user_input(std::string question) {
  std::string answer;
  std::cout << question << std::endl;
  std::cin >> answer;
  return answer;
}
