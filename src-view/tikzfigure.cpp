#include "sealibvisual/tikzfigure.h"

std::ostream &SealibVisual::TikzFigure::out(std::ostream &os) const {
    os << "\\begin{figure}" << std::endl <<
       "\\centering" << std::endl <<
       (*content) <<
       "\\caption{" << caption << "}" << std::endl <<
       "\\end{figure}" << std::endl;
    return os;
}
