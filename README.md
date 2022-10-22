# parcel
This library reimplements the stl containers using a static allocator, with the purpose of being used with embedded systems. The parcel containers allow for dynamic allocation without a heap, which lets you write clean, modern c++ code without the hassle.

## Usage
The parcel containers can be used just like the standard ones. An example of how they could be used is:
```cpp
#include "parcel_stl.hpp"

int main() {
    parcel::vector<int> vec { 1, 2, 3 };
    vec.push_back(4);
    vec.push_back(5);
    for(auto& i : vec) std::cout << i << ' ';
    std::cout << '\n';

    parcel::string str { "Hello" };
    str.append(" there!");
    std::cout << str << '\n';
}
```

## Notes
Allocation is not thread-safe. Please use caution when integrating with multithreaded applications.

The parcel allocator is also not complete. You may experience some bugs, and if you do, please let me know.
