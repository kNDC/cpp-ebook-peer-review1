#define SUBMISSION
#undef SUBMISSION

// IO, strings
#include <iostream>
#include <iomanip>

#if !defined(SUBMISSION)
#include <fstream>
#include <sstream>
#include <conio.h>
#endif

#include <string>

// Containers
#include <vector>
#include <map>
#include<algorithm>

class ReadingStats
{
private:
    enum class QueryType
    {
        READ = 0,
        CHEER,
        UNSUPPORTED
    };

    std::map<int, size_t> reading_data_;
    std::vector<size_t> pages_by_readers_;
    std::vector<size_t> page_counts_;
    size_t readers_total_;

    // Классификация запроса
    static QueryType ClassifyQuery(const std::string&);

    // Обработка запроса READ
    void ProcessRead(size_t, size_t);

    // Обработка запроса CHEER
    void ProcessCheer(size_t, std::ostream&) const;
    
public:
    void ProcessQuery(std::istream&, std::ostream&);
};

ReadingStats::QueryType ReadingStats::ClassifyQuery(const std::string& query_type)
{
    if (query_type == "READ") return QueryType::READ;
    if (query_type == "CHEER") return QueryType::CHEER;
    return QueryType::UNSUPPORTED;
}

void ReadingStats::ProcessRead(size_t id, size_t new_page)
{
    // Страницы по читателям
    if (id >= this->pages_by_readers_.size())
    {
        this->pages_by_readers_.resize(id + 1);
    }

    // Если раньше читателя не было, 
    // наращиваем общее количество
    if (!this->pages_by_readers_[id])
    {
        ++this->readers_total_;
    }
    
    size_t old_page = this->pages_by_readers_[id];
    this->pages_by_readers_[id] = new_page;

    // Количество прочтений по страницам
    if (new_page >= this->page_counts_.size())
    {
        this->page_counts_.resize(new_page + 1);
    }
    
    for (size_t i = old_page + 1; i <= new_page; ++i)
    {
        ++this->page_counts_[i];
    }
}

void ReadingStats::ProcessCheer(size_t id, std::ostream& os) const
{
    double share = 0;
    
    // Если Id больше размера pages_by_readers_,
    // то читатель в базе не значится и оставляем ноль. 
    if (id < this->pages_by_readers_.size())
    {
        // Если единственный читатель, то приравниваем
        // долю единице.
        if (this->readers_total_ == 1)
        {
            share = 1;
        }
        // Если количество прочитанных страниц нулевое, 
        // ничего не делаем.
        else if (this->pages_by_readers_[id])
        {
            share = this->readers_total_;
            share -= this->page_counts_[this->pages_by_readers_[id]];
            share /= this->readers_total_ - 1;
        }
    }
    
    os << std::setprecision(6) 
       << share << '\n';
}

void ReadingStats::ProcessQuery(std::istream& is, std::ostream& os)
{
    std::string query_type;
    is >> query_type;

    switch (ClassifyQuery(query_type))
    {
    case QueryType::READ:
    {
        int id;
        size_t page;
        is >> id >> page;
        is.get();

        this->ProcessRead(id, page);
        break;
    }
    
    case QueryType::CHEER:
    {
        int id;
        is >> id;
        is.get();

        this->ProcessCheer(id, os);
        break;
    }

    default:
        break;
    }
}

int main()
{
#if !defined(SUBMISSION)
    std::ifstream in("X:\\Work\\Programming\\Yandex Practicum\\Sprint 12_2\\code 1_5 4_9\\test_input.txt");
    std::streambuf* cinbuf = std::cin.rdbuf(); // сохранение старого буфера потока
    std::cin.rdbuf(in.rdbuf());

    std::ofstream out("X:\\Work\\Programming\\Yandex Practicum\\Sprint 12_2\\code 1_5 4_9\\test_output.txt");
    std::streambuf* coutbuf = std::cout.rdbuf(); // сохранение старого буфера потока
    std::cout.rdbuf(out.rdbuf());
#endif

    size_t n;

    std::cin >> n;
    std::cin.get(); // Съедаем перенос строки

    ReadingStats reading_stats;

    while (n--)
    {
        reading_stats.ProcessQuery(std::cin, std::cout);
    }

#if !defined(SUBMISSION)
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);

    std::cout << "Press any key to exit...";
    _getch();
#endif
}