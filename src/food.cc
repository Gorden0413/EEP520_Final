#include <iostream>
#include "food.h"

using namespace enviro;

namespace enviro {

void foodController::init() {
    eaten = false; // Initialize the eaten flag to false
}

void foodController::start() {
    initialized = true; // Mark the food as ready to begin logic
}

void foodController::update() {
    if (initialized && !is_initialized) { // Ensure initialization happens only once
        is_initialized = true;
        
        // Set up collision detection with snake segments
        notice_collisions_with("snake_segment", [this](Event& e) {
            if (eaten) return; // Skip if food is already being processed as eaten
            
            const auto& value = e.value();
            auto type_it = value.find("type");
            auto id_it = value.find("id");
            
            if (type_it != value.end() && id_it != value.end()) {
                std::string type = type_it->get<std::string>();
                int id = id_it->get<int>();
                
                // Check if collision is with the snake head (id == 0)
                if (type == "snake_segment" && id == 0) {
                    double dx = e.value()["x"].get<double>() - x();
                    double dy = e.value()["y"].get<double>() - y();
                    double distance = sqrt(dx * dx + dy * dy);
                    std::cout << "Collision detected with snake_segment_0, distance: " << distance << std::endl;
                    if (distance > 30) { // Ignore if too far (collision threshold)
                        std::cout << "Collision detected but too far: " << distance << std::endl;
                        return; 
                    }

                    eaten = true; // Mark food as eaten
                    // Randomly teleport food to a new position within game bounds
                    int new_x = rand() % 1101 - 550; // Range: -550 to 550
                    int new_y = rand() % 801 - 400; // Range: -400 to 400
                    teleport(new_x, new_y, 0);
                    std::string food_label = spec["position"]["label"].get<std::string>();
                    json event_data = {{"food_label", food_label}};
                    emit(Event("food_eaten", event_data)); // Notify the game of food being eaten
                    std::cout << "Food eaten at x=" << new_x << ", y=" << new_y 
                              << " by " << food_label << std::endl;
                    eaten = false; // Reset eaten flag for next cycle
                }
            }
        });
    }
}

}  // namespace enviro