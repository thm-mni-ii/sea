

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
#include "mainwindow.h"

void testHierholzer() {
    clock_t start = clock();
    Sealib::Graph *g = Sealib::GraphIO::randomEulerianSealibGraph(1000, 10000);
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
    boost::dynamic_bitset<> bits(2);
    Sealib::SimpleRankSelect simple(bits);

    std::cout << simple.select(0) << std::endl;
    std::cout << simple.rank(0) << std::endl;

    std::cout << simple.select(1) << std::endl;
    std::cout << simple.rank(1) << std::endl;

    std::cout << simple.select(1241241) << std::endl;
    std::cout << simple.rank(1241241) << std::endl;

    Sealib::RankSelect rankSelect(bits);
    std::cout << bits.size() << std::endl;
    std::cout << rankSelect.select(0) << std::endl;
    std::cout << rankSelect.rank(0) << std::endl;

    std::cout << rankSelect.select(1) << std::endl;
    std::cout << rankSelect.rank(1) << std::endl;

    std::cout << rankSelect.select(2) << std::endl;
    std::cout << rankSelect.rank(2) << std::endl;

    std::cout << rankSelect.select(1241241) << std::endl;
    std::cout << rankSelect.rank(1241241) << std::endl;
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

int main(int argc, char *argv[]) {
    testHierholzer();
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
