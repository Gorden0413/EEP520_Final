#ifndef __SNAKE_SEGMENT_AGENT__H
#define __SNAKE_SEGMENT_AGENT__H
#include "enviro.h"
#include <vector>
#include <string>
#include <cmath>
#include <chrono>
namespace enviro {
class snake_segment;
class snake_segmentController : public Process, public AgentInterface {
    public:
        snake_segmentController() : Process(), AgentInterface(),
                                   velocity(0), max_speed(120),
                                   turning_rate(0), is_head(false),
                                   follow_id(-1), world(nullptr), target_agent(nullptr),
                                   desired_velocity(0), desired_turning_rate(0),
                                   initialized(false), is_initialized(false),
                                   score(0), game_time(30.0), time_remaining(30.0),
                                   start_time(std::chrono::steady_clock::now()),
                                   game_over(false), scored_this_frame(false) {}
        void init();
        void start();
        void update();
        void stop() {}
        void set_world(World& w) { world = &w; }
        void set_spec(json s) { spec = s; }
        int get_score() const { return score; }
        double get_time_remaining() const { return time_remaining; }
    private:
        double velocity;
        double max_speed;
        double turning_rate;
        bool is_head;
        int follow_id;
        World* world;
        Agent* target_agent;
        json spec;
        double desired_velocity;
        double desired_turning_rate;
        bool initialized;
        bool is_initialized;
        int score;
        double game_time;
        double time_remaining;
        std::chrono::steady_clock::time_point start_time;
        bool game_over;
        bool scored_this_frame; 
};
class snake_segment : public Agent {
public:
    snake_segment(json spec, World& world) : Agent(spec, world), spec_(spec) {
        c.set_world(world);
        c.set_spec(spec);
        add_process(c);
    }
    json get_spec() const { return spec_; }
private:
    snake_segmentController c;
    json spec_;
};
DECLARE_INTERFACE(snake_segment)
}  // namespace enviro
#endif