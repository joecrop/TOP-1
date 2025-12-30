#pragma once

#include "core/modules/module.hpp"
#include "core/modules/module-props.hpp"
#include "core/ui/module-ui.hpp"
#include "core/ui/drawing.hpp"
#include <vector>
#include <string>
#include <functional>

namespace top1::modules {

  class ModMatrixScreen;

  /**
   * Modulation assignment - routes an LFO to a parameter
   */
  struct ModAssignment {
    int lfoIndex = 0;           // Which LFO (0 or 1)
    std::string targetName;     // Display name of target
    float amount = 0.5f;        // Modulation amount (0-1)
    bool bipolar = true;        // Bipolar (-1 to 1) or unipolar (0 to 1)
    bool active = false;
    
    // Function pointer to apply modulation
    std::function<void(float)> applyMod;
  };

  /**
   * Modulation Matrix - Routes LFOs to synth/effect parameters
   * Inspired by OP-1's LFO assignment system
   */
  class ModMatrix : public Module {
  public:
    static constexpr int MAX_ASSIGNMENTS = 8;

    struct Props : public Properties {
      Property<int> selectedSlot = {this, "slot", 0, {0, MAX_ASSIGNMENTS - 1, 1}};
      Property<int> selectedLFO  = {this, "lfo", 0, {0, 1, 1}};
      Property<float> amount     = {this, "amount", 0.5f, {0.0f, 1.0f, 0.02f}};
    } props;

    std::array<ModAssignment, MAX_ASSIGNMENTS> assignments;
    
    // Available modulation targets (populated from synth/effect modules)
    struct ModTarget {
      std::string name;
      std::function<void(float)> setter;
      std::function<float()> getter;
      float minVal;
      float maxVal;
    };
    std::vector<ModTarget> availableTargets;

    ModMatrix();
    ~ModMatrix() = default;

    void process();  // Apply all active modulations
    void display();
    
    void addTarget(const std::string& name, 
                   std::function<void(float)> setter,
                   std::function<float()> getter,
                   float minVal, float maxVal);
    void assignLFO(int slot, int lfoIndex, int targetIndex);
    void clearSlot(int slot);

  private:
    std::unique_ptr<ModMatrixScreen> screen;
    int currentTargetIndex = 0;
  };

  class ModMatrixScreen : public ui::ModuleScreen<ModMatrix> {
  public:
    using ui::ModuleScreen<ModMatrix>::ModuleScreen;
    
    int editMode = 0;  // 0 = slot select, 1 = target select, 2 = amount
    int targetBrowseIndex = 0;
    
    void draw(ui::drawing::Canvas& ctx) override;
    bool keypress(ui::Key key) override;
    void rotary(ui::RotaryEvent e) override;
  };

} // namespace top1::modules
