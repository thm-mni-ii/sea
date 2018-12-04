#ifndef SEALIBVISUAL_TIKZPICTURE_H_
#define SEALIBVISUAL_TIKZPICTURE_H_

#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "./tikzelement.h"

namespace SealibVisual {
/**
 * Representation of a tikzpicture / scope. By default, a scope will be
 * generated so that multiple scopes can be nested into one larger tikzpicture.
 */
class TikzPicture : public TikzElement {
 public:
    /**
     * Creates a new tikzpicture / scope environment.
     * @param _options Options of the environment (e.g. scale=0.5)
     * @param _scope Generate a scope instead of a tikzpicture? (default: true)
     */
    explicit TikzPicture(std::string _options = "", bool _scope = true)
        : elements(), options(std::move(_options)), scope(_scope) {}
    void add(std::shared_ptr<TikzElement> tikzElement);

 protected:
    std::ostream &out(std::ostream &os) const override;

 private:
    std::vector<std::shared_ptr<TikzElement>> elements;
    std::string options;
    bool scope;
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZPICTURE_H_
