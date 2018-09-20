#include <sealib/tikzelement.h>

std::ostream &operator<<(std::ostream &os, const SealibVisual::TikzElement &tikzElement) {
    return tikzElement.out(os);
}