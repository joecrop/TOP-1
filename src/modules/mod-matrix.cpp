#include "modules/mod-matrix.hpp"
#include "core/globals.hpp"
#include "core/ui/drawing.hpp"

namespace top1::modules {

  ModMatrix::ModMatrix() 
    : screen(std::make_unique<ModMatrixScreen>(this)) {
    // Initialize with some default targets (will be expanded when synths register)
    // These are placeholder targets - actual synth params can be added dynamically
  }

  void ModMatrix::addTarget(const std::string& name,
                            std::function<void(float)> setter,
                            std::function<float()> getter,
                            float minVal, float maxVal) {
    availableTargets.push_back({name, setter, getter, minVal, maxVal});
  }

  void ModMatrix::assignLFO(int slot, int lfoIndex, int targetIndex) {
    if (slot < 0 || slot >= MAX_ASSIGNMENTS) return;
    if (targetIndex < 0 || targetIndex >= (int)availableTargets.size()) return;
    
    auto& target = availableTargets[targetIndex];
    auto& assign = assignments[slot];
    
    assign.lfoIndex = lfoIndex;
    assign.targetName = target.name;
    assign.active = true;
    
    // Create modulation function that scales LFO output to parameter range
    float range = target.maxVal - target.minVal;
    float baseVal = target.getter();
    
    assign.applyMod = [&target, range, baseVal](float lfoVal) {
      // lfoVal is -1 to 1 (bipolar) or 0 to 1 (unipolar)
      float modOffset = lfoVal * range * 0.5f;  // Scale by half range
      float newVal = baseVal + modOffset;
      newVal = std::max(target.minVal, std::min(target.maxVal, newVal));
      target.setter(newVal);
    };
  }

  void ModMatrix::clearSlot(int slot) {
    if (slot < 0 || slot >= MAX_ASSIGNMENTS) return;
    assignments[slot].active = false;
    assignments[slot].targetName = "";
    assignments[slot].applyMod = nullptr;
  }

  void ModMatrix::process() {
    for (auto& assign : assignments) {
      if (!assign.active || !assign.applyMod) continue;
      
      float lfoVal;
      if (assign.bipolar) {
        lfoVal = Globals::lfos[assign.lfoIndex].getValue();
      } else {
        lfoVal = Globals::lfos[assign.lfoIndex].getValueUnipolar();
      }
      
      lfoVal *= assign.amount;
      assign.applyMod(lfoVal);
    }
  }

  void ModMatrix::display() {
    Globals::ui.display(*screen);
  }

  /****************************************/
  /* ModMatrixScreen Implementation       */
  /****************************************/

  using namespace ui::drawing;

  void ModMatrixScreen::draw(Canvas& ctx) {
    using namespace ui::drawing;

    // Title
    ctx.font(Fonts::Bold);
    ctx.font(18);
    ctx.fillStyle(Colours::White);
    ctx.textAlign(TextAlign::Center, TextAlign::Middle);
    ctx.fillText("MOD MATRIX", 160, 18);

    // LFO indicators
    ctx.font(Fonts::Norm);
    ctx.font(12);
    for (int i = 0; i < 2; i++) {
      float lfoVal = Globals::lfos[i].getValue();
      float x = 40 + i * 120;
      float y = 40;
      
      ctx.fillStyle(i == 0 ? Colours::Blue : Colours::Green);
      ctx.textAlign(TextAlign::Left, TextAlign::Middle);
      ctx.fillText(fmt::format("LFO {}", i + 1), x, y);
      
      // Value bar
      ctx.beginPath();
      ctx.strokeStyle(Colours::Gray60);
      ctx.roundedRect(x + 40, y - 5, 60, 10, 2);
      ctx.stroke();
      
      float barWidth = (lfoVal + 1.0f) * 0.5f * 58;
      ctx.beginPath();
      ctx.fillStyle(i == 0 ? Colours::Blue : Colours::Green);
      ctx.roundedRect(x + 41, y - 4, barWidth, 8, 1);
      ctx.fill();
    }

    // Assignment slots
    float slotY = 65;
    float slotH = 18;
    int selectedSlot = module->props.selectedSlot.get();

    for (int i = 0; i < ModMatrix::MAX_ASSIGNMENTS; i++) {
      float y = slotY + i * (slotH + 2);
      auto& assign = module->assignments[i];
      
      // Background
      Colour bgCol = (i == selectedSlot) ? Colours::Gray70 : Colours::Gray70.dim(0.3f);
      ctx.beginPath();
      ctx.roundedRect(20, y, 280, slotH, 2);
      ctx.fillStyle(bgCol);
      ctx.fill();
      
      if (assign.active) {
        // LFO indicator
        ctx.font(10);
        ctx.fillStyle(assign.lfoIndex == 0 ? Colours::Blue : Colours::Green);
        ctx.textAlign(TextAlign::Left, TextAlign::Middle);
        ctx.fillText(fmt::format("L{}", assign.lfoIndex + 1), 25, y + slotH/2);
        
        // Arrow
        ctx.fillStyle(Colours::Gray60);
        ctx.fillText("->", 45, y + slotH/2);
        
        // Target name
        ctx.fillStyle(Colours::White);
        ctx.fillText(assign.targetName, 65, y + slotH/2);
        
        // Amount bar
        float amtX = 200;
        ctx.beginPath();
        ctx.strokeStyle(Colours::Gray60);
        ctx.roundedRect(amtX, y + 3, 60, slotH - 6, 2);
        ctx.stroke();
        
        float amtWidth = assign.amount * 58;
        ctx.beginPath();
        ctx.fillStyle(Colours::Red);
        ctx.roundedRect(amtX + 1, y + 4, amtWidth, slotH - 8, 1);
        ctx.fill();
        
        // Amount text
        ctx.fillStyle(Colours::White);
        ctx.textAlign(TextAlign::Right, TextAlign::Middle);
        ctx.fillText(fmt::format("{:.0f}%", assign.amount * 100), 295, y + slotH/2);
      } else {
        ctx.font(10);
        ctx.fillStyle(Colours::Gray60);
        ctx.textAlign(TextAlign::Center, TextAlign::Middle);
        ctx.fillText("- empty -", 160, y + slotH/2);
      }
      
      // Selection indicator
      if (i == selectedSlot) {
        ctx.beginPath();
        ctx.strokeStyle(Colours::White);
        ctx.lineWidth(2);
        ctx.roundedRect(19, y - 1, 282, slotH + 2, 3);
        ctx.stroke();
      }
    }

    // Mode indicator
    ctx.font(Fonts::Norm);
    ctx.font(10);
    ctx.fillStyle(Colours::Gray60);
    ctx.textAlign(TextAlign::Center, TextAlign::Top);
    
    const char* modeNames[] = {"SELECT SLOT", "SELECT TARGET", "SET AMOUNT"};
    ctx.fillText(modeNames[editMode], 160, 220);
    
    // Controls hint
    ctx.font(9);
    ctx.fillText("BLUE: Slot | GREEN: Target | WHITE: LFO | RED: Amount", 160, 232);
  }

  bool ModMatrixScreen::keypress(ui::Key key) {
    int slot = module->props.selectedSlot.get();
    
    switch (key) {
      case ui::Key::K_SHIFT:
        // Clear current slot
        module->clearSlot(slot);
        return true;
        
      case ui::Key::K_BLUE_CLICK:
        // Cycle edit mode
        editMode = (editMode + 1) % 3;
        return true;
        
      case ui::Key::K_GREEN_CLICK:
        // Assign current target to slot
        if (!module->availableTargets.empty()) {
          module->assignLFO(slot, module->props.selectedLFO, targetBrowseIndex);
        }
        return true;
        
      default:
        return false;
    }
  }

  void ModMatrixScreen::rotary(ui::RotaryEvent e) {
    int slot = module->props.selectedSlot.get();
    
    switch (e.rotary) {
      case ui::Rotary::Blue:
        // Select slot
        module->props.selectedSlot.step(e.clicks);
        break;
        
      case ui::Rotary::Green:
        // Browse targets
        if (!module->availableTargets.empty()) {
          targetBrowseIndex += e.clicks;
          targetBrowseIndex = std::max(0, std::min(targetBrowseIndex, 
            (int)module->availableTargets.size() - 1));
        }
        break;
        
      case ui::Rotary::White:
        // Select LFO
        module->props.selectedLFO.step(e.clicks);
        break;
        
      case ui::Rotary::Red:
        // Adjust amount
        if (module->assignments[slot].active) {
          module->assignments[slot].amount += e.clicks * 0.02f;
          module->assignments[slot].amount = std::max(0.0f, 
            std::min(1.0f, module->assignments[slot].amount));
        }
        break;
    }
  }

} // namespace top1::modules
