#include <sealib/tikzdocument.h>

using std::endl;

SealibVisual::TikzDocument::TikzDocument(std::string filename, std::string libraries) :
    filename(filename), file(filename), libraries(libraries) {
    initialize();
}

void SealibVisual::TikzDocument::initialize() {
    using std::endl;

    file << "% Document: " << filename << endl;
    file << "% Created by sealib TikzDocument" << endl << endl;
    file << "\\documentclass[tikz,border=2mm]{standalone}" << endl;
    file << "\\usetikzlibrary{" << libraries << "}" << endl;
    file << "\\begin{document}" << endl;
}

void SealibVisual::TikzDocument::close() {
    file << "\\end{document}" << endl;
    file << "% Closing document.";
    file.close();
}

bool SealibVisual::TikzDocument::isOpen() {
    return file.is_open();
}


void SealibVisual::TikzDocument::add(const SealibVisual::TikzElement *element) {
    file << (*element) << std::endl;
}

void SealibVisual::TikzDocument::add(std::shared_ptr<SealibVisual::TikzElement> element) {
    file << (*element) << std::endl;
}

void SealibVisual::TikzDocument::add(const std::string &line) {
    file << line << std::endl;
}

void SealibVisual::TikzDocument::add(const char* line) {
    file << line << std::endl;
}

void SealibVisual::TikzDocument::add(const SealibVisual::TikzElement &element) {
    file << element << std::endl;

}


