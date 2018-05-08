#include <iostream>
#include <sealib/graphcreator.h>
#include <sealib/graphalgorithms.h>
#include "sealib/graphio.h"
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
    /*Sealib::LocalDyckTable ldt = Sealib::LocalDyckTable();
    TrailStructure ts = TrailStructure(2, ldt);
    ts.enter(1);*/
    /*Sealib::LocalDyckTable ldt = Sealib::LocalDyckTable();
    TrailStructure ts = TrailStructure(38, ldt);
    *//*ts.enter(0);
    ts.enter(6);
    //ts.enter(14);
    ts.enter(12);
    ts.enter(3);
    ts.enter(5);
    ts.enter(10);
    ts.enter(9);*//*

    ts.enter(1);
    ts.enter(4);
    ts.enter(9);
    ts.enter(11);
    ts.enter(18);
    ts.enter(22);
    ts.enter(24);
    ts.enter(27);
    ts.enter(31);
    ts.enter(34);
    ts.enter(30);
    ts.enter(26);
    ts.enter(21);

    ts.enter(8);
    ts.enter(7);
    ts.enter(6);
    ts.enter(3);
    ts.enter(0);
    ts.enter(20);
    cout <<endl<<"DEPTHS"<<endl;
    for(unsigned int i = 0; i<7; i++) {
        cout << (int) ldt[76]->localDepths[i] << ", ";
    }
    cout <<"MATCHES"<<endl;
    for(unsigned int i = 0; i<7; i++) {
        cout << (int) ldt[76]->localMatches[i] << ", ";
    }
    cout <<"PIONEERS"<<endl;
    cout << (int) ldt[76]->leftPioneer << ", ";
    cout << (int) ldt[76]->rightPioneer << ", ";
    cout << endl;




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
    for(unsigned int i = 0; i < ts.getMatched().size(); i++) {
        cout << i << "\t" << ts.getMatched(i) << endl;
    }
    cout << endl;

    //boost::dynamic_bitset<> dw = ts.getDyckWord();

    cout << "idx\tmatch" << endl;
    for(unsigned int i = 0; i < ts.getMatched().size(); i++) {
        cout << i << "\t" << ts.getMatchedNew(i) << endl;
    }
    cout << endl;*/


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

    clock_t start = clock();
    Graph *g = Sealib::GraphIO::randomEulerianSealibGraph(40000,2500000);
    /*unsigned int order = 5;
    unsigned int **adj_mtrx = new unsigned int *[order];
    adj_mtrx[0] = new unsigned int[order]{0, 1, 0, 1, 1};
    adj_mtrx[1] = new unsigned int[order]{1, 0, 1, 0, 0};
    adj_mtrx[2] = new unsigned int[order]{0, 1, 0, 0, 0};
    adj_mtrx[3] = new unsigned int[order]{1, 0, 0, 0, 1};
    adj_mtrx[4] = new unsigned int[order]{1, 0, 0, 1, 0};

    Sealib::Graph *g = Sealib::GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);
*/
    clock_t end = clock();
    cout << "GENERATION: " << (end - start)/CLOCKS_PER_SEC << "s" << endl;

    start = clock();
    TrailStructure **ts = GraphAlgorithms::hierholzer(g);
    end = clock();
    cout << "HIERHOLZER: " << (end - start)/CLOCKS_PER_SEC << "s" << endl;

    /*ts[1]->getMatchedNew(0);
    ts[1]->getMatchedNew(1);
    ts[1]->getMatchedNew(2);*/
    start = clock();
    Sealib::GraphIO::stringFromTrail(g, ts);
    end = clock();
    cout << "STRING: " << (end - start)/CLOCKS_PER_SEC << "s" << endl;
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

