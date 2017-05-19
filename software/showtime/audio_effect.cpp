#include "audio_effect.h"

#include <iostream>
#include <numeric>

#include "showtime/audio_device.h"

namespace showtime {

ColorChannels AudioEffect::process(double t_seconds, void* /*data*/) {
  ColorChannels c;

  // Grab the latest spectrum data
  auto freq_out = AudioDevice::process();

  if (freq_out.size()) {
    // Grab the minimum and maximum values in the spectrum
    auto min = *std::min_element(freq_out.begin(), freq_out.end());
    auto max = *std::max_element(freq_out.begin(), freq_out.end());

    // Stupid way to slice up each section of the spectrum. Cut it
    // into c.size() sections and find the mean for each and that's
    // your final magnitude
    int step = (freq_out.size() / c.size());
    for (unsigned int idx = 0; idx < c.size(); ++idx) {
      const unsigned int freq_idx = idx * step;

      // Accumulate and average the values
      float total = std::accumulate(freq_out.begin() + freq_idx, freq_out.begin() + freq_idx + step, 0);
      float average = total / (float)step;

      // Pass on flat values. TODO: replace this with a spectrum of colors, maybe cool to hot?
      auto adjusted = average - min / (max - min);
      c[idx] = Color(adjusted, adjusted, adjusted);
    }
  }

  return c;
}

} // namespace showtime
