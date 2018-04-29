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

using namespace std;
using namespace ogdf;

int main(int argc, char *argv[]) {


    /*unsigned int order = 18;
    auto **adj_mtrx = new unsigned int *[order];

    adj_mtrx[0] = new unsigned int[order]   {0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1};
    adj_mtrx[1] = new unsigned int[order]   {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[2] = new unsigned int[order]   {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0};
    adj_mtrx[3] = new unsigned int[order]   {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[4] = new unsigned int[order]   {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[5] = new unsigned int[order]   {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[6] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[7] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[8] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[9] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[10] = new unsigned int[order]  {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
    adj_mtrx[11] = new unsigned int[order]  {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};

    adj_mtrx[12] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    adj_mtrx[13] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 5, 0, 0};
    adj_mtrx[14] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    adj_mtrx[15] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0};

    adj_mtrx[16] = new unsigned int[order]  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[17] = new unsigned int[order]  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    Sealib::Graph *g  = GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);

    ogdf::Graph *G = GraphAlgorithms::ogdfGraphFromSealibGraph(g);

    ogdf::GraphAttributes GA(*G, GraphAttributes::nodeGraphics |
                                 GraphAttributes::edgeGraphics |
                                 GraphAttributes::nodeLabel |
                                 GraphAttributes::nodeStyle |
                                 GraphAttributes::edgeType |
                                 GraphAttributes::edgeArrow |
                                 GraphAttributes::edgeStyle );
    ogdf::List<ogdf::node> nodes;
    G->allNodes(nodes);
    for(auto v: nodes) {
        GA.fillColor(v) = Color( "#FFFF00" ); // set node color to yellow

        GA.height(v) = 20.0; // set the height to 20.0
        GA.width(v) = 20.0; // set the width to 40.0
        GA.shape(v) = ogdf::Shape::Ellipse;

        string s = to_string(v->index());
        char const *pchar = s.c_str(); //use char const* as target type
        GA.label(v) = pchar;
    }

    ogdf::List<ogdf::edge> edges;
    G->allEdges(edges);
    for(auto e: edges) {
        GA.arrowType(e) = ogdf::EdgeArrow::None;
        GA.bends(e);
        GA.strokeColor(e) = Color("#0000FF");
        GA.strokeWidth(e) = 2;
    }

    SugiyamaLayout SL; //Compute a hierarchical drawing of G (using SugiyamaLayout)
    SL.setRanking( new OptimalRanking );
    SL.setCrossMin( new MedianHeuristic );

    auto *ohl = new OptimalHierarchyLayout;

    SL.setLayout( ohl );
    SL.call( GA );

    std::ofstream o = std::ofstream("ogdfConversion.gml");
    GraphIO::writeGML(GA, o);
    o.close();*/

    //GraphAlgorithms::dotFileFromGraph(g, "sealibConversion.dot");
    /*unsigned int order = 18;
    auto **adj_mtrx = new unsigned int *[order];

    adj_mtrx[0] = new unsigned int[order]   {0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1};
    adj_mtrx[1] = new unsigned int[order]   {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[2] = new unsigned int[order]   {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0};
    adj_mtrx[3] = new unsigned int[order]   {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[4] = new unsigned int[order]   {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[5] = new unsigned int[order]   {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[6] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[7] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[8] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[9] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[10] = new unsigned int[order]  {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
    adj_mtrx[11] = new unsigned int[order]  {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};

    adj_mtrx[12] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    adj_mtrx[13] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0};
    adj_mtrx[14] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    adj_mtrx[15] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};

    adj_mtrx[16] = new unsigned int[order]  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[17] = new unsigned int[order]  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    Sealib::Graph *g  = GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);
    GraphAlgorithms::dotFileFromGraph(g, "graph.dot");
    TrailStructure **ts = GraphAlgorithms::hierholzer(g);
    cout << GraphAlgorithms::stringFromTrail(g, ts);

    GraphAlgorithms::dotFileFromTrail(g, ts, "trail.dot");

    ogdf::GraphAttributes GA = GraphAlgorithms::graphAttributesFromTrail(g, ts);

    SugiyamaLayout SL; //Compute a hierarchical drawing of G (using SugiyamaLayout)
    SL.setRanking( new OptimalRanking );
    SL.setCrossMin( new MedianHeuristic );

    auto *ohl = new OptimalHierarchyLayout;

    SL.setLayout( ohl );
    SL.call( GA );

    std::ofstream o = std::ofstream("sugiyama.svg");
    GraphIO::drawSVG(GA, o);
    o.close();

    PlanarizationLayout PL;
    PL.call(GA);

    o = std::ofstream("planar.svg");
    GraphIO::drawSVG(GA, o);
    o.close();*/

    /*clock_t begin = clock();
    ogdf::Graph *ogdfGraph;
    //for(int i = 0; i < 1000; i++) {
        ogdfGraph = GraphAlgorithms::randomEulerianOgdfGrah(50000, 1000000);
    //}
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Elapsed Time to generate random graph: " << elapsed_secs << endl;

    *//*ogdf::Graph* ogdfGraph = new Graph();
    clock_t begin = clock();
    std::ifstream i = std::ifstream("randomEulerGraph.dot");
    GraphIO::readDOT(*ogdfGraph, i);
    i.close();
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Elapsed Time to read graph from dot: " << elapsed_secs << endl;
*//*
    begin = clock();
    Sealib::Graph *g = GraphAlgorithms::sealibGraphFromOgdfGraph(ogdfGraph);
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Elapsed Time to convert ogdf to sealib: " << elapsed_secs << endl;

    begin = clock();

    TrailStructure **ts =
            GraphAlgorithms::hierholzer(g);
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Elapsed Time to calculate tour: " << elapsed_secs << endl;

    begin = clock();
    //string s =
            GraphAlgorithms::stringFromTrail(g, ts);
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Elapsed Time to write tour as string: " << elapsed_secs << endl;*/

    //cout << s << endl;

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
/*

int hierholzerTest() {
    unsigned int order = 18;
    auto **adj_mtrx = new unsigned int *[order];

    adj_mtrx[0] = new unsigned int[order]   {0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1};
    adj_mtrx[1] = new unsigned int[order]   {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[2] = new unsigned int[order]   {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0};
    adj_mtrx[3] = new unsigned int[order]   {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[4] = new unsigned int[order]   {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[5] = new unsigned int[order]   {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[6] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[7] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[8] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[9] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[10] = new unsigned int[order]  {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
    adj_mtrx[11] = new unsigned int[order]  {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};

    adj_mtrx[12] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    adj_mtrx[13] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 5, 0, 0};
    adj_mtrx[14] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    adj_mtrx[15] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0};

    adj_mtrx[16] = new unsigned int[order]  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[17] = new unsigned int[order]  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    Sealib::Graph *g  = GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);
    GraphAlgorithms::dotFileFromGraph(g, "graph.dot");
    TrailStructure **ts = GraphAlgorithms::hierholzer(g);
    cout << GraphAlgorithms::stringFromTrail(g, ts);

    GraphAlgorithms::dotFileFromTrail(g, ts, "trail.dot");

    ogdf::GraphAttributes *GA = GraphAlgorithms::graphAttributesFromTrail(g, ts);
    std::ofstream o = std::ofstream("ogdfTraiTest.svg");
    GraphIO::drawSVG(*GA, o);
    return 0;
}*/
