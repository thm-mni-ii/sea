
#include <sealib/graphcreator.h>
#include <iostream>
#include <sealib/trailstructure.h>

using namespace std;

int main() {
    TrailStructure ts = TrailStructure(5);
    unsigned int l = ts.leave();

    l = ts.leave();

    cout << "blabla";
}