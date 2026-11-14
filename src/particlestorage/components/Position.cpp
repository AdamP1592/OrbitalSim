#include "Position.hpp"

Position::Position(vec3<double> pos_){
    pos = pos_;
}

void Position::set(vec3<double> newPosition){
    pos = newPosition;
}