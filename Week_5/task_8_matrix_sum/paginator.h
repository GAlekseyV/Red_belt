//
// Created by GAlekseyV on 02.10.2018.
//

#ifndef RED_BELT_PAGINATOR_H
#define RED_BELT_PAGINATOR_H
#include <vector>

using namespace std;

template <typename Iterator>
class IteratorRange {
private:
    Iterator first, last;
    size_t size_;
public:
    IteratorRange(Iterator begin, Iterator end)
            : first(begin)
            , last(end)
            , size_(distance(first, last)){
    }
    Iterator begin()const{
        return first;
    }
    Iterator end()const{
        return last;
    }
    size_t size() const{
        return size_;
    }
};

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        for(size_t left = distance(begin, end); left > 0;){
            size_t current_page_size = min(page_size, left);
            Iterator current_page_end = next(begin, current_page_size);
            pages.push_back({begin, current_page_end});

            left -= current_page_size;
            begin = current_page_end;
        }
    }

    size_t size()const{
        // Возвращает количество страниц, на которые был разбит переданный контейнер
        return pages.size();
    }

    auto begin() const{
        return pages.begin();
    }

    auto end() const{
        return pages.end();
    }

private:
    //Iterator first, last;
    vector<IteratorRange<Iterator>> pages;
    //const size_t page_size;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
    // Реализуйте этот шаблон функции
    return Paginator(c.begin(), c.end(), page_size);
}
#endif //RED_BELT_PAGINATOR_H
