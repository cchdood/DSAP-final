#ifndef DSAP_CUSTOM_CONTROLLER_H
#define DSAP_CUSTOM_CONTROLLER_H

#include "ISnakeController.h"
#include "DirectionType.h"

#include <math.h>
class CustomController : public ISnakeController {
public:
    DirectionType NextDirection(const Game& game, size_t id) override 
	{
		const float pi = 3.14159;
		const Snake snake = game.Snakes().at(id);
		const float r_ = 113.59;
		size_t counter = 0;
		Position pos = snake.Head(); 
		Position fifthpos;
		for(auto it = snake.Body().rbegin(); it != snake.Body().rend(); ++it) {
			if(counter >= 5) break;
			if(counter == 4) fifthpos = *it;
			counter++;
		}
		Position p = pos - fifthpos;
		if(p.Length() - head_to_fifth < 1) {
			recurse++;
		}
		head_to_fifth = p.Length();

		Snake near = snake;
		Position npos;
		float minidis = 300;
		for (auto itr = game.Snakes().begin(); itr != game.Snakes().end(); ++itr) 
		{
			const size_t tID = itr->first;
            const Snake& tSnake = itr->second;
			if(tID == id) continue;
			const std::list<Position> check_body = tSnake.Body();
			for(auto it = check_body.rbegin(); it != check_body.rend(); ++it) {
				Position tpos = *it;
				Position dis = tpos - pos;
				if (dis.Length() < minidis) {
				minidis = dis.Length();
				near = tSnake;
				npos = tpos;
				}
			}
		}
		
		if(near.Id() != snake.Id())
		{
			if(1560<game.Time() && game.Time()<1890) 
			{
				if (pos.y < 600)
				{
					return DirectionType::kRight;
				}
				else return DirectionType::kLeft;
			}
            else return DirectionType::kLeft;
		}
			
		
		if(pos.x > game.FieldWidth() - 290 || pos.x < 290 || pos.y < 290 || pos.y > game.FieldHeight() - 290)
		{
			float rad = 1.0 * snake.Direction() / 180 * pi;
			float rc_x, rc_y, lc_x, lc_y;
			rc_x = pos.x + r_ * cos(rad - pi/2);
			rc_y = pos.y + r_ * sin(rad - pi/2);
			lc_x = pos.x + r_ * cos(rad + pi/2);
			lc_y = pos.y + r_ * sin(rad + pi/2);
			
			if((pos.x > game.FieldWidth() - 300 || pos.x < 300) && (pos.y < 300 || pos.y > game.FieldHeight() - 300))
			{
				if(rc_x - r_ < 250 || rc_x + r_ > game.FieldWidth() - 250 || rc_y - r_ < 250 || rc_y + r_ > game.FieldHeight() - 250)
			        {
				        return DirectionType::kLeft;
		            }
			    if(lc_x - r_ < 250 || lc_x + r_ > game.FieldWidth() - 250 || lc_y - r_ < 250 || lc_y + r_ > game.FieldHeight() - 250)
			        {
			            return DirectionType::kRight;
		            }
			}
			
			if(rc_x - r_ < 100 || rc_x + r_ > game.FieldWidth() - 100 || rc_y - r_ < 100 || rc_y + r_ > game.FieldHeight() - 100)
			{
				return DirectionType::kLeft;
		    }
			if(lc_x - r_ < 100 || lc_x + r_ > game.FieldWidth() - 100 || lc_y - r_ < 100 || lc_y + r_ > game.FieldHeight() - 100)
			{
			    return DirectionType::kRight;
		    }
		}
		
		std::vector<Position> foods = game.Foods();
		Position Destination;
		float mindistance = 1000000;
		float rad = 1.0 * snake.Direction() / 180 * pi;
		for (int i = 0; i < 100; i++) {
			Position food_pos = foods[i];
			Position food_vec = food_pos - pos;
			float distance = food_vec.Length();
			Position dir_vec;
			dir_vec.x = cos(rad); dir_vec.y = sin(rad);
			if (distance < mindistance && food_vec.InnerProduct(dir_vec) > food_vec.Length() * cos(pi/6)) {
				mindistance = distance;
				Destination.x = food_pos.x;
				Destination.y = food_pos.y;
			}
		}
		if(std::abs(sin(rad)) < 0.01 && std::abs(cos(rad) - 1) < 0.01) {
			if(Destination.y > pos.y) return DirectionType::kRight;
			else if(Destination.y < pos.y) {
				return DirectionType::kLeft;
			}
			else return DirectionType::kForward;
		}
		else if(std::abs(sin(rad)) < 0.01 && std::abs(cos(rad) + 1) < 0.01) {
			if(Destination.y > pos.y) {
				return DirectionType::kLeft;
			}
			else if(Destination.y < pos.y) return DirectionType::kRight;
			else return DirectionType::kForward;
		}
		else if(std::abs(sin(rad) - 1) < 0.01 && std::abs(cos(rad)) < 0.01) {
			if(Destination.x > pos.x) {
				return DirectionType::kLeft;
			}
			else if(Destination.x < pos.x) return DirectionType::kRight;
			else return DirectionType::kForward;
		}
		else if(std::abs(sin(rad) + 1) < 0.01 && std::abs(cos(rad)) < 0.01) {
			if(Destination.x > pos.x) return DirectionType::kRight;
			else if(Destination.x < pos.x) {
				return DirectionType::kLeft;
			}
			else return DirectionType::kForward;
		}
		else if(tan(rad) * (Destination.x - pos.x) + pos.y > Destination.y) {
			if(0 < snake.Direction()) {
				return DirectionType::kLeft;
			}
			else return DirectionType::kRight;
		}
		else if(tan(rad) * (Destination.x - pos.x) + pos.y < Destination.y) {
			if(0 < snake.Direction()) return DirectionType::kRight;
			else {
				return DirectionType::kLeft;
			}
		}
		
		return DirectionType::kForward;
    }
    
private:
	int recurse;
	float head_to_fifth;
	int recursedur;
	bool flee;
};

#endif // DSAP_CUSTOM_CONTROLLER_H


