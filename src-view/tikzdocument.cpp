#include <sealibvisual/tikzdocument.h>

using std::endl;

SealibVisual::TikzDocument::TikzDocument(std::string filename,
                                         std::string tikzLibraries,
                                         std::string gdLibraries, bool lualatex)
    : filename(filename),
      file(filename),
      tikzLibraries(tikzLibraries),
      gdLibraries(gdLibraries),
      lualatex(lualatex) {
  initialize();
}

void SealibVisual::TikzDocument::initialize() {
  using std::endl;

  if (lualatex) {
    file << "% !TeX program = lualatex" << endl;
  }
  file << "% Document: " << filename << endl;
  file << "% Created by sealib TikzDocument" << endl << endl;
  if (lualatex) file << "\\RequirePackage{luatex85}" << endl;
  file << "\\documentclass[tikz,border=20pt]{standalone}" << endl;
  file << "\\standaloneenv{" << blockName << "}" << endl;
  file << "\\usepackage{tikz}" << endl;
  file << "\\usetikzlibrary{" << tikzLibraries << "}" << endl;
  if (gdLibraries != "")
    file << "\\usegdlibrary{" << gdLibraries << "}" << endl;
  file << "\\begin{document}" << endl;
}

void SealibVisual::TikzDocument::close() {
  file << "\\end{document}" << endl;
  file << "% Closing document.";
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
  file << "\\begin{" << blockName << "}\n";
}

void SealibVisual::TikzDocument::endBlock() {
  file << "\\end{" << blockName << "}\n";
}
