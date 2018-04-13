#include <iostream>
#include <sealib/trailstructure.h>

using namespace std;

int main() {
    TrailStructure ts = TrailStructure(5);

    cout << ts.enter(0) << endl;
    cout << ts.enter(4)<< endl;
    cout << ts.leave()<< endl;
}