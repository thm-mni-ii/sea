#ifndef SEALIBVISUAL_TIKZFIGURE_H_
#define SEALIBVISUAL_TIKZFIGURE_H_

#include "./tikzpicture.h"
#include "./tikzelement.h"
#include <iostream>
#include <sstream>
#include <string>
#include <memory>


namespace SealibVisual {
/**
 * Abstract class representing a single Element in a TikzPicture.
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
    const std::string &getCaption() { return caption; }
    void setCaption(const std::string &_caption) {
        caption=_caption;
    }
    std::shared_ptr<TikzElement> &getContent() { return content; }
    void setContent(const std::shared_ptr<TikzElement> &_content) {
        content=_content;
    }
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZFIGURE_H_
