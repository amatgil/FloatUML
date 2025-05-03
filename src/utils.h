
#include "raylib.h"
Vector2 rect_center(Rectangle rect) {
    return (Vector2){rect.x + rect.width / 2, rect.y + rect.height / 2};
}
