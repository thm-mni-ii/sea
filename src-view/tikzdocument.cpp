#include "sealibvisual/tikzdocument.h"
#include <iostream>

void SealibVisual::TikzDocument::initialize() {
  using std::endl;

  if (mode == "standalone") {
    if (lualatex) {
      file << "% !TeX program = lualatex" << endl;
    }
    file << "% Document: " << filename << endl;
    file << "% Created by sealib TikzDocument" << endl << endl;
    if (lualatex) file << "\\RequirePackage{luatex85}" << endl;
    file << "\\documentclass[tikz,border=20pt]{standalone}" << endl;
    // file << "\\standaloneenv{" << blockName << "}" << endl;
    file << "\\usepackage{tikz}" << endl;
    file << "\\usetikzlibrary{" << tikzLibraries << "}" << endl;
    if (gdLibraries != "")
      file << "\\usegdlibrary{" << gdLibraries << "}" << endl;
    file << "\\begin{document}" << endl;
  } else if (mode == "beamer") {
    file << "% Document: " << filename << endl;
    file << "% Created by sealib TikzDocument" << endl << endl;
    file << "%  Your beamer presentation:\n";
    file << "% \\documentclass{beamer}" << endl;
    file << "% \\usepackage{tikz}" << endl;
    file << "% \\usetikzlibrary{" << tikzLibraries << "}" << endl;
    if (gdLibraries != "")
      file << "% \\usegdlibrary{" << gdLibraries << "}" << endl;
    file << "% \\begin{document}" << endl;
    file << "% ...\n";
    file << "% \\input{" << filename
         << "}\t\t% You can auto-animate with \\transduration{1.5}, etc.\n";
    file << "% ...\n";
  }
}

void SealibVisual::TikzDocument::close() {
  if (mode == "standalone") {
    file << "\\end{document}" << std::endl;
    file << "% Closing document.";
  }
  file.close();
}

bool SealibVisual::TikzDocument::isOpen() { return file.is_open(); }

void SealibVisual::TikzDocument::add(const SealibVisual::TikzElement *element) {
  file << (*element) << std::endl;
}

void SealibVisual::TikzDocument::add(
    std::shared_ptr<SealibVisual::TikzElement> element) {
  file << (*element) << std::endl;
}

void SealibVisual::TikzDocument::add(const std::string &line) {
  file << line << std::endl;
}

void SealibVisual::TikzDocument::add(const char *line) {
  file << line << std::endl;
}

void SealibVisual::TikzDocument::add(const SealibVisual::TikzElement &element) {
  file << element << std::endl;
}

void SealibVisual::TikzDocument::beginBlock() {
  static unsigned slide = 1;
  if (mode == "standalone") {
    file << "\\begin{" << blockName << "}\n";
  } else if (mode == "beamer") {
    file << "\n\\only<" << slide << ">{\n";
    slide++;
  }
}

void SealibVisual::TikzDocument::endBlock() {
  if (mode == "standalone")
    file << "\\end{" << blockName << "}\n";
  else if (mode == "beamer")
    file << "\n}\n";
}
