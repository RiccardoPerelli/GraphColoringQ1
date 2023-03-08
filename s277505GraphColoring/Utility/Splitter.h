#ifndef GRAPHCOLORINGPERELLI_SPLITTER_H
#define GRAPHCOLORINGPERELLI_SPLITTER_H

#include <cmath>
#include <vector>

class Splitter {
    int max;
    int items_per_range;

public:
    Splitter(int max, int num_ranges) : max(max) {
        items_per_range = std::ceil(float(max) / float(num_ranges));
    };

    // Get the lower end for the i-th range
    inline int get_min(int index) const {
        return std::min(items_per_range * index, max);
    };
    // Get the higher end for the i-th range
    inline int get_max(int index) const {
        return std::min(items_per_range * (index + 1), max);
    };
    // Get whether the i-th range is empty
    inline bool is_empty(int index) const {
        return get_min(index) == get_max(index);
    };
};

class VSplitter {
    Splitter rs;
    std::vector<uint32_t> &vec;

public:
    VSplitter(std::vector<uint32_t> &vec, int num_ranges) : rs(vec.size(), num_ranges), vec(vec){};

    // Get the i-th range
    std::vector<uint32_t> get(int index) {
        if (rs.is_empty(index))
            return std::vector<uint32_t>();
        return std::vector<uint32_t>(vec.begin() + rs.get_min(index), vec.begin() + rs.get_max(index));
    };
};

#endif //GRAPHCOLORINGPERELLI_SPLITTER_H
