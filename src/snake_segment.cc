#include "snake_segment.h"
#include <sstream>
#include <iostream>

namespace enviro {

void snake_segmentController::init() {
    std::string agent_label = spec["position"]["label"].get<std::string>();
    is_head = (agent_label == "snake_segment_0"); // Identify if this is the snake head
}

void snake_segmentController::start() {
    initialized = true;
    start_time = std::chrono::steady_clock::now(); // Record game start time
    if (is_head) {
        std::cout << "Time: " << (int)game_time << "s, Score: " << score << std::endl;
        emit(Event("update_display", {{"score", score}})); // Update initial display
    }
}

void snake_segmentController::update() {
    if (initialized && !is_initialized) { // One-time initialization
        is_initialized = true;
        
        if (is_head) { // Logic specific to the snake head
            // Handle key presses for movement
            watch("keydown", [this](Event e) {
                auto k = e.value()["key"].get<std::string>();
                if (k == "w" && !game_over) desired_velocity = max_speed; // Move forward
                else if (k == "s" && !game_over) desired_velocity = -max_speed / 2; // Move backward
                else if (k == "a" && !game_over) desired_turning_rate = -16.0; // Turn left
                else if (k == "d" && !game_over) desired_turning_rate = 16.0; // Turn right
            });

            watch("keyup", [this](Event e) { // Stop movement on key release
                auto k = e.value()["key"].get<std::string>();
                if ((k == "w" || k == "s") && !game_over) desired_velocity = 0;
                else if ((k == "a" || k == "d") && !game_over) desired_turning_rate = 0;
            });

            // Handle food being eaten
            watch("food_eaten", [this](Event e) {
                if (!scored_this_frame) { // Prevent multiple scores in one frame
                    score += 1;
                    scored_this_frame = true;
                    std::string food_label = e.value()["food_label"].get<std::string>();
                    emit(Event("update_display", {{"score", score}}));
                    std::cout << "Food eaten, Score: " << score << " by " << food_label << std::endl;
                }
            });
        } else { // Logic for snake body segments
            std::string name_str = spec["position"]["label"].get<std::string>();
            int id;
            if (sscanf(name_str.c_str(), "snake_segment_%d", &id) == 1) {
                follow_id = id - 1; // Follow the segment ahead of this one
            }
            if (world != nullptr && follow_id >= 0) {
                std::string target_label = "snake_segment_" + std::to_string(follow_id);
                world->all([this, target_label](Agent& agent) { // Find the target segment
                    snake_segment* segment = dynamic_cast<snake_segment*>(&agent);
                    if (segment && segment->get_spec()["position"]["label"] == target_label) {
                        target_agent = &agent;
                    }
                });
            }
        }
    }
    
    if (is_head) { // Head-specific update logic
        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = current_time - start_time;
        time_remaining = game_time - elapsed.count(); // Update remaining time

        if (time_remaining <= 0 && !game_over) { // Check for game over
            game_over = true;
            std::cout << "Game Over! Final Score: " << score << std::endl;
            emit(Event("update_display", {{"score", score}, {"game_over", true}}));
            velocity = 0;
            turning_rate = 0;
            desired_velocity = 0;
            desired_turning_rate = 0;
            apply_force(0, 0);
            label("Game Over\nFinal Score: " + std::to_string(score), 0, -50);
            return;
        }

        if (!game_over) { // Update movement and display
            std::cout << "Time: " << (int)time_remaining << "s, Score: " << score << std::endl;
            emit(Event("update_display", {{"score", score}}));
            label("Time: " + std::to_string((int)time_remaining) + "s\nScore: " + std::to_string(score), 0, -20);

            velocity += (desired_velocity - velocity) * 0.1; // Smoothly adjust velocity
            turning_rate += (desired_turning_rate - turning_rate) * 0.1; // Smoothly adjust turning
            apply_force(velocity, turning_rate); // Apply movement

            // Keep snake within map boundaries
            const double MAP_MIN_X = -600;
            const double MAP_MAX_X = 600;
            const double MAP_MIN_Y = -450;
            const double MAP_MAX_Y = 450;
            double new_x = x();
            double new_y = y();
            if (new_x < MAP_MIN_X) new_x = MAP_MIN_X;
            if (new_x > MAP_MAX_X) new_x = MAP_MAX_X;
            if (new_y < MAP_MIN_Y) new_y = MAP_MIN_Y;
            if (new_y > MAP_MAX_Y) new_y = MAP_MAX_Y;
            if (new_x != x() || new_y != y()) {
                teleport(new_x, new_y, angle());
            }
        }
        scored_this_frame = false; // Reset scoring flag
    } else if (follow_id >= 0 && target_agent != nullptr && !game_over) { // Body segment following logic
        double dx = target_agent->x() - x();
        double dy = target_agent->y() - y();
        double dist = sqrt(dx*dx + dy*dy);
        if (dist > 2) { // Maintain distance from the segment ahead
            double desired_angle = target_agent->angle();
            double new_x = target_agent->x() - 20 * cos(desired_angle);
            double new_y = target_agent->y() - 20 * sin(desired_angle);
            teleport(new_x, new_y, desired_angle);
        }
    }
}

}  // namespace enviro