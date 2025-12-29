#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include <array>

namespace top1::modules {

  class GridSeq : public SequencerModule {

    ui::ModuleScreen<GridSeq>::ptr screen;

  public:

    struct Props : Properties {
      Property<int>   rows       = {this, "ROWS",       8,    {4, 16, 1}};
      Property<int>   columns    = {this, "COLUMNS",    16,   {8, 32, 1}};
      Property<int>   division   = {this, "DIVISION",   4,    {1, 16, 1}};
      Property<float> swing      = {this, "SWING",      0.0,  {0.0, 0.75, 0.01}};
    } props;

    GridSeq();

    void process(const audio::ProcessData&) override;

    void display() override;

  private:
    static constexpr int MAX_ROWS = 16;
    static constexpr int MAX_COLS = 32;
    
    std::array<std::array<bool, MAX_COLS>, MAX_ROWS> grid;
    std::array<std::array<float, MAX_COLS>, MAX_ROWS> velocities;
    
    int currentColumn = 0;
    std::array<int, MAX_ROWS> activeNotes;
    int samplesAccumulator = 0;
    bool swingOffset = false;
    
    friend class GridSeqScreen;
  };
}
