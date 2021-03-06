#include "MemoryFactory.h"
#include "LPDDR4.h"
#include "WideIO.h"
#include "WideIO2.h"
#include "HBM.h"
#include "SALP.h"

using namespace ramulator;

template <>
void MemoryFactory<LPDDR4>::validate(int channels, int ranks, map<string, string>& options) {
    assert(channels >= 2 && "LPDDR4 requires 2, 4, 8 ... channels");
}

template <>
void MemoryFactory<WideIO>::validate(int channels, int ranks, map<string, string>& options) {
    assert(channels == 4 && "WideIO comes with 4 channels");
}

template <>
void MemoryFactory<WideIO2>::validate(int channels, int ranks, map<string, string>& options) {
    assert((channels == 4 || channels == 8) && "WideIO2 comes with 4 or 8 channels");
    assert((ranks == 1 || ranks == 2) && "WideIO2 comes with 1 or 2 ranks");
}

template <>
void MemoryFactory<HBM>::validate(int channels, int ranks, map<string, string>& options) {
    assert(channels == 8 && "HBM comes with 8 channels");
}

template <>
MemoryBase *MemoryFactory<WideIO2>::create(map<string, string>& options, int cacheline) {
    int channels = stoi(options["channels"], NULL, 0);
    int ranks = stoi(options["ranks"], NULL, 0);
    validate(channels, ranks, options);

    string& org_name = options["org"];
    string& speed_name = options["speed"];

    WideIO2 *spec = new WideIO2(org_name, speed_name, channels);

    extend_channel_width(spec, cacheline);

    return (MemoryBase *)populate_memory(spec, channels, ranks);
}


template <>
MemoryBase *MemoryFactory<SALP>::create(map<string, string>& options, int cacheline) {
    int channels = stoi(options["channels"], NULL, 0);
    int ranks = stoi(options["ranks"], NULL, 0);
    int subarrays = stoi(options["subarrays"], NULL, 0);
    validate(channels, ranks, options);

    string& std_name = options["standard"];
    string& org_name = options["org"];
    string& speed_name = options["speed"];

    SALP *spec = new SALP(org_name, speed_name, std_name, subarrays);

    extend_channel_width(spec, cacheline);

    return (MemoryBase *)populate_memory(spec, channels, ranks);
}

