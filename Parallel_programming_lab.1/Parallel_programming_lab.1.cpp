#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <cstdlib>
#include <sstream>
#include <cstdio>

using namespace std;

// Отключение проверки времени выполнения
#pragma runtime_checks("", off)

// Функция для генерации хэша для строки
size_t generateHash(const string& str) {
    size_t hash = 0;
    for (char ch : str) {
        hash = hash * 31 + ch; // Временная сложность: O(M), где M - длина строки; Затраты памяти: O(1)
    }
    return hash;
}

// Функция для создания и заполнения хэш-таблицы
void createAndPopulateHashTable(unordered_map<string, vector<string>>& fragmentMap, const vector<string>& filenames, const string& fragment) {
    for (const string& filename : filenames) {
        ifstream file(filename); // Открытие файла для чтения
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                size_t pos = line.find(fragment);
                while (pos != string::npos) {
                    fragmentMap[line.substr(pos, fragment.size())].push_back(filename); // Добавление в хэш-таблицу
                    pos = line.find(fragment, pos + 1);
                }
            }
            file.close(); // Закрытие файла
        }
        else {
            cerr << "Не удалось открыть файл: " << filename << endl;
        }
    }
}

// Функция для поиска фрагмента в хэш-таблице
void searchFragmentInHashTable(const unordered_map<string, vector<string>>& fragmentMap, const string& fragment, const vector<string>& filenames) {
    auto start = chrono::high_resolution_clock::now(); // Засекаем начало времени выполнения поиска

    if (fragmentMap.find(fragment) != fragmentMap.end()) {
        cout << "Фрагмент найден в следующих файлах:" << endl;
        for (const string& filename : fragmentMap.at(fragment)) {
            cout << filename << endl;
        }
    }
    else {
        cout << "Фрагмент не найден в указанных файлах." << endl;
    }

    auto end = chrono::high_resolution_clock::now(); // Засекаем конец времени выполнения поиска
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    double seconds = duration.count() / 1000.0; // Переводим миллисекунды в секунды с дробной частью
    cout << "Время затраченное на поиск: " << seconds << " секунд" << endl;
}

// Функция для получения информации о затратах памяти
string getMemoryUsage() {
    FILE* file = _popen("ps -o rss= -p $$", "r"); // Выполнение команды ps для получения затрат памяти текущего процесса
    if (!file) return "Ошибка при выполнении команды ps.";

    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        result += buffer;
    }
    _pclose(file);

    stringstream ss(result);
    int memory;
    ss >> memory;
    if (memory == 0) return "Не удалось получить информацию о затратах памяти.";

    return to_string(memory / -1024) + " КБ"; // Конвертация затрат памяти из КБ в МБ
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<string> filenames = { "source1.txt", "source2.txt", "source3.txt", "source4.txt", "source5.txt", "source6.txt", "source7.txt", "source8.txt", "source9.txt", "source10.txt" }; // Список имен файлов с исходными текстами
    //vector<string> filenames = { "source1.txt"}; // Список имен файлов с исходными текстами
    string fragment; // Фрагмент, который нужно найти

    // Ввод фрагмента для поиска
    cout << "Введите фрагмент для поиска: ";
    getline(cin, fragment);

    unordered_map<string, vector<string>> fragmentMap; // Хэш-таблица для хранения фрагментов и имен файлов, содержащих каждый фрагмент

    // Создание и заполнение хэш-таблицы
    createAndPopulateHashTable(fragmentMap, filenames, fragment);

    // Поиск фрагмента в файлах с использованием хэш-таблицы
    searchFragmentInHashTable(fragmentMap, fragment, filenames);

    // Получение и вывод информации о затратах памяти
    cout << "Затраты памяти на создание хэш-таблицы: " << getMemoryUsage() << endl;

    return 0;
}
