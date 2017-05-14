#pragma once

#include <nanogui/nanogui.h>

#include "showtime/sink.h"
#include "showtime/types.h"

namespace showtime {

class VisualizerSink : public Sink {
public:
  VisualizerSink();
  ~VisualizerSink() override;

  void sink(ColorChannels channels) override;

private:
  void makeTables();
  nanogui::GLShader table_shader_;
};

} // namespace showtime
