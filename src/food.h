#ifndef __FOOD_AGENT__H
#define __FOOD_AGENT__H
#include "enviro.h"
#include "agent_interface.h"
namespace enviro {
class foodController : public Process, public AgentInterface {
public:
    foodController() : Process(), AgentInterface(), initialized(false), is_initialized(false), eaten(false) {}
    void init();
    void start();
    void update();
    void stop() {}
    void set_spec(json s) { spec = s; }
private:
    bool initialized;
    bool is_initialized;
    bool eaten;
    json spec;
};

class food : public Agent {
public:
    food(json spec, World& world) : Agent(spec, world) {
        c.set_spec(spec);
        add_process(c);
    }
private:
    foodController c;
};
DECLARE_INTERFACE(food)
}
#endif