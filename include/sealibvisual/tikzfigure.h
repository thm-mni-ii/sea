#ifndef SEALIBVISUAL_TIKZFIGURE_H_
#define SEALIBVISUAL_TIKZFIGURE_H_

#include <sealibvisual/tikzpicture.h>
#include <sealibvisual/tikzelement.h>
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
    TikzFigure(const std::string &caption,
               const std::shared_ptr<TikzPicture> &content);
    const std::string &getCaption();
    void setCaption(const std::string &caption);
    std::shared_ptr<TikzElement> &getContent();
    void setContent(const std::shared_ptr<TikzElement> &content);
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZFIGURE_H_
