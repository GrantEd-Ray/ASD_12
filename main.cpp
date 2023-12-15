#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

const int MAX_MEMORY_SIZE = 1000; // Максимальный размер доступной оперативной памяти
const std::string TEMP_FILE_PREFIX = "tempfile_";

// Функция для разбиения входного файла на отсортированные файлы меньшего размера
void phaseOne(const std::string& inputFile) {
    std::ifstream input(inputFile);
    if (!input) {
        std::cerr << "Unable to open input file." << std::endl;
        return;
    }

    std::vector<std::string> buffer(MAX_MEMORY_SIZE);

    int fileCount = 0;
    while (!input.eof()) {
        for (int i = 0; i < MAX_MEMORY_SIZE && !input.eof(); ++i) {
            input >> buffer[i];
        }

        std::sort(buffer.begin(), buffer.begin() + buffer.size());

        std::stringstream tempFileName;
        tempFileName << TEMP_FILE_PREFIX << fileCount++ << ".txt";
        std::ofstream tempFile(tempFileName.str());
        for (int i = 0; i < MAX_MEMORY_SIZE && !input.eof(); ++i) {
            tempFile << buffer[i] << "\n";
        }
    }

    input.close();
}

// Функция для слияния отсортированных файлов
void mergeFiles(const std::vector<std::string>& files, const std::string& outputFile) {
    std::vector<std::ifstream> inputs;
    for (const auto& file : files) {
        inputs.emplace_back(file);
    }

    std::vector<std::string> values(inputs.size());
    std::vector<bool> isFileEmpty(inputs.size(), false);

    std::ofstream output(outputFile);

    while (true) {
        bool allEmpty = true;
        for (size_t i = 0; i < inputs.size(); ++i) {
            if (!isFileEmpty[i]) {
                allEmpty = false;
                if (!(inputs[i] >> values[i])) {
                    isFileEmpty[i] = true;
                    continue;
                }
            }
        }

        if (allEmpty) break;

        auto minIt = std::min_element(values.begin(), values.end());
        size_t minIndex = std::distance(values.begin(), minIt);

        output << *minIt << "\n";

        if (!(inputs[minIndex] >> values[minIndex])) {
            isFileEmpty[minIndex] = true;
        }
    }

    output.close();

    for (auto& input : inputs) {
        input.close();
    }
}

// Основная функция многофазной сортировки
void externalSort(const std::string& inputFile, const std::string& outputFile) {
    phaseOne(inputFile);

    std::vector<std::string> tempFiles;
    std::ifstream input(inputFile);
    int fileSize = 0;
    while (!input.eof()) {
        std::stringstream tempFileName;
        tempFileName << TEMP_FILE_PREFIX << fileSize++ << ".txt";
        tempFiles.push_back(tempFileName.str());
        for (int i = 0; i < MAX_MEMORY_SIZE && !input.eof(); ++i) {
            std::string temp;
            input >> temp;
        }
    }
    input.close();

    mergeFiles(tempFiles, outputFile);

    for (const auto& file : tempFiles) {
        remove(file.c_str());
    }
}

int main() {
    std::string inputFileName = "input.txt"; // Название входного файла
    std::string outputFileName = "output.txt"; // Название выходного файла

    externalSort(inputFileName, outputFileName);

    std::cout << "External sort complete." << std::endl;

    return 0;
}
