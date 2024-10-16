
#include <iostream>
#include <list>

using std::list;


using namespace std;

int main() {

    list<int> ls1= {1,2};
    ls1.splice(ls1.end(),ls1,ls1.begin());
    for(auto&num:ls1)
    {
        std::cout<<num<<",";
    }
    std::cout<<"\n";
    return 0;
}
