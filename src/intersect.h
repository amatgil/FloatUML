#include "raylib.h"
#include <math.h>

struct Vector2 int_seg_rect(Vector2 start, Vector2 end, Rectangle rect) {
    float m = (end.y - start.y) / (end.x - start.x);
    float n = start.y - start.x * m;

    Vector2 minpt = {fmin(start.x, end.x), fmin(start.y, end.y)};
    Vector2 maxpt = {fmax(start.x, end.x), fmax(start.y, end.y)};

    Vector2 leftp = {0, 0};
    leftp.y = m * rect.x + n;
    leftp.x = rect.x;
    if (leftp.y >= minpt.y && leftp.y <= maxpt.y &&
        leftp.y <= rect.y + rect.height && leftp.y >= rect.y)
        return leftp;

    Vector2 rightp = {0, 0};
    rightp.y = m * (rect.x + rect.width) + n;
    rightp.x = (rect.x + rect.width);
    if (rightp.y >= minpt.y && rightp.y <= maxpt.y &&
        rightp.y <= rect.y + rect.height && rightp.y >= rect.y)
        return rightp;

    Vector2 topp = {0, 0};
    topp.x = (rect.y - n) / m;
    topp.y = rect.y;
    if (topp.x >= minpt.x && topp.x <= maxpt.x && topp.x >= rect.x &&
        topp.x <= rect.x + rect.width)
        return topp;

    Vector2 bottomp = {0, 0};
    bottomp.x = (rect.y + rect.height - n) / m;
    bottomp.y = rect.y + rect.height;
    return bottomp;
}
