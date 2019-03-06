#ifndef SEALIBVISUAL_TIKZFIGURE_H_
#define SEALIBVISUAL_TIKZFIGURE_H_

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include "./tikzelement.h"
#include "./tikzpicture.h"

namespace SealibVisual {
/**
 * Representation of a figure. You can embed content into a TEX figure to give
 * it a caption and make it "floating".
 */
class TikzFigure : public TikzElement {
 private:
    std::string caption;
    std::shared_ptr<TikzElement> content;

 protected:
    std::ostream &out(std::ostream &os) const override;

 public:
    TikzFigure(const std::string &_caption,
               const std::shared_ptr<TikzPicture> &_content)
        : caption(_caption), content(_content) {}
    TikzFigure(const TikzFigure &) = default;
    TikzFigure(TikzFigure &&) noexcept = default;
    TikzFigure& operator=(const TikzFigure &) = default;
    // TikzFigure& operator=(TikzFigure &&) noexcept = default;
    virtual ~TikzFigure() = default;

    const std::string &getCaption() { return caption; }
    void setCaption(const std::string &_caption) { caption = _caption; }
    std::shared_ptr<TikzElement> &getContent() { return content; }
    void setContent(const std::shared_ptr<TikzElement> &_content) {
        content = _content;
    }
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZFIGURE_H_
