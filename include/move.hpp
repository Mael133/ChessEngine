#pragma once
#include "position.hpp"

struct Move{
    Position startPosition = Position(0);
    Position targetPosition = Position(0);
    bool isCapture = false;
};