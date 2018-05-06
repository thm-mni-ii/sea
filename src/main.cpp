#include <iostream>
#include <sealib/graphcreator.h>
#include <sealib/graphalgorithms.h>
#include <QApplication>
#include "mainwindow.h"
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/planarlayout/PlanarDrawLayout.h>
#include <ogdf/planarlayout/PlanarStraightLayout.h>
#include <ogdf/planarlayout/MixedModelLayout.h>
#include <sealib/dynamicbitsetextension.h>
#include <bitset>
#include <include/sealib/localdycktable.h>

using namespace std;
using namespace Sealib;



int main(/*int argc, char *argv[]*/) {
    /*QApplication app(argc, argv);

    MainWindow mw;
    mw.show();



    app.exec();*/

    Sealib::LocalDyckTable ldt = Sealib::LocalDyckTable();
    TrailStructure ts = TrailStructure(13, ldt);
    /*ts.enter(0);
    ts.enter(6);
    //ts.enter(14);
    ts.enter(12);
    ts.enter(3);
    ts.enter(5);
    ts.enter(10);
    ts.enter(9);*/

    ts.enter(6);
    ts.enter(5);
    ts.enter(4);
    ts.enter(3);
    ts.enter(2);
    ts.enter(1);
    ts.enter(0);

    cout << "MATCHED:" << endl;
    for(unsigned int i = 0; i < ts.getMatched().size(); i++) {
        cout << ts.getMatched()[i];
    }
    cout << endl;

    cout << "INOUT:" << endl;
    for(unsigned int i = 0; i < ts.getInAndOut().size(); i++) {
        cout << ts.getInAndOut()[i];
    }
    cout << endl;

    cout << "idx\tmatch" << endl;
    for(unsigned int i = 0; i < 13; i++) {
        cout << i << "\t" << ts.getMatched(i) << endl;
    }
    cout << endl;

    boost::dynamic_bitset<> dw = ts.getDyckWord();

    cout << "idx\tmatch" << endl;
    for(unsigned int i = 0; i < 13; i++) {
        cout << i << "\t" << ts.getMatchedNew(i) << endl;
    }
    cout << endl;

    ts.getMatchedNew(10);


    /*unsigned long segment;
    unsigned long beg = 0;
    unsigned long end = 7;
    boost::to_block_range(dw, make_tuple(beg, end, std::ref(segment)));
    Sealib::LocalDyckTable ldt = Sealib::LocalDyckTable();

    cout << "segment 1:" << endl;

    cout << "Idx\tMatch\tDepth" << endl;
    for(unsigned long i  = 0; i < 7; i++) {
        cout << i << "\t" << (unsigned int) ldt[segment]->localMatches[i] <<"\t\t" << (int) ldt[segment]->localDepths[i] << endl;
    }
    cout << "Depths:" << endl;
    for(unsigned long  i = 0; i < 7; i++) {
        cout << i << " : " << (int) ldt[segment]->localDepths[i] << endl;
    }
    cout << "Pioneers: L" << (unsigned int) ldt[segment]->leftPioneer << " R " << (unsigned int) ldt[segment]->rightPioneer << endl << endl;

    cout << "segment 2:" << endl;

    beg = 7;
    end = 14;
    boost::to_block_range(dw, make_tuple(beg, end, std::ref(segment)));

    cout << "Idx\tMatch\tDepth" << endl;
    for(unsigned long i  = 0; i < 7; i++) {
        cout << i << "\t" << (unsigned int) ldt[segment]->localMatches[i] <<"\t\t" << (int) ldt[segment]->localDepths[i] << endl;
    }
    cout << "Depths:" << endl;
    for(unsigned long  i = 0; i < 7; i++) {
        cout << i << " : " << (int) ldt[segment]->localDepths[i] << endl;
    }
    cout << "Pioneers: L" << (unsigned int) ldt[segment]->leftPioneer << " R " << (unsigned int) ldt[segment]->rightPioneer << endl << endl;



    cout << endl;*/


    /*boost::dynamic_bitset<> db = boost::dynamic_bitset<>(7);
    db[0] = 1;

    db[5] = 1;

    unsigned long res;
    unsigned long beg = 0;
    unsigned long end = 7;
    boost::to_block_range(db, make_tuple(beg, end, std::ref(res)));

    Sealib::LocalDyckTable ldt = Sealib::LocalDyckTable();

    for(int j = 0; j < std::pow(2,7); j++) {
        std::bitset<7> bs(j);
        cout << bs << endl;
        cout << "Idx\tMatch\tDepth" << endl;
        for(unsigned long i  = 0; i < 7; i++) {
            cout << i << "\t" << (unsigned int) ldt[j]->localMatches[i] <<"\t\t" << (int) ldt[j]->localDepths[i] << endl;
        }
        cout << "Depths:" << endl;
        for(unsigned long  i = 0; i < 7; i++) {
            cout << i << " : " << (int) ldt[j]->localDepths[i] << endl;
        }
        cout << "Pioneers: L" << (unsigned int) ldt[j]->leftPioneer << " R " << (unsigned int) ldt[j]->rightPioneer << endl << endl;

    }*/

}

/*
namespace boost {
    template <typename Block, typename Allocator, typename BlockOutputIterator>
    inline void
    to_block_range(const dynamic_bitset<Block, Allocator>& b,
                   BlockOutputIterator result)
    {
        std::copy(b.m_bits.begin(), b.m_bits.end(), result);
    }

}*/

