#include "sealib/bfs/inplaceBFS.h"
#include <cmath>

using Sealib::ChoiceDictionaryDummy;
using Sealib::Compactgraph;
using Sealib::InplaceBFS;

ChoiceDictionaryDummy::ChoiceDictionaryDummy(uint n) : CompactArray(n, 4) {
    size = n;
}

void ChoiceDictionaryDummy::init(uint color) {
    current_position = 0;
    current_color = color;
}

bool ChoiceDictionaryDummy::more() {
    for (uint i = current_position; i < size; ++i) {
        if (get(i) == current_color) {
            current_position = i;
            return true;
        }
    }
    return false;
}

uint ChoiceDictionaryDummy::next() { return current_position++; }

void ChoiceDictionaryDummy::setColor(uint v, uint color) { insert(v, color); }

uint ChoiceDictionaryDummy::color(uint v) { return get(v); }

InplaceBFS::InplaceBFS(Compactgraph *graph, ChoiceDictionaryDummy *c,
                       UserFunc1 pp, UserFunc2 pe) {
    this->g = graph;
    this->color = c;
    for (uint i = 0; i < g->getOrder(); ++i) {
        color->insert(i, BFS_WHITE);
    }
    this->preprocess = pp;
    this->preexplore = pe;
    round_number = 0;
}

void InplaceBFS::runInplaceBFS() {
    uint order = g->getOrder();
    uint size = g->getData()[order + 1];
    uint array_length = order + size + 2;
    color->setColor(0, BFS_LIGHTGRAY);
    bool running = true;
    while (running) {
        running = false;
        if (round_number % 2 == 0) {
            color->init(BFS_LIGHTGRAY);
        } else {
            color->init(BFS_DARKGRAY);
        }
        while (color->more()) {
            uint v = color->next();
            uint adj_pointer = read(v + 1);
            uint u = g->getData()[adj_pointer];
            uint next_u = 0;
            if (adj_pointer < array_length) {
                next_u = g->getData()[++adj_pointer];
            }
            bool isSameAdjacency = true;
            while (isSameAdjacency) {
                if (preexplore != BFS_NOP_EXPLORE) {
                    preexplore(u, v);
                }
                if (color->color(u) == BFS_WHITE) {
                    if (preprocess != BFS_NOP_PROCESS) {
                        preprocess(u);
                    }
                    running = true;
                    if (round_number % 2 != 0) {
                        color->setColor(u, BFS_LIGHTGRAY);
                    } else {
                        color->setColor(u, BFS_DARKGRAY);
                    }
                }
                u = next_u;
                next_u = 0;
                if (adj_pointer < array_length) {
                    next_u = g->getData()[++adj_pointer];
                }
                if (u >= next_u) {
                    isSameAdjacency = false;
                }
            }
            color->setColor(v, BFS_BLACK);
        }
        round_number++;
    }
}
uint InplaceBFS::read(uint i) {
    unsigned int order = g->getData()[0];
    unsigned int wordsize = sizeof(g->getData()[0]) * 8;
    unsigned int c_prime = g->getData()[order];
    unsigned int wordsize_packed = wordsize - c_prime;
    unsigned int c_prime_mask = static_cast<unsigned int>(-1)
                                << (wordsize - c_prime);
    unsigned int initial_prefix = order + 2 & c_prime_mask;
    unsigned int current_prefix = initial_prefix >> (wordsize - c_prime);
    unsigned int pos_index = (unsigned int)order - pow(2, c_prime);
    while (i >= g->getData()[pos_index] && g->getData()[pos_index] != 0 &&
           pos_index < order) {
        current_prefix++;
        pos_index++;
    }
    unsigned int startbit = wordsize_packed * (i - 1);
    unsigned int offset = startbit & (wordsize - 1);
    unsigned int index = (startbit >> (unsigned int)log2(wordsize)) + 1;
    if (offset + wordsize_packed > wordsize) {
        unsigned int leftbits = wordsize - offset;
        unsigned int rightbits = wordsize_packed - leftbits;
        unsigned int value =
            (g->getData()[index] << rightbits) |
            (g->getData()[index + 1] >> (wordsize - rightbits));
        return (value & ~c_prime_mask) |
               (current_prefix << (wordsize - c_prime));
    } else {
        return (g->getData()[index] >> (c_prime - offset) & ~c_prime_mask) |
               (current_prefix << (wordsize - c_prime));
    }
}
