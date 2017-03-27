#include <iostream>
#include <vector>
using namespace std;

template <class T>
ostream& operator<<(ostream& o, const vector<T>& v) {
    o << "[";
    for(auto& x : v) {
        o << x << ", ";
    }
    return o << "]";
}

template <class TFunc, class TResult, class TParam1>
void test(TResult expect, TFunc f, TParam1 p1) {
    auto got = f(p1);
    if(got != expect) {
        cerr << "failed: " << expect << " != " << got << endl;
    }
}

template <class TFunc, class TResult, class TParam1, class TParam2>
void test(TResult expect, TFunc f, TParam1 p1, TParam2 p2) {
    auto got = f(p1, p2);
    if(got != expect) {
        cerr << "failed: f(" << p1 << ", " << p2 << ") = "
            << got << " != " << expect << endl;
    }
}


////////////////////////////////////////////////

template <class TIter, class T>
TIter my_lower_bound(TIter b, TIter e, const T& key) {

    while(b < e) {
        auto m = b + (e-b)/2;
        if(*m < key) {
            b = m+1;
        } else {
            e = m;
        }
    }
    return b;
}

template <class TIter, class T>
TIter my_upper_bound(TIter b, TIter e, const T& key) {
    while(b < e) {
        auto m = b + (e-b)/2;
        if(key < *m) {
            e = m;
        } else {
            b = m+1;
        }
    }
    return b;
}

template <class TIter, class T>
TIter my_binary_search(TIter b, TIter e, const T& key) {
    auto lb = my_lower_bound(b, e, key);
    return b != e && !(key < *lb) ? lb : e;
}




void test_search() {

    typedef vector<int> Array;

    auto search = [](const vector<int>& v, int key) {
        auto r = my_binary_search(v.begin(), v.end(), key);
         return r != v.end() ? r-v.begin() : -1;
    };

    auto key = 8;
    // key not exists in array
    test(-1, search, Array(), key); // degerate
    test(-1, search, Array({key-1}), key);          // trivial
    test(-1, search, Array({key+1}), key);          // trivial
    test(-1, search, Array({key-2, key-1}), key);   // trivial2
    test(-1, search, Array({key+1, key+2}), key);   // trivial2
    test(-1, search, Array({key-1, key+1}), key);   // trivial2
    test(-1, search, Array({1,2,3,4,5,7}), key);    // general
    test(-1, search, Array({9,10,11,12}), key);     // general
    test(-1, search, Array({1,2,4,7,10}), key);     // general
    
    // key exists in array
    // non appliable // degerate
    test(0, search, Array({key}), key);                     // trivial
    test(0, search, Array({key, key+1}), key);              // trivial2
    test(1, search, Array({key-1, key}), key);              // trivial2
    test(8, search, Array({0,1,2,3,4,5,6,7,key}), key);     // general
    test(0, search, Array({key, 9,10,11,12}), key);         // general
    test(2, search, Array({1,4,key,9,10}), key);            // general                
    
    test(0, search, Array({key, key, key+1, key+2}), key);  // general                
    test(2, search, Array({key-2, key-1, key, key}), key);  // general                
    test(2, search, Array({1, 7, key, key}), key);          // general
}


int main(int argc, char const *argv[]) {
    test_search();
    return 0;
}













