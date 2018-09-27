#include <sealibvisual/tikzfigure.h>

std::ostream &SealibVisual::TikzFigure::out(std::ostream &os) const {
    os << "\\begin{figure}" << std::endl <<
       "\\centering" << std::endl <<
       (*content) <<
       "\\caption{" << caption << "}" << std::endl <<
       "\\end{figure}" << std::endl;
    return os;
}
SealibVisual::TikzFigure::TikzFigure(const std::string &caption,
                                     const std::shared_ptr<SealibVisual::TikzPicture> &content)
    : caption(caption), content(content) {}

const std::string &SealibVisual::TikzFigure::getCaption() {
    return caption;
}
void SealibVisual::TikzFigure::setCaption(const std::string &caption) {
    TikzFigure::caption = caption;
}
std::shared_ptr<SealibVisual::TikzElement> &SealibVisual::TikzFigure::getContent() {
    return content;
}
void SealibVisual::TikzFigure::setContent(
    const std::shared_ptr<SealibVisual::TikzElement> &content) {
    TikzFigure::content = content;
}
