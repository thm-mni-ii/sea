#include <sealibvisual/tikzdocument.h>

using std::endl;

SealibVisual::TikzDocument::TikzDocument(std::string filename,
                                         std::string tikzLibraries,
                                         std::string gdLibraries, bool lualatex,
                                         bool animate)
    : filename(filename),
      file(filename),
      tikzLibraries(tikzLibraries),
      gdLibraries(gdLibraries),
      lualatex(lualatex),
      animate(animate) {
  initialize();
}

void SealibVisual::TikzDocument::initialize() {
  using std::endl;

  if (lualatex) {
    file << "% !TeX program = lualatex" << endl;
  }
  file << "% Document: " << filename << endl;
  file << "% Created by sealib TikzDocument" << endl << endl;
  file << "\\RequirePackage{luatex85}" << endl;
  file << "\\documentclass[tikz,border=20pt]{standalone}" << endl;
  file << "\\standaloneenv{tikzContainer}" << endl;
  file << "\\usepackage{tikz}" << endl;
  if (animate) file << "\\usepackage{animate}" << endl;
  file << "\\usetikzlibrary{" << tikzLibraries << "}" << endl;
  if (gdLibraries != "")
    file << "\\usegdlibrary{" << gdLibraries << "}" << endl;
  file << "\\begin{document}" << endl;
  if (animate) {
    file << "\\begin{animateinline}[controls,loop]{1}" << endl;
  }
}

void SealibVisual::TikzDocument::close() {
  if (animate) file << "\\end{animateinline}" << endl;
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
