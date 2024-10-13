// J.A.R.V.I.S.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>
#include <windows.h>
#include <string>
#include <system_error> // Для std::system_error

namespace fs = std::filesystem;

// Функция для поиска файла/директории на заданном диске
void searchDrive(const fs::path& drivePath, const std::string& targetName, std::ofstream& out) {
	if (fs::exists(drivePath)) {
		out << "Диск " << drivePath << " доступен" << std::endl;

		for (const auto& entry : fs::recursive_directory_iterator(
			drivePath, fs::directory_options::skip_permission_denied)) {
			try {
				if ((entry.is_directory() || entry.is_regular_file()) &&
					entry.path().filename() == targetName) {
					if (entry.is_directory()) {
						out << "Директория найдена: ";
					}
					else {
						out << "Файл найден: ";
					}
					out << entry.path() << std::endl;
				}
			}
			catch (const std::system_error& e) {
				std::cerr << "Ошибка при доступе к " << entry.path()
					<< ": " << e.what() << std::endl;
			}
		}
	}
}

// Функция для поиска файла/директории на всех логических дисках
void searchAllDrives(const std::string& targetName, std::ofstream& out) {
	DWORD driveMask = GetLogicalDrives();
	for (int drive = 0; drive < 26; ++drive) {
		if (driveMask & (1 << drive)) {
			char driveLetter = 'A' + drive;
			fs::path drivePath = std::string(1, driveLetter) + ":\\";
			searchDrive(drivePath, targetName, out);
		}
	}
}

int main() {
	setlocale(LC_ALL, "Russian");

	std::string targetName;
	std::cout << "Введи название директории или файла: " << std::endl;
	std::getline(std::cin, targetName);
	std::cout << "Вы ввели: " << targetName << std::endl;

	std::cout << "Идет поиск, подождите, результаты в файле output.txt\n";
	std::ofstream out("output.txt", std::ios::app);

	if (!out.is_open()) {
		std::cerr << "Ошибка открытия файла output.txt" << std::endl;
		return 1;
	}

	searchAllDrives(targetName, out);

	std::cout << "Поиск завершен. Гудбай от t.me/geiseberg" << std::endl;
	return 0;
}
