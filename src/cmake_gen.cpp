#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct CMakeConfig {
  std::string version = "3.10";
  std::string projectName;
  std::string cxxStandard = "17";
  std::vector<std::string> sourceFiles;
  std::string buildType = "Debug";
  std::string outputDirectory = ".";
  std::map<std::string, std::string> compilerFlags;
  std::vector<std::string> dependencies;
};

std::string getTemplatePath();
std::string readFromFile(const std::string &filePath);
std::string getLineInput(const std::string &question,
                         const std::string &defaultValue = "");
std::vector<std::string> getTokenizedInput(const std::string &question);
bool validateCxxStandard(const std::string &standard);
std::string applyTemplateReplacements(
    const std::string &templateStr,
    const std::map<std::string, std::string> &replacements);
bool writeToFile(const std::string &filePath, const std::string &content);

std::string getTemplatePath() {
  std::filesystem::path exePath = std::filesystem::canonical("/proc/self/exe");
  std::filesystem::path templatePath = exePath.parent_path().parent_path() / "share" / "gencmake" / "templates" / "cmake_template.txt";
  return templatePath.string();
}

void cmake_gen() {
  CMakeConfig config;

  std::string templatePath = getTemplatePath();
  if (!std::filesystem::exists(templatePath)) {
    std::cerr << "Error: Template file not found at " << templatePath
              << std::endl;
    return;
  }

  config.version = getLineInput("Enter CMake version", "3.10");
  config.projectName = getLineInput("Enter project name", "MyProject");

  std::string standardInput =
      getLineInput("C++ standard? [11/14/17/20/23]", "17");
  if (!validateCxxStandard(standardInput)) {
    std::cerr << "Invalid C++ standard. Using default (17)" << std::endl;
    standardInput = "17";
  }
  config.cxxStandard = standardInput;

  config.sourceFiles =
      getTokenizedInput("Enter source files (separated by space)");
  config.buildType =
      getLineInput("Build type [Debug/Release/RelWithDebInfo]", "Debug");
  config.outputDirectory = getLineInput("Output directory", ".");

  std::string cmake_template = readFromFile(templatePath);
  if (cmake_template.empty()) {
    std::cerr << "Error: Failed to read template file" << std::endl;
    return;
  }

  std::map<std::string, std::string> replacements;
  replacements["{{VERSION}}"] = config.version;
  replacements["{{PROJECT_NAME}}"] = config.projectName;
  replacements["{{CXX_STANDARD}}"] = config.cxxStandard;

  std::string sourceFilesStr;
  for (size_t i = 0; i < config.sourceFiles.size(); ++i) {
    sourceFilesStr += config.sourceFiles[i];
    if (i < config.sourceFiles.size() - 1) {
      sourceFilesStr += " ";
    }
  }
  replacements["{{SOURCE_FILES}}"] = sourceFilesStr;
  replacements["{{BUILD_TYPE}}"] = config.buildType;

  std::string result = applyTemplateReplacements(cmake_template, replacements);

  std::string outputPath = config.outputDirectory + "/CMakeLists.txt";
  if (writeToFile(outputPath, result)) {
    std::cout << "CMakeLists.txt successfully generated at " << outputPath
              << std::endl;
    std::cout << "\nPreview of generated file:\n"
              << std::string(40, '-') << std::endl;
    std::cout << result.substr(0, std::min(size_t(500), result.size()))
              << std::endl;
    if (result.size() > 500) {
      std::cout << "... (truncated)" << std::endl;
    }
    std::cout << std::string(40, '-') << std::endl;
  } else {
    std::cerr << "Failed to write CMakeLists.txt to " << outputPath
              << std::endl;
  }
}

std::string readFromFile(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    return "";
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

std::string getLineInput(const std::string &question,
                         const std::string &defaultValue) {
  std::string answer;
  std::cout << question;
  if (!defaultValue.empty()) {
    std::cout << " [" << defaultValue << "]";
  }
  std::cout << ": ";

  std::getline(std::cin >> std::ws, answer);

  if (answer.empty() && !defaultValue.empty()) {
    return defaultValue;
  }
  return answer;
}

std::vector<std::string> getTokenizedInput(const std::string &question) {
  std::string input = getLineInput(question);
  std::vector<std::string> tokens;
  std::istringstream iss(input);
  std::string token;

  while (iss >> token) {
    tokens.push_back(token);
  }

  return tokens;
}

bool validateCxxStandard(const std::string &standard) {
  return standard == "11" || standard == "14" || standard == "17" ||
         standard == "20" || standard == "23";
}

std::string applyTemplateReplacements(
    const std::string &templateStr,
    const std::map<std::string, std::string> &replacements) {
  std::string result = templateStr;
  for (const auto &[placeholder, value] : replacements) {
    size_t pos = 0;
    while ((pos = result.find(placeholder, pos)) != std::string::npos) {
      result.replace(pos, placeholder.length(), value);
      pos += value.length();
    }
  }
  return result;
}

bool writeToFile(const std::string &filePath, const std::string &content) {
  std::filesystem::path dirPath = std::filesystem::path(filePath).parent_path();
  if (!dirPath.empty() && !std::filesystem::exists(dirPath)) {
    try {
      std::filesystem::create_directories(dirPath);
    } catch (...) {
      return false;
    }
  }

  std::ofstream file(filePath);
  if (!file.is_open()) {
    return false;
  }

  file << content;
  file.close();
  return true;
}
