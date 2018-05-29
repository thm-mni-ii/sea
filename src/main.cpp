

#include <ctime>
#include <iostream>
#include <include/sealib/graph.h>
#include <include/sealib/graphio.h>
#include <include/sealib/rankselect.h>
#include <include/sealib/graphalgorithms.h>
#include <include/sealib/differencelist.h>
#include <include/sealib/recursivedyckmatchingstructure.h>
#include <include/sealib/doublelinkedlist.h>
#include <include/sealib/graphcreator.h>
#include <QtWidgets/QApplication>
#include <QtGui/QIcon>
#include <include/sealib/localranktable.h>
#include <include/sealib/localselecttable.h>
#include <include/sealib/simpletrail.h>
#include "mainwindow.h"

void testHierholzer() {
    clock_t start = clock();
    Sealib::Graph *g = Sealib::GraphIO::randomEulerianSealibGraph(1500, 45000);
    clock_t end = clock();
    std::cout << "GENERATION: " << (end - start)/CLOCKS_PER_SEC << "s" << std::endl;
    start = clock();
    Sealib::TrailStructure **ts = Sealib::GraphAlgorithms::hierholzer(g);
    end = clock();
    std::cout << "HIERHOLZER: " << (end - start)/CLOCKS_PER_SEC << "s" << std::endl;
    start = clock();
    std::cout << Sealib::GraphIO::stringFromTrail(g, ts) << std::endl;
    end = clock();
    std::cout << "STRING: " << (end - start)/CLOCKS_PER_SEC << "s" << std::endl;
}

void testRankStructure() {
    boost::dynamic_bitset<unsigned char> bits(16);
    bits[0] = 1;
    bits[7] = 1;
    bits[8] = 1;
    bits[15] = 1;
    Sealib::RankSelect rankSelect(bits);
    std::cout << rankSelect.select(1) << std::endl;
    std::cout << rankSelect.select(2) << std::endl;
    std::cout << rankSelect.select(3) << std::endl;
    std::cout << rankSelect.select(4) << std::endl;
}


boost::dynamic_bitset<> wrapDyckWord(const boost::dynamic_bitset<> &toWrap) {
    boost::dynamic_bitset<> wrapped(toWrap.size() + 2);
    wrapped[0] = 1;
    for(unsigned long i = 0; i < toWrap.size(); i++) {
        wrapped[i+1] = toWrap[i];
    }
    wrapped[wrapped.size()-1] = 0;
    return wrapped;
}

boost::dynamic_bitset<> concatDyckWord(const boost::dynamic_bitset<>& word1,const boost::dynamic_bitset<>& word2) {
    boost::dynamic_bitset<> res(word1);
    res.resize(word1.size()+word2.size());
    size_t bs1Size=word1.size();
    size_t bs2Size=word2.size();

    for(size_t i=0;i<bs2Size;i++)
        res[i+bs1Size]=word2[i];
    return res;
}
boost::dynamic_bitset<> generateRandomDyckword(unsigned long size) {
    boost::dynamic_bitset<> bits(2);
    bits[0] = 1;
    while(bits.size() < size) {
        if(rand() % 2 == 0) {
            bits = wrapDyckWord(bits);
        } else {
            bits = concatDyckWord(bits, generateRandomDyckword(size/2));
        };
    }

    return bits;
}

int visual(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    QIcon appIcon;
    appIcon.addFile(":/Icons/AppIcon32");
    appIcon.addFile(":/Icons/AppIcon128");
    app.setWindowIcon(appIcon);
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}

void testAllDyckWords() {
    unsigned char c1 = 0 ;
    do {
        unsigned char c2 = 0 ;
        do {
            unsigned char c3 = 0;
            do {
                for(unsigned int off = 0; off < 20; off+=2) {
                    boost::dynamic_bitset<unsigned char> bs;
                    bs.append(c1);
                    bs.append(c2);
                    bs.append(c3);
                    bs.resize(bs.size() - off);

                    if (bs.count() == bs.size() / 2) {
                        Sealib::DyckMatchingStructure dyckMatchingStructure(bs);
                        std::vector<unsigned long> matches(bs.size());
                        bool isWord = true;
                        for (unsigned int i = 0; i < bs.size(); i++) {
                            unsigned long match = dyckMatchingStructure.findMatch(i);
                            if (match == i) {
                                isWord = false;
                                break;
                            }
                            matches[i] = match;
                        }
                        if (isWord) {
                            Sealib::RecursiveDyckMatchingStructure recursiveDyckMatchingStructure(bs);
                            for (unsigned int i = 0; i < bs.size(); i++) {
                                unsigned long match = recursiveDyckMatchingStructure.findMatch(i);
                                if (match != matches[i]) {
                                    std::cout << "WORD: " << std::endl;
                                    Sealib::DyckMatchingStructure::printDyckWord(bs);
                                    std::cout << "CORRECT MATCH: " << i << " : " << matches[i] << std::endl;
                                    std::cout << "ENCOUNTERED MATCH: " << i << " : " << match << std::endl;
                                }
                            }
                        }
                    }
                }
            }
            while (++c3);
        }
        while ( ++c2 ) ;
    }
    while ( ++c1 ) ;
}

int stuff(){ return 0;}

int main() {
    Sealib::SimpleTrail simpleTrail;
    for(unsigned int i = 0; i < 5; i++) {
        simpleTrail.addEdge(i);
    }
    for(auto a: simpleTrail.getTrail()) {
        std::cout << a << " ";
    }
    std::cout << std::endl;

    Sealib::SimpleTrail subTrail;
    for(unsigned int i = 5; i < 10; i++) {
        subTrail.addEdge(i);
    }
    for(auto a: subTrail.getTrail()) {
        std::cout << a << " ";
    }
    std::cout << std::endl;

    simpleTrail.insertSubTrail(subTrail, 5);
    for(auto a: simpleTrail.getTrail()) {
        std::cout << a << " ";
    }
    std::cout << std::endl;


    //testHierholzer();
    /*testAllDyckWords();
    boost::dynamic_bitset<unsigned char> bs(22);
    for(unsigned int i = 0; i < 8; i++) {
        bs[i] = 1;
    }
    bs[13] = 1;
    bs[14] = 1;
    bs[15] = 1;
    Sealib::DyckMatchingStructure::printDyckWord(bs);
    Sealib::DyckMatchingStructure dyckMatchingStructure(bs);
    Sealib::RecursiveDyckMatchingStructure recursiveDyckMatchingStructure(bs);
    for(unsigned int i = 0; i < bs.size(); i++) {
        unsigned long match = recursiveDyckMatchingStructure.findMatch(i);
        unsigned long matchSimple = dyckMatchingStructure.findMatch(i);
        if(match != matchSimple) {
            std::cout << "WORD: " << std::endl;
            Sealib::DyckMatchingStructure::printDyckWord(bs);
            std::cout << "CORRECT MATCH: " << i << " : " << matchSimple << std::endl;
            std::cout << "ENCOUNTERED MATCH: " << i << " : " << match << std::endl;
        }
    }

    return 0;*/


    /*QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    QIcon appIcon;
    appIcon.addFile(":/Icons/AppIcon32");
    appIcon.addFile(":/Icons/AppIcon128");
    app.setWindowIcon(appIcon);
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();*/
}
