#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>

using std::vector;
using std::ostream;
using std::swap;

template <class T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& v) {
    o << "[";
    for(auto& x : v) {  o << x << ", "; }
    return o << "]";
}

template <class TFunc, class TResult, class TParam1>
void test(TResult expect, TFunc f, TParam1 p1) {
    using namespace std;

    auto got = f(p1);
    if(got != expect) {
        cerr << "failed: " << expect << " != " << got << endl;
    }
}

template <class TFunc, class TResult, class TParam1, class TParam2>
void test(TResult expect, TFunc f, TParam1 p1, TParam2 p2) {
    using namespace std;
    auto got = f(p1, p2);
    if(got != expect) {
        cerr << "failed: f(" << p1 << ", " << p2 << ") = "
            << got << " != " << expect << endl;
    }
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
namespace My {

template <class TIter>
TIter min_element2(TIter b, TIter e) {
    auto result = b;
    while(b < e) {
        // [ )  = [processed) [curent] [unprocessed)
        // assert(tmp is min from [processed))
        if(*b < *result) {
            result = b;
        }
        ++b;
        // assert(tmp is min from [processed))
    }
    return result;
}


template <class TIter>
void naive_sort(TIter b, TIter e) {
    for (auto i = b; i < e; ++i) {
        assert(is_sorted(b, i));
        // [sorted) U [unsorted) = [b, i) U [i e)
        
        for (auto j = i+1; j < e; ++j) {
            // [unsorted) = [i] U [i+1, j) U [j e)
            assert(min_element2(i, j) == i);

            if(*j < *i)
                std::swap(*i, *j);

            assert(min_element(i, j) == i);
        }

        assert(is_sorted(b, i+1));
    }
}


template <class TIter>
void selection_sort(TIter b, TIter e) {
    for (auto i = b; i < e; ++i) {
        // [sorted) U [unsorted) = [b, i) U [i e)
        assert(is_sorted(b, i));
        swap(*i , *min_element(i, e));
    }
}

template <class TIter>
void bubble_sort(TIter b, TIter e) {
    if(b == e) { 
        return; 
    }

    TIter sorted_begin = e-1;
    while (b < sorted_begin) {

        // [unsorted) U [sorted) = [b, sb) U [sb e)
        assert(sorted_begin < e);
        assert(is_sorted(sorted_begin, e));

        auto j = b;
        while(j < sorted_begin) {
            // assert(std::reveverse_max_element(b, j+1) == j);
            if(*(j+1) < *j) {
                std::iter_swap(j+1, j);
            }
            ++j;
            // assert(std::reveverse_max_element(b, j+1) == j);
        }

        --sorted_begin;
        assert(is_sorted(sorted_begin, e));
    }
}


template <class TIter>
void insertion_sort(TIter b, TIter e) {

     auto pivot = b+1;
    while(pivot < e) {
        // [b, pivot) [pivot) [pivot+1, e)
        assert(std::is_sorted(b, pivot));

        auto i = pivot;
        while(b < i && *i < *(i-1)) {
            assert(is_sorted(b, i) && is_sorted(i, pivot));
            std::iter_swap(i, i-1);
            --i;
            assert(is_sorted(b, i) && is_sorted(i, pivot));
        }

        ++pivot;
        assert(std::is_sorted(b, pivot));
    }
    
}

template <class TIter>
void merge(TIter b, TIter m, TIter e, TIter buff) {
    const auto size = e-b;
    const auto old = buff;
    const auto e1 = m;
    while(b < e1 && m < e) {
        *buff++ = *b < *m ? *b++ : *m++;
    }
    buff = copy(b, e1, buff);
    buff = copy(m, e,  buff);

    assert(buff-old == size);
}
template <class TIter>
void merge_sort(TIter b, TIter e, TIter buff) {
    auto size = e-b;
    if(size < 1024) {
        std::copy(b, e, buff);
        insertion_sort(buff, buff+size);
    } else {
        auto m = b + size/2;
        merge_sort(b, m, buff);
        merge_sort(m, e, buff+(size)/2);
        std::copy(buff, buff+size, b);
        merge(b, m, e, buff);
    }
}

template <class TIter>
void merge_sort2(TIter b, TIter e) {
    std::vector<int> buff(e-b);
    merge_sort(b, e, buff.begin());
    copy(buff.begin(), buff.end(), b);
}


template <class TIter>
TIter partition1(TIter b, TIter p, TIter e) {
    assert(b < e);
    auto pivot = *p;
    swap(*(e-1), *p);
    auto ub = b;
    auto ue = e-1;
    // [b, ub) [ub, ue) [ue, e)
    // [ <p  ) [      ) [ p<= )
    while (ub < ue) {
        if(*ub < pivot) {
            ++ub;
        } else {
            --ue;
            swap(*ub, *ue);
        }
    }

    swap(*ub, *(e-1));
    return ub;
}

template <class TIter>
TIter partition2(TIter b, TIter p, TIter e) {
    assert(b < e);
    auto pivot = *p;
    iter_swap(e-1, p);
    auto b2 = b;
    auto e2 = b;
    // [b, b2) [b2, e2) [e2, e)
    // [ <p  ) [ p <= ) [ unp )
    while (e2 < e-1) {
        if(*e2 < pivot) {
            swap(*b2, *e2);
            ++b2;
        }
        ++e2;
    }
    iter_swap(e-1, b2);
    return b2;
}


template <class TIter>
TIter pivot_strategy(TIter b, TIter e) {
    assert(b < e);
    auto m = b + (e-b)/2;
    auto last = e-1;
    
    // WRONG
    if(*m < *b) swap(b, m);
    if(*last < *m) swap(last, m);
    if(*m < *b) swap(b, m);
    return m;
}


template <class TIter>
void quck_sort(TIter b, TIter e) {
    if(e-b > 1) {
        auto p = partition2(b, pivot_strategy(b, e), e);
        // assert(*max(b, p) <= *p && p < *min(p+1, e) );
        quck_sort(b, p);
        quck_sort(p+1, e);
    }
}



template <class TSorter>
void test_sorting(TSorter my_sort) {

    typedef vector<int> Array;
    auto sort = [my_sort](vector<int> u) {
        my_sort(u.begin(), u.end());
        return u;
    };

    test(Array(), sort, Array());       // degerate
    test(Array({1}), sort, Array({1})); // trivial
    test(Array({1, 2}), sort, Array({1, 2})); // trivial 2nd
    test(Array({1, 2}), sort, Array({2, 1})); // trivial 2nd
    test(Array({1, 1}), sort, Array({1, 1})); // trivial 2nd
    
    test(Array({1, 1, 1}), sort, Array({1, 1, 1})); 
    test(Array({1, 2, 3}), sort, Array({1, 2, 3})); 
    test(Array({1, 2, 3}), sort, Array({3, 2, 1})); 
    test(Array({1, 2, 3}), sort, Array({2, 3, 1})); 
    
    test(Array({0, 1, 5, 5, 6, 7, 8}), 
        sort, Array({8, 5, 1, 7, 6, 0, 5})); 

}    


} // namespace My

int main(int argc, char const *argv[]) {
    typedef std::vector<int>::iterator Iter;
    using namespace My;
    test_sorting(naive_sort<Iter>);
    test_sorting(selection_sort<Iter>);
    test_sorting(bubble_sort<Iter>);
    test_sorting(insertion_sort<Iter>);
    test_sorting(merge_sort2<Iter>);
    test_sorting(quck_sort<Iter>);
    return 0;
}













