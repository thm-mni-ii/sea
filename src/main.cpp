
#include <sealib/graphcreator.h>
#include <iostream>
#include <sealib/trailstructure.h>

using namespace std;

int main() {
    TrailStructure ts = TrailStructure(5);

    cout << ts.enter(2) << endl;
    cout << ts.enter(4)<< endl;
    cout << ts.enter(1) << endl << endl;

    cout << ts.getMatched(0) << endl;
    cout << ts.getMatched(1) << endl;
    cout << ts.getMatched(2) << endl;
    cout << ts.getMatched(3) << endl;
    cout << ts.getMatched(0) << endl << endl;

    ts.marry(0,1);
    cout << ts.getMatched(0) << endl;
    cout << ts.getMatched(1) << endl << endl;

    cout << endl;
    cout << ts.getMatched(4) << endl;
}