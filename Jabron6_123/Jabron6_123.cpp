#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <locale>
#include <codecvt>

class Book {
public:
    Book(const std::wstring& title, const std::wstring& author, const std::wstring& genre, int publicationYear, const std::wstring& description)
        : m_title(title), m_author(author), m_genre(genre), m_publicationYear(publicationYear), m_description(description) {}

    std::wstring title() const { return m_title; }
    std::wstring author() const { return m_author; }
    std::wstring genre() const { return m_genre; }
    int publicationYear() const { return m_publicationYear; }
    std::wstring description() const { return m_description; }

private:
    std::wstring m_title;
    std::wstring m_author;
    std::wstring m_genre;
    int m_publicationYear;
    std::wstring m_description;
};

class Library {
public:
    void addBook(const Book& book) {
        m_books.push_back(book);
    }

    std::vector<Book> searchByTitle(const std::wstring& title) const {
        std::vector<Book> results;
        std::copy_if(m_books.begin(), m_books.end(), std::back_inserter(results),
            [&title](const Book& book) {
                return book.title().find(title) != std::wstring::npos;
            }
        );
        return results;
    }

    std::vector<Book> searchByAuthor(const std::wstring& author) const {
        std::vector<Book> results;
        std::copy_if(m_books.begin(), m_books.end(), std::back_inserter(results),
            [&author](const Book& book) {
                return book.author().find(author) != std::wstring::npos;
            }
        );
        return results;
    }

    std::vector<Book> searchByGenre(const std::wstring& genre) const {
        std::vector<Book> results;
        std::copy_if(m_books.begin(), m_books.end(), std::back_inserter(results),
            [&genre](const Book& book) {
                return book.genre().find(genre) != std::wstring::npos;
            }
        );
        return results;
    }

    bool reserveBook(const std::wstring& title, int userId) {
        auto it = std::find_if(m_books.begin(), m_books.end(),
            [&title](const Book& book) {
                return book.title() == title;
            }
        );
        if (it != m_books.end()) {
            Reservations[title].push_back(userId);
            return true;
        }
        return false;
    }

    std::vector<std::wstring> getReservedBooks(int userId) const {
        std::vector<std::wstring> reservedBooks;
        for (const auto& reservation : Reservations) {
            if (std::find(reservation.second.begin(), reservation.second.end(), userId) != reservation.second.end()) {
                reservedBooks.push_back(reservation.first);
            }
        }
        return reservedBooks;
    }

    void cancelReservation(const std::wstring& title, int userId) {
        auto it = std::find(Reservations[title].begin(), Reservations[title].end(), userId);
        if (it != Reservations[title].end()) {
            Reservations[title].erase(it);
        }
    }

private:
    std::vector<Book> m_books;
    std::map<std::wstring, std::vector<int>> Reservations;
};

int main() {
    setlocale(LC_ALL, "Russian");

    Library library;

    int choice = 0;
    while (choice != 9) {
        std::wstring title, author, genre, description;
        int publicationYear, userId;

        std::wcout << L"Выберите действие:\n";
        std::wcout << L"1. Добавить книгу\n";
        std::wcout << L"2. Поиск книг по автору\n";
        std::wcout << L"3. Поиск книг по названию\n";
        std::wcout << L"4. Поиск книг по жанру\n";
        std::wcout << L"5. Резервировать книгу\n";
        std::wcout << L"6. Посмотреть зарезервированные книги\n";
        std::wcout << L"7. Отменить резервацию\n";
        std::wcout << L"9. Выйти\n";

        std::wcin >> choice;

        switch (choice) {
        case 1:
            std::wcout << L"Введите название книги: ";
            std::wcin.ignore();
            std::getline(std::wcin, title);
            std::wcout << L"Введите автора книги: ";
            std::getline(std::wcin, author);
            std::wcout << L"Введите жанр книги: ";
            std::getline(std::wcin, genre);
            std::wcout << L"Введите год издания книги: ";
            std::wcin >> publicationYear;
            std::wcout << L"Введите описание книги: ";
            std::wcin.ignore();
            std::getline(std::wcin, description);
            library.addBook(Book(title, author, genre, publicationYear, description));
            break;
        case 2:
            std::wcout << L"Введите автора для поиска: ";
            std::wcin.ignore();
            std::getline(std::wcin, author);
            for (const auto& book : library.searchByAuthor(author)) {
                std::wcout << L"Название: " << book.title() << std::endl;
                std::wcout << L"Автор: " << book.author() << std::endl;
                std::wcout << L"Жанр: " << book.genre() << std::endl;
                std::wcout << L"Описание: " << book.description() << std::endl;
                std::wcout << std::endl;
            }
            break;
        case 3:
            std::wcout << L"Введите название для поиска: ";
            std::wcin.ignore();
            std::getline(std::wcin, title);
            for (const auto& book : library.searchByTitle(title)) {
                std::wcout << L"Название: " << book.title() << std::endl;
                std::wcout << L"Автор: " << book.author() << std::endl;
                std::wcout << L"Жанр: " << book.genre() << std::endl;
                std::wcout << L"Описание: " << book.description() << std::endl;
                std::wcout << std::endl;
            }
            break;
        case 4:
            std::wcout << L"Введите жанр для поиска: ";
            std::wcin.ignore();
            std::getline(std::wcin, genre);
            for (const auto& book : library.searchByGenre(genre)) {
                std::wcout << L"Название: " << book.title() << std::endl;
                std::wcout << L"Автор: " << book.author() << std::endl;
                std::wcout << L"Жанр: " << book.genre() << std::endl;
                std::wcout << L"Описание: " << book.description() << std::endl;
                std::wcout << std::endl;
            }
            break;
        case 5:
            std::wcout << L"Введите название книги для резервирования: ";
            std::wcin.ignore();
            std::getline(std::wcin, title);
            std::wcout << L"Введите ID пользователя: ";
            std::wcin >> userId;
            library.reserveBook(title, userId);
            break;
        case 6:
            std::wcout << L"Введите ID пользователя для просмотра зарезервированных книг: ";
            std::wcin >> userId;
            for (const auto& title : library.getReservedBooks(userId)) {
                std::wcout << title << std::endl;
            }
            break;
        case 7:
            std::wcout << L"Введите название книги для отмены резервации: ";
            std::wcin.ignore();
            std::getline(std::wcin, title);
            std::wcout << L"Введите ID пользователя: ";
            std::wcin >> userId;
            library.cancelReservation(title, userId);
            break;
        }
    }

    return 0;
}
